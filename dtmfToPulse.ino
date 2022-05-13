#include <Ticker.h>
// https://github.com/sstaub/Ticker

// NOTE no delay() calls allowed, use the timers and the state machine to implement delays and control flow
//  this way you can have inputs and outputs occuring at the same time without conflicts

// 1 pulse = 50ms on, 50ms off

// https://wokwi.com/projects/331414878730322516

///////////
//
// some lovely tweakables.. Hint: change one thing at a time

// uncomment for debug settings
// #define DEBUG

//    uncomment if i2c LCD display present
#define LCD_DISPLAY

#define SPEED 1
//  vvv uncomment to go a factor of 4 slower
//#define SPEED 4


//  Delay Dial Control:
//     set DELAY_DAIL to 1 to wait until after DELAY_TIMEOUT ms from last typed digit to start pulse dialing

#define DELAY_DAIL  0
#define DELAY_TIMEOUT 1000*SPEED


//  Minimum Digits:
//     less than this number of digits toned and if you timeout you get hung up
//     set to 0 to turn off

#define MIN_DIGITS 200


// Initial High Control:
//   comment line below to stop initial HIGH pulse on start of dialling, used with High Low Hangup below
//#define INITIAL_HIGH


//  Input Settling Time:
//     how long to wait for inputs to settle (ms)
#define INPUT_SETTLE_TIME 10


//   Inactive Hangup Control:
//     comment out line below if you ONLY want a '#' to trigger hangups and not timeouts or too few digits 
// #define INACTIVE_HANGUP


//   High Low Hangup:
//     uncomment if you want the hang up to finish in a LOW state
#define END_LOW_HANGUP

//  Short Low Hangup:
//     uncomment to not do 2nd round of HIGH/LOW pulse
#define SHORT_LOW_HANGUP


//  Start High:
//     uncomment so at start up it starts HIGH
#define START_HIGH


// Idle Hangup:
//     uncomment line below so that when an idle has occurred it will inject a hangup before the
//       next dialed number
#define IDLE_HANGUP



//  Start Hangup:
//     uncomment line below so from power up inject a hangup with first caller
#define START_HANGUP





#ifdef LCD_DISPLAY
// https://github.com/johnrickman/LiquidCrystal_I2C
// under code grab the zip and install
//
//   LCD        Nano
//   ===        ====
//   GND         GND
//   Vcc          5v
//   SDA          A4
//   SCL          A5

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif

//   Individual Timing Controls
int pulse_length_make = 50*SPEED;
int pulse_length_break = 50*SPEED;
int pulse_hangup_delay = 1000*SPEED;
int interdigit_gap = 300*SPEED;
int initial_gap = 300*SPEED;

//   how long to be inactive before a hangup gets sent and it resets (in seconds)
#ifndef DEBUG
long hangup_timeout = 10*SPEED;
#else
long hangup_timeout = 3*SPEED;
#endif

// long idle timeout after which it will set HIGH if in state 0
long idle_timeout = 1*60;   // in seconds

long last_digit_time = 0;


#define FIFO_LEN 20
volatile boolean doDTMFread=false;  // flag to main routine to do DTMF read
byte fifo[FIFO_LEN];  // basic circular buffer
byte fifoIn=0,fifoOut=0;  // head and tail pointers
byte fifoDone=0; // beginning of display
byte state=0, old_state=0;      // state machine current state
byte numberP=0;    // number being pulsed
volatile unsigned short timer0=0,timer1=0,timerSec0=0,timerSec1=0,timerSec2=0;   // 3 timers that countdown, last 3 in seconds
volatile long secCount=0;
boolean timerSec0Fired=false,timerSec1Fired=false,timerSec2Fired=false;
boolean injectHangup=false;
byte digitCount=0;  // count the digits typed, less than MIN_DIGITS and it does a hangup
unsigned int callCount=0;  // keep track of calls processed

// pin config
#define stq_pin 3 //for nano, can only use D2 or D3 for interrupt pins.
#define q1_pin 4
#define q2_pin 5
#define q3_pin 6
#define q4_pin 7

#define output_pin 8



void read_dtmf_inputs_intr() {
  // keep very short
  doDTMFread=true;
  timer1=INPUT_SETTLE_TIME;  // settle down delay before reading pins
}





void tickerKick() {  // the ticker routine
  if(timer0>0) timer0--;  // countdown timer 0
  if(timer1>0) timer1--;  // countdown timer 1
  secCount++;
  if(secCount>=1000) {
    secCount=0;
    if(timerSec0>0) {
      timerSec0--;
      if(timerSec0==0) timerSec0Fired=true;   // it only gets set once..
    }
    if(timerSec1>0) {
      timerSec1--;
      if(timerSec1==0) timerSec1Fired=true;   // it only gets set once..
    }
    if(timerSec2>0) {
      timerSec2--;
      if(timerSec2==0) timerSec2Fired=true;   // it only gets set once..
    }
  }
}


// ticker routine goes off every 1ms
Ticker ticker(tickerKick,1);

void backLightOn() {
  if(timerSec1==0) {
#ifdef LCD_DISPLAY
    lcd.backlight();
#endif
    timerSec1=60;
  }
}


