// this setup is a generic setup for a attiny84
// should also work on attiny44 and attiny24
/* Pinout according to https://github.com/SpenceKonde/ATTinyCore
 *    _ _
 * VCC v GND
 *  10   0
 *   9   1
 *  11   2
 *   8   3
 *   7   4
 *   6___5
 */

#define NUM_OUTS (2)
   // intern   time     mode     in_pin  out_pins    line    intern
#define PIN_SETUP \
   { MAX_TIME, 1500, MODE_TOGGLE,   0, { 1,  2}, true,  STATE_RESET }, \
   { MAX_TIME, 1500, MODE_TRIGGER,  5, { 3,  4}, true,  STATE_RESET }, \
   { MAX_TIME, 1500, MODE_TOGGLE,   6, { 7,  8}, true,  STATE_RESET }, \
   { MAX_TIME, 1500, MODE_TRIGGER, 10, { 9, 11}, true,  STATE_RESET }
