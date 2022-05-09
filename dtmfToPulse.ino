#include <Ticker.h>
// https://github.com/sstaub/Ticker

// NOTE no delay() calls allowed, use the timers and the state machine to implement delays and control flow
//  this way you can have inputs and outputs occuring at the same time without conflicts

// 1 pulse = 50ms on, 50ms off
int pulse_length_make = 33;
int pulse_length_break = 66;
int pulse_hangup_delay = 1000;
int interdigit_gap = 300;

//int hangup_timeout = 60*1000*15;
int hangup_timeout = 3000;

long last_hash_time = 0;



#define FIFO_LEN 20
volatile boolean doDTMFread=false;  // flag to main routine to do DTMF read
byte fifo[FIFO_LEN];
byte fifoIn=0,fifoOut=0;
byte state=0;
byte numberP=0;
volatile unsigned short timer0=0,timer1=0;



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
  timer1=250;
}

void tickerKick() {
  if(timer0>0) timer0--;  // countdown timer 0
  if(timer1>0) timer1--;  // countdown timer 1
}

// ticker routine goes off every 1ms

Ticker ticker(tickerKick,1);


void setup() {
  Serial.begin(9600);
  Serial.println("Hello, I'm in a terminal!");
  Serial.println();

  /*Define input pins for DTMF Decoder pins connection */
  pinMode(stq_pin, INPUT); // connect to Std pin
  pinMode(q4_pin, INPUT); // connect to Q4 pin
  pinMode(q3_pin, INPUT); // connect to Q3 pin
  pinMode(q2_pin, INPUT); // connect to Q2 pin
  pinMode(q1_pin, INPUT); // connect to Q1 pin

  attachInterrupt(digitalPinToInterrupt(stq_pin), read_dtmf_inputs_intr, FALLING);
}


void doStates() {
  switch(state) {
    case 0:
      if(fifoIn!=fifoOut) {  // something on the queue, send it out
        numberP=fifo[fifoIn];
        fifoIn=(fifoIn+1)%FIFO_LEN;
        state=1;
      }
      break;

    case 1:
      if(numberP==0x0c) {
        state=100; // hangup
      } else {
        state=2;
      }
      break;

    case 2: {
      digitalWrite(output_pin, HIGH);
      timer0=pulse_length_make;
      state=3;
      break;
    }

    case 3: {
      if(timer0==0) {
        digitalWrite(output_pin, LOW);
        timer0=pulse_length_break;
        state=4;
      }
      break;
    }

    case 4: {
      if(timer0==0) {
        numberP--;
        if(numberP==0) {
          timer0=interdigit_gap;
          state=5;
          break;
        }
        state=2;
      }
      break;
    }

    case 5: {
      if(timer0==0) {
        state=0;  // next digit
      }
    }
    break;

    case 100: {  // hang up
      digitalWrite(output_pin, HIGH);
      timer0=pulse_hangup_delay;
      state=101;
    }
    break;

    case 101: {
      if(timer0==0) {
        digitalWrite(output_pin, LOW);
        timer0=pulse_hangup_delay*2;
        state=102;
      }
      break;
    }

    case 102: {
      digitalWrite(output_pin, HIGH);
      timer0=pulse_hangup_delay*2;
      state=103;
    }

    case 103: {
      if(timer0==0) {
        digitalWrite(output_pin, LOW);
        state=0;
      }
    }
    break;
  }

}


void loop() {
  ticker.update();
  if( doDTMFread && (timer1==0)) { // read!
    read_dtmf_inputs();
    doDTMFread=false; //done handling interrupted flag
  }
  doStates();

  long now = millis();
  long diff_times = (now-last_hash_time);
  if ( (diff_times > (hangup_timeout)) & (diff_times != 0) )
  {
    fifo[fifoOut]=0x0c;
    fifoOut+(fifoOut+1)%FIFO_LEN;
    Serial.println("you took too long!!!");
  }

}

void read_dtmf_inputs()
{
  Serial.println("Hello, I'm in read_dtmf_inputs()!");
  Serial.println();
  uint8_t number_pressed;
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
    last_hash_time = millis();
    break;    
  }

  // put on end of queue
  fifo[fifoIn]=number_pressed;
  fifoIn=(fifoIn+1)%FIFO_LEN;
}


// code below here no longer needed, but kept for reference

void number_pulse_out(int number_pressed) 
{
  if (number_pressed==0x0C) {
    hang_up();
  }
  
  for (int i = 0; i < number_pressed; i++)
  {
    digitalWrite(output_pin, HIGH);
    delay(pulse_length_make);
    digitalWrite(output_pin, LOW);
    delay(pulse_length_break);
    Serial.print("\nnumber_pressed=");
    Serial.print(number_pressed);
    Serial.print("\ni=");
    Serial.print(i);
    Serial.println();
  }
  delay(interdigit_gap);
}


void hang_up()
{
  digitalWrite(output_pin, HIGH);
  delay(pulse_hangup_delay);
  digitalWrite(output_pin, LOW);
  delay(pulse_hangup_delay);
  delay(pulse_hangup_delay);
  digitalWrite(output_pin, HIGH);
  return;
}
