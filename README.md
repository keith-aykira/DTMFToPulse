
# DTMFToPulse    [![Badge License]][License]

***DTMF Logic   🠖   Pulses***

<br>
<br>
<br>

<div align = center>

[![Button LCD]][LCD]   
[![Button State]][State]   
[![Button Async]][Async]   
[![Button Improvements]][Improvements]

<br>
<br>

The conversion is done using **State Machines** & **Countdown Timers**.

</div>

<br>
<br>

## Limitation

The `delay()` function cannot be used as the <br>
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

[Improvements]: Documentation/Improvements.md
[License]: #
[State]: Documentation/State%20Machines.md
[Async]: Documentation/Async%20Problem.md
[LCD]: Documentation/LCD.md


<!----------------------------------[ Badges ]--------------------------------->

[Badge License]: https://img.shields.io/badge/License-Unknown-808080.svg?style=for-the-badge


<!---------------------------------[ Buttons ]--------------------------------->

[Button Improvements]: https://img.shields.io/badge/Improvements-428813?style=for-the-badge&logoColor=white&logo=AddThis
[Button State]: https://img.shields.io/badge/State_Machines-006272?style=for-the-badge&logoColor=white&logo=GraphQL
[Button Async]: https://img.shields.io/badge/Async_Problems-FF4747?style=for-the-badge&logoColor=white&logo=Hackaday
[Button LCD]: https://img.shields.io/badge/LCD-2490D7?style=for-the-badge&logoColor=white&logo=Microsoft