void setStatus(char *s) {
#ifdef LCD_DISPLAY
  lcd.setCursor(0,0);
  lcd.print(s);
  backLightOn();
#endif
}


void displayState() {
#ifdef LCD_DISPLAY
  char st[5];
  sprintf(st,"S%03d",state);
  lcd.setCursor(12,1);
  lcd.print(st);
#endif
}


char mapFifo(byte c) {
  if(c==0xc) return '#';
  if(c<10) return (char)(48+c);
  return '0';
}


void displayQueue() {
#ifdef LCD_DISPLAY
  int i=fifoDone,p=0;
  while((i!=fifoIn) && (p<8)) {
    char c[2];
    if(i==fifoOut) {
      lcd.setCursor(p,1);
      p++;
      lcd.print("^");
    }
    c[0]=mapFifo(fifo[i]);
    lcd.setCursor(p,1);
    c[1]=0;
    lcd.print(c);
    p++;
    i=(i+1)%FIFO_LEN;
  }
  if(i==fifoOut) {
    lcd.setCursor(p,1);
    p++;
    lcd.print("^");
  }
  while(p<9) {
    lcd.setCursor(p,1);
    lcd.print(" ");
    p++;
  }
#endif
}

void resetQueue() {
  fifoIn=fifoOut=fifoDone=0;
  digitCount=0;
  displayQueue();
}

int putOnQueue(uint8_t numberP) {
  if(numberP>0) {
    // put on end of queue
    fifo[fifoIn]=numberP;
    fifoIn=(fifoIn+1)%FIFO_LEN;
    displayQueue();
    return 1;
  }
  return 0;
}


void displayCallCount() {
#ifdef LCD_DISPLAY
  char s[5];
  sprintf(s,"C%03d",callCount);
  lcd.setCursor(12,0);
  lcd.print(s);
#endif
}





void setup() {
  Serial.begin(9600);
  Serial.println("Hello, I'm in a terminal! Feed me tones");
  Serial.println();

  /*Define input pins for DTMF Decoder pins connection */
  pinMode(stq_pin, INPUT); // connect to Std pin
  pinMode(q4_pin, INPUT); // connect to Q4 pin
  pinMode(q3_pin, INPUT); // connect to Q3 pin
  pinMode(q2_pin, INPUT); // connect to Q2 pin
  pinMode(q1_pin, INPUT); // connect to Q1 pin
  // output
  pinMode(output_pin, OUTPUT); // connect to output

  attachInterrupt(digitalPinToInterrupt(stq_pin), read_dtmf_inputs_intr, FALLING);
  ticker.start();   // lets get ticking

#ifndef END_LOW_HANGUP
  digitalWrite(output_pin, HIGH);
#endif 

#ifdef START_HIGH
  digitalWrite(output_pin, HIGH);
#endif 

#ifdef START_HANGUP
  injectHangup=true;
#endif


#ifdef LCD_DISPLAY
  lcd.init();                      // initialize the lcd
#endif 
  // Print a message to the LCD.
  setStatus("GDay Mate!");

}





void doStates() {
  switch(state) {   // state matches to each case..
    case 0:  // start state
      if(fifoIn!=fifoOut) {  // someone dialed in, move to state 1
        if( (DELAY_DAIL==0) || ((last_digit_time+DELAY_TIMEOUT)<millis()) ) {
          callCount++;
          displayCallCount();
          timerSec0=0;  // no dangling hangups pending
          timerSec2=idle_timeout;  // idle timeout reset
#ifdef INITIAL_HIGH
          digitalWrite(output_pin, HIGH);  // an initial "I'm here" HIGH
          timer0=initial_gap;
          state=1;
#else
          state=2; // jump right into dialling
#endif
        }
      }
      break;

    case 1:
      if(timer0==0) {
        digitalWrite(output_pin, LOW);  // I'm here end
        state=2;
        timer0=pulse_length_break;
      }
      break;

    case 2: 
       if(timer0==0) {
        if(fifoIn!=fifoOut) {  // something on the queue, grab it and move to state 3
          numberP=fifo[fifoOut];
          fifoOut=(fifoOut+1)%FIFO_LEN;
          displayQueue();
          char temp[10];
          sprintf(temp,"Dial %c    ",mapFifo(numberP));
          setStatus(temp);
          state=3;
        }
      }
      break;  // we wait here for more digits..  hangup timeout applies here only

    case 3:
      if(numberP==0x0c) {
        state=100; // hangup, move to state 100
        setStatus("Hang Up   ");
      } else {
        state=4; // regular digit, move to state 4
      }
      break;

    case 4: {
      digitalWrite(output_pin, HIGH);  //set high for pulse time and move to state 5
      timer0=pulse_length_make;
      state=5;
      break;
    }

    case 5: {
      if(timer0==0) {  // only do when timer stopped
        digitalWrite(output_pin, LOW); // set low for pulse time and time to state 6
        timer0=pulse_length_break;
        state=6;
      }
      break;
    }

    case 6: {
      if(timer0==0) {     // timer finished
        numberP--;        // done one pulse train
        if(numberP==0) {  // none left
          timer0=interdigit_gap; // time the gap and move to state 7
          state=7;
          break;
        }
        state=4;  // next pulse
      }
      break;
    }

    case 7: {
      if(timer0==0) {
        state=2;  // back to the start for next digit
        if(fifoIn==fifoOut) setStatus("Done      ");
#ifdef INACTIVE_HANGUP
        timerSec0=hangup_timeout;  // hangup if inactive
#endif
      }
    }
    break;

    case 100: {  // hang up
      digitalWrite(output_pin, HIGH);  // set high for hangup delay
      timer0=pulse_hangup_delay;
      state=101;
    }
    break;

    case 101: {
      if(timer0==0) {
        digitalWrite(output_pin, LOW);  // set low for 2* hangup delay
        timer0=pulse_hangup_delay*2;
        state=102;
      }
      break;
    }

    case 102: {
#ifdef END_LOW_HANGUP
#ifdef SHORT_LOW_HANGUP
      state=0;
      timerSec2=idle_timeout;
      setStatus("Wait   ");
#else
      digitalWrite(output_pin, HIGH);
      timer0=pulse_hangup_delay*2;  // set high for 2* hangup
      state=103;
#endif
#else
      digitalWrite(output_pin, HIGH);
      state=0;
      timerSec2=idle_timeout;
      setStatus("Wait   ");
#endif
      break;
    }

    case 103: {
      if(timer0==0) {
        digitalWrite(output_pin, LOW);  // I set this low, seems weird to leave high..
        state=0; // back to the start for next digit
        timerSec2=idle_timeout;
        setStatus("Wait   ");
      }
    }
    break;
  }

}


