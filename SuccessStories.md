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


Amiga 2000
----------
You can get the reset signal from pin 5 of the 5719 Gary chip. The reset
signal is as least 1.6 seconds long and will be expanded if the keys are held
longer, so a time configuration of 2 seconds is more useful here. Also note
that I configured the third output as -13, so the internal LED can be used
to monitor the state as well. It is also negative, so pins A1 and 13 can be
used to drive a dual color LED, e.g. as a replacement for the power LED.
```
pin_group_t pin_group[NUM_GROUPS] = {
   // intern   time     mode     in_pin  out_pins    line    intern
   // long press on reset changes kickstart 1.3 <-> 3.1
   { MAX_TIME, 2000, MODE_TOGGLE,  A3, {A2, A1, -13}, true, STATE_RESET }
```

Amiga CDTV
----------
As with the Amiga 2000, you can get the reset signal from pin 5 of the
5719 Gary chip. But this time it is not compatible with a long press, as
the reset signal is will always be less than a second. Only "double press"
will work here.

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
This setup is as yet untested. This works like the Amiga 2000. It should be
also possible to try pin 3 of the keyboard connector.

