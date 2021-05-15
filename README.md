 LongPressSwitcher
===================

A simple Arduino project that intended to be used as a "hidden switch"
for retro projects. For example pressing the reset button for more than
one and a half seconds, the content of the kernal rom is being switched.

The code is written for an Arduino Pro Mini with an ATmega168
microcontroller. It was the cheapest I could find.

The configuration is done at the start of the code. There are some
common parameters and a small array. Basically you have a number of
"groups" containing an input pin and a fixed number of output pins. The
number of groups and the number of output pins per group are defined at
the beginning of the code (NUM_GROUPS and NUM_OUTS). The checked in
configuration is four groups with three output pins per group, so let's
stick to this configuration.

This is what the configuration looks like:
```
pin_group_t pin_group[NUM_GROUPS] = {
   // intern   time     mode     in_pin  out_pins    line    intern
   { MAX_TIME, 1500, MODE_TOGGLE,  A3, {A2, A1, A0}, true, STATE_RESET },
   { MAX_TIME, 1500, MODE_TRIGGER  10, {11, 12, 13}, true, STATE_RESET },
   { MAX_TIME, 1500, MODE_TOGGLE,   9, { 8,  7,  6}, true, STATE_RESET },
   { MAX_TIME, 1500, MODE_TRIGGER,  2, { 3,  4,  5}, true, STATE_RESET }
};
```
Let's walk through the parameters:
* don't touch the first one noted "intern" with the value of "MAX_TIME"
* the second defines the time on how long you have to hold an input to
  trigger the action; if the value is negative, then the time is
  measured between the release and the next trigger (think double click)
* mode defines the mode of operation: it can toggle from high to low
  when you trigger it, and change back from low to high the next time;
  it can also just trigger a change of the state for a short period of
  time and then change it back.
* the input pin defines the pin is waited on for a change, input is
  always expected to change from high to low
* the output pins are all changed together, so several functions can be
  tied together; if the output pin is a negative number (like -9 or -A3)
  the output of that single pin is reversed
* the line parameter states the default upon startup; for toggle mode
  the last value can be read from the EEPROM, if configured
* the final parameter is again used only internally

The time of the trigger event is specified in TRIGGER_TIME. There also
is an option to trigger an event upon startup.

The idea is based upon the [kernal switcher introduced by Adrians
Digital Basement](https://www.youtube.com/watch?v=GPq5xnJRw2w). The main
changes are that switching will only happen between two states. Also,
four inputs instead of one are supported, so you can toggle something
on reset (like the kernal) and the restore key (like the SID) with the
same microcontroller. 

Flashing
--------
Here is what's working on my system. If you use this without verifying,
you will most likely get and error message, something that will not work,
or might even break the ATmega in the process.
```
avrdude -pm168p -cusbasp \
   -U flash:w:tmp/LongPressSwitcher.hex:i \
   -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
```

Implementation Examples
-----------------------
Those and some information about the details are available in the
document [SuccessStories.md](SuccessStories.md).
