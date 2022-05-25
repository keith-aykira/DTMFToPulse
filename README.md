
# DTMFToPulse

***DTMF Logic   🠖   Pulses***

<br>

<div align = center>

---

[![Button LCD]][LCD]  
[![Button State Machines]][State Machines]  
[![Button Async Problem]][Async Problem]  
[![Button Improvements]][Improvements]

---

</div>

<br>

Does so using a state machine and countdown timers. delay() cannot be used at all in this case as the digital DTMF input could occur at any time and the speed of the DTMF changes is faster than output pulses, so using delays() in the pulse generation won't work (any more than one DTMF tone transition during this pulsing will be dropped).



# Configs
At the top of the file is a set of hash defines that control the behaviour when hang ups occur, how quickly it tries to Pulse dial and whether it holds HIGH or LOW when idle.

Below the configs is a section controlling the various timing presets.



<!----------------------------------------------------------------------------->

[State Machines]: Documentation/State%20Machineies.md
[Async Problem]: Documentation/Async%20Problem.md
[Improvements]: Documentation/Improvements.md
[LCD]: Documentation/LCD.md


<!---------------------------------{ Buttons }--------------------------------->

[Button State Machines]: https://img.shields.io/badge/State_Machines-c9c307?style=for-the-badge&logoColor=white&logo=MaterialDesignIcons
[Button Async Problem]: https://img.shields.io/badge/Multiple_Async_Problem-884cc9?style=for-the-badge&logoColor=white&logo=ChakraUI
[Button Improvements]: https://img.shields.io/badge/Improvements-33c56e?style=for-the-badge&logoColor=white&logo=AddThis
[Button LCD]: https://img.shields.io/badge/LCD-73ade7?style=for-the-badge&logoColor=white&logo=GNOMETerminal