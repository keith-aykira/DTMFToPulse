# DTMFToPulse
Converts DTMF logic to pulses. Does so using a state machine and countdown timers. delay() cannot be used at all in this case as the digital DTMF input could occur at any time and the speed of the DTMF changes is faster than output pulses, so using delays() in the pulse generation won't work (any more than one DTMF tone transition during this pulsing will be dropped).
## What is a state machine?
A state machine is a piece of code which moves between a number of states, a bit like a piece on a Snakes & ladders board. Each state runs a little bit of code, which then can do something and then 'jump' to another state. You often find these used to reliably progress some form of processing, be that for some inputs or to generate some outputs, or do both at the same time (yes you can have multiple pieces/states in play at once, but you really need to know what you are doing). Quite often what was a horrid nest of IF THEN ELSE statements with random timers in it converts into a more readily readable state machine.
## the multiple async problem
When you have to deal with different things occuring that are interrelated to each other, in this case DTMF codes driving a slower pulse train, you have be very careful you don't create hidden side effects. This typically is seen when you have to resort to controlling interrupts on mass to ensure ordering - something will often drop between the cracks. I like to keep things simple and use interrupt action flags and a state machine so you know whats going on.


#Configs
At the top of the file is a set of hash defines that control the behaviour when hang ups occur, how quickly it tries to Pulse dial and whether it holds HIGH or LOW when idle.

Below the configs is a section controlling the various timing presets.

#LCD Display
The code will support a i2c 16x2 LCD display, how to wire it up is at the top of the code. The display is split into four areas: top left quadrant is for status messages, top right is for a count of callers so far, bottom left is what was dialled with a little ^ pointer showing you where the Pulsing has got to and bottom right shows the current state machine state.  The backlight will come on for 60 seconds after the last change, so makes it very easy to see activity from a distance.

#Improvements..
- a prefix insert switch - so if Sam wants to divert everyone to a single extension, he can just have a switch to pull low an input, which should be safe to wire as a group across the set (assuming the same potential ground). This could be used when doing maintenance or making announcements etc.
- hard time out. this would enforce a maximum session time after which it won't do number pulses anymore - any way to chuck someone off the incoming line? hmm...
