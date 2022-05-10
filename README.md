# DTMFToPulse
converts DTMF logic to pulses. Does so using a state machine and countdown timers. delay() cannot be used at all in this case as the digital DTMF input could occur at any time and the speed of the DTMF changes is faster than output pulses, so using delays() in the pulse generation won't work.
