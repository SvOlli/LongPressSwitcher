// this setup is used for my Commodore CDTV
// double press on reset changes kickstart 1.3 <-> 3.1
// also note that pin 13 is used instead of A0, because that on-board LED
// is connected to that pin and the LED is visible through the ventilation
// slots

#define NUM_OUTS (3)
   // intern   time     mode     in_pin  out_pins    line    intern
#define PIN_SETUP \
   { MAX_TIME, -500, MODE_TOGGLE,  A3, {A2, A1, -13}, true, STATE_RESET }
