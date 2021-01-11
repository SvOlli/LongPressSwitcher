 LongPressSwitcher
===================

A simple Arduino project that intended to be used as a "hidden switch"
for retro projects. For example pressing the reset button for more than
one and a half seconds, the content of the kernal rom is being switched.

The code is written for an Arduino Pro Mini with an ATmega168
microcontroller. It was the cheapest I could find.

The GPIOs are set in four groups, each containing of one input and three
outputs. If the input is low for more than one second, then the outputs
will toggled. The starting value of all outputs upon power on is low.
The groups are:

| input    | output 1 | output 2 | output 3 |
| :------: | :------: | :------: | :------: |
| 2        | 3        | 4        | 5        |
| 9        | 8        | 7        | 6        |
| 10       | 11       | 12       | 13       |
| A3       | A2       | A1       | A0       |

(Keep in mind that the pin 13 is also connected to the LED, so pin 10
also toggles the LED.)

The idea is taken from the [kernal switcher introduced by Adrians
Digital Basement](https://www.youtube.com/watch?v=GPq5xnJRw2w). The main
changes are that switching will only happen between two states. Also,
four inputs instead of one are supported, so you can toggle something
on reset (like the kernal) and the restore key (like the SID) with the
same microcontroller. 
