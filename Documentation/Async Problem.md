## the multiple async problem
When you have to deal with different things occuring that are interrelated to each other, in this case DTMF codes driving a slower pulse train, you have be very careful you don't create hidden side effects. This typically is seen when you have to resort to controlling interrupts on mass to ensure ordering - something will often drop between the cracks. I like to keep things simple and use interrupt action flags and a state machine so you know whats going on.