void loop() {
  ticker.update();  // tick tick goes the ticker...
  
  if( doDTMFread && (timer1==0)) { // read the DTMF
    doDTMFread=false;   //done handling interrupted flag
    int readDigits=read_dtmf_inputs();
    last_digit_time = millis();
    digitCount++;
    if(digitCount>8) fifoDone=(fifoDone+readDigits)%FIFO_LEN;  // for the queue display
    if(readDigits>0) displayQueue();
  }
  
  doStates();   // the magical state machine

  if(state!=old_state) {
    Serial.print(old_state);
    Serial.print(" -> ");
    Serial.println(state);
    displayState();
    old_state=state;
  }


  if ( timerSec0Fired && (state==2)) {
    timerSec0Fired=false;
    if(digitCount<MIN_DIGITS) {
      fifo[fifoIn]=0x0c;  // throw a hangup on the queue
       fifoIn=(fifoIn+1)%FIFO_LEN;
      Serial.println("Slow and too few digits => hangup");
    }
    else {
      state=0;  // all good, we are done 
      Serial.println("Assume got connected, wait for more...");
      setStatus("Wait   ");
    }
    digitCount=0;   // reset count
  }

  if(timerSec1Fired) {  // put out the backlight after timeout
#ifdef LCD_DISPLAY
    lcd.noBacklight();
#endif
    timerSec1Fired=false;
  }

  if(timerSec2Fired) {
   
   if((state==0) || (state==2)) {
      digitalWrite(output_pin, HIGH);  // idled out, set HIGH
      setStatus("Idled   ");
      state=0;
      resetQueue();
#ifdef IDLE_HANGUP
      injectHangup=true;
#endif
    }
    timerSec2Fired=false;
  }
}

int read_dtmf_inputs()
{
  Serial.println("Hello, I'm in read_dtmf_inputs()!");
  Serial.println();
  uint8_t number_pressed, readDigits=0;
  // delay(250);  // done with timer1
  // checks q1,q2,q3,q4 to see what number is pressed.
  number_pressed = ( 0x00 | (digitalRead(q4_pin)<<0) | (digitalRead(q3_pin)<<1) | (digitalRead(q2_pin)<<2) | (digitalRead(q1_pin)<<3) );
  switch (number_pressed)
  {
    case 0x01:
    Serial.println("Button Pressed =  1");
    break;
    case 0x02:
    Serial.println("Button Pressed =  2");
    break;
    case 0x03:
    Serial.println("Button Pressed =  3");
    break;
    case 0x04:
    Serial.println("Button Pressed =  4");
    break;
    case 0x05:
    Serial.println("Button Pressed =  5");
    break;
    case 0x06:
    Serial.println("Button Pressed =  6");
    break;
    case 0x07:
    Serial.println("Button Pressed =  7");
    break;
    case 0x08:
    Serial.println("Button Pressed =  8");
    break;
    case 0x09:
    Serial.println("Button Pressed =  9");
    break;
    case 0x0A:
    Serial.println("Button Pressed =  0");
    break;
    case 0x0B:
    Serial.println("Button Pressed =  *");
    break;
    case 0x0C:
    Serial.println("Button Pressed =  #");
    timerSec0=hangup_timeout;
    break;    
  }

  if(injectHangup) {
    if(putOnQueue(0x0c)) readDigits++; // wakey wakey!
    injectHangup=false;
  }

  if(putOnQueue(number_pressed)) readDigits++;

  return readDigits;
}
