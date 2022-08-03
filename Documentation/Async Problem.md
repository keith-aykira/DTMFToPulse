
# Multiple Async Problem

When you have to deal with different things <br>
occurring that are interrelated to each other, <br>
in this case **DTMF** tones driving slower pulse <br>
trains, you have be careful you don't create <br>
hidden side effects.


This typically is seen when controlling interrupts <br>
on mass - to ensure ordering - where something <br>
will often drop between the cracks.

I like to keep things simple and use interrupt <br>
action flags and a state machine so you know <br>
whats going on.

<br>
