// this setup is used for my Commodore 128 D
// long press on reset (A3) changes kernel CBM <-> JiffyDOS
// long press on restore (10) triggers "next image" on the internal SD2IEC

#define NUM_OUTS (3)
   // intern   time     mode     in_pin  out_pins    line    intern
#define PIN_SETUP \
   { MAX_TIME, 1500, MODE_TOGGLE,  A3, {A2, A1, A0}, true, STATE_RESET }, \
   { MAX_TIME, 1500, MODE_TRIGGER, 10, {11, 12, 13}, true, STATE_RESET }
