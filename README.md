
# DTMFToPulse    [![Badge License]][License]

***DTMF Logic   🠖   Pulses***

<br>

<div align = center>

---

[![Button LCD]][LCD]  
[![Button State Machines]][State Machines]  
[![Button Async Problem]][Async Problem]  
[![Button Improvements]][Improvements]

---

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

[State Machines]: Documentation/State%20Machineies.md
[Async Problem]: Documentation/Async%20Problem.md
[Improvements]: Documentation/Improvements.md
[LCD]: Documentation/LCD.md

[Badge License]: https://img.shields.io/badge/License-Unknown-808080.svg?style=for-the-badge
[License]: #

<!---------------------------------{ Buttons }--------------------------------->

[Button State Machines]: https://img.shields.io/badge/State_Machines-c9c307?style=for-the-badge&logoColor=white&logo=MaterialDesignIcons
[Button Async Problem]: https://img.shields.io/badge/Multiple_Async_Problem-884cc9?style=for-the-badge&logoColor=white&logo=ChakraUI
[Button Improvements]: https://img.shields.io/badge/Improvements-33c56e?style=for-the-badge&logoColor=white&logo=AddThis
[Button LCD]: https://img.shields.io/badge/LCD-73ade7?style=for-the-badge&logoColor=white&logo=GNOMETerminal