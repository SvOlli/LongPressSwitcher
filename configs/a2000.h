// this setup is used for my Commodore Amiga 2000
// long press on reset (CTRL+Amiga+Amiga) changes kickstart 1.3 <-> 3.1

#define NUM_OUTS (3)
   // intern   time     mode     in_pin  out_pins    line    intern
#define PIN_SETUP \
   { MAX_TIME, 2000, MODE_TOGGLE,  A3, {A2, A1, -13}, true, STATE_RESET }
