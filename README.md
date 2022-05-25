# DTMFToPulse
Converts DTMF logic to pulses. Does so using a state machine and countdown timers. delay() cannot be used at all in this case as the digital DTMF input could occur at any time and the speed of the DTMF changes is faster than output pulses, so using delays() in the pulse generation won't work (any more than one DTMF tone transition during this pulsing will be dropped).



# Configs
At the top of the file is a set of hash defines that control the behaviour when hang ups occur, how quickly it tries to Pulse dial and whether it holds HIGH or LOW when idle.

Below the configs is a section controlling the various timing presets.



