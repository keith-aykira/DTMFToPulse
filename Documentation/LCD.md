
# LCD

*The code supports a `I2C 16x2 LCD`.*

<br>

## Wiring

The wiring instructions can be <br>
found at the top of the code file.

<br>
<br>

## Interface

The display is split into four areas:

<br>

| Area | Usage
|:----:|:-----
| **[◰]** | Used for status messages.
| **[◳]** | Shows caller count.
| **[◲]** | Displays the status of the state machine.
| **[◱]** | Shows what was dialed and indicates <br> \- with a pointer - where the pulsing <br> process is current at.

<br>
<br>

## Backlight

Will turn on for `60 seconds` after the last change, <br>
to make it easy to see activity from a distance.

<br>



<!----------------------------------------------------------------------------->

[◲]: # 'Bottom Right Area'
[◱]: # 'Bottom Left Area'
[◳]: # 'Top Right Area'
[◰]: # 'Top Left Area'
