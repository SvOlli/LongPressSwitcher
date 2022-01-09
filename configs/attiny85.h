// this setup is a generic setup for a attiny85
// should also work on attiny45 and attiny25
/* Pinout according to https://github.com/SpenceKonde/ATTinyCore
 *    _ _
 *   5 v VCC
 *   3   2
 *   4   1
 * GND___0
 */

#define NUM_OUTS (2)
   // intern   time     mode     in_pin  out_pins    line    intern
#define PIN_SETUP \
   { MAX_TIME, 1500, MODE_TOGGLE,   0, { 1,  2}, true,  STATE_RESET }, \
   { MAX_TIME, 1500, MODE_TRIGGER,  5, { 3,  4}, true,  STATE_RESET }
