 Success Stories
=================

Commodore 128D (plastic)
------------------------
This machine has an additional internal SD2IEC as a harddisk replacement.
Two different kernals: original and [JiffyDOS](http://jiffydos.com) /
[JaffyDOS](http://blog.worldofjani.com/?p=3544). Here, the option to connect
three different output lines come into effect:
 - C128 kernal
 - C64 mode kernal
 - 1571 firmware

The configuration used looks like this:
```
pin_group_t pin_group[NUM_GROUPS] = {
   // intern   time     mode     in_pin  out_pins    line    intern
   // long press on reset changes kernel CBM <-> JiffyDOS
   { MAX_TIME, 1500, MODE_TOGGLE,  A3, {A2, A1, A0}, true, STATE_RESET },
   // long press triggers "next image" on internal SD2IEC
   { MAX_TIME, 1500, MODE_TRIGGER  10, {11, 12, 13}, true, STATE_RESET }
};
```
Also for the SD2IEC the "next image" function was mapped to a long press on
the restore key. The SD2IEC uses a custom setup and configuration described
here: [https://github.com/SvOlli/SD2IEC-LCD/tree/c128d-internal](https://github.com/SvOlli/SD2IEC-LCD/tree/c128d-internal)


Commodore 16
------------
Simpler than the C=128D approach, and also working like a charm. The
configuration used is the same as for the C=128D, only that the SD2IEC
line is unused.


Amiga 2000 / CDTV
-----------------
You can get the reset signal from pin 5 of the 5719 Gary chip. This is not
compatible with a long press. Only "double press" will work here.

The configuration used looks like this:
```
pin_group_t pin_group[NUM_GROUPS] = {
   // intern   time     mode     in_pin  out_pins    line    intern
   // double press on reset changes kickstart 1.3 <-> 3.1
   { MAX_TIME, -500, MODE_TOGGLE,  A3, {A2, A1, -13}, true, STATE_RESET }
};
```
The pin A2 is intended to go to the EPROM, A1 and 13 can be used with LEDs
as indicators, that's why pin 13 is inverted, so only one LED is active at
the same time. Pin 13 is used instead of A0 because it's also connected to
the on-board LED.

On The CDTV the reset button on the device works also, if the second press
is not done too soon after the first one.


Amiga 500
---------
This works like the Amiga 2000. It should be also possible to try pin 3 of
the keyboard connector, there the long press should be working, but that is
as yet untested.

