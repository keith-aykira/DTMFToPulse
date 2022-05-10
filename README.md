# DTMFToPulse
Converts DTMF logic to pulses. Does so using a state machine and countdown timers. delay() cannot be used at all in this case as the digital DTMF input could occur at any time and the speed of the DTMF changes is faster than output pulses, so using delays() in the pulse generation won't work (any more than one DTMF tone transition during this pulsing will be dropped).
## What is a state machine?
A state machine is a piece of code which moves between a number of states, a bit like a piece on a Snakes & ladders board. Each state runs a little bit of code, which then can do something and then 'jump' to another state. You often find these used to reliably progress some form of processing, be that for some inputs or to generate some outputs, or do both at the same time (yes you can have multiple pieces/states in play at once, but you really need to know what you are doing). Quite often what was a horrid nest of IF THEN ELSE statements with random timers in it converts into a more readily readable state machine.
## the multiple async problem
When you have to deal with different things occuring that are interrelated to each other, in this case DTMF codes driving a slower pulse train, you have be very careful you don't create hidden side effects. This typically is seen when you have to resort to controlling interrupts on mass to ensure ordering - something will often drop between the cracks. I like to keep things simple and use interrupt action flags and a state machine so you know whats going on.
