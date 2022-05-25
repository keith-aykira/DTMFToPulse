
# DTMFToPulse    [![Badge License]][License]

***DTMF Logic   🠖   Pulses***

<br>

<div align = center>

---

[<kbd> <br> LCD <br> </kbd>][LCD]   
[<kbd> <br> State <br> Machines <br> </kbd>][State Machines]   
[<kbd> <br> Mutliple <br> Async Problem <br> </kbd>][Async Problem]   
[<kbd> <br> Improvements <br> </kbd>][Improvements]

---

<br>
<br>

The conversion is done using **State Machines** & **Countdown Timers**.

</div>

<br>
<br>

## Limitation

The `delay()` function cannot be used as the <br>
digital **DTMF** input can occur at any time and <br>
it's signal changes are faster than any output. <br>

Thus the delay function cannot even be used <br>
for pulse generation, since this could mean <br>
dropping pulses of the **DTMF** tone.

<br>
<br>

## Configuration

At the start of the source file you can find <br>
preprocessor definitions that allow you to:

- Control the behavior when hangups occur

- Adjust how quickly dials are pulsed

- Set whether to stay LOW / HIGH when idle

<br>

*In the section below the previous you* <br>
*will find the various timing presets.*

<br>


<!----------------------------------------------------------------------------->

[State Machines]: Documentation/State%20Machines.md
[Async Problem]: Documentation/Async%20Problem.md
[Improvements]: Documentation/Improvements.md
[LCD]: Documentation/LCD.md

[Badge License]: https://img.shields.io/badge/License-Unknown-808080.svg?style=for-the-badge
[License]: #
