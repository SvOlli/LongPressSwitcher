
// Generic hold line two way switch
// by SvOlli

#define NUM_GROUPS   (4)
#define NUM_OUTS     (3)
#define MAX_TIME     (0xFFFFFFFF)
#define USE_EEPROM   (1)

#if USE_EEPROM
#include <EEPROM.h>
#endif


const uint32_t HoldTime = 1500;  // how long has a press to be in ms


typedef struct {
   uint32_t time_pressed;        // time stamp of press event
   uint8_t in_pin;               // pin to read
   uint8_t out_pins[NUM_OUTS];   // pins to write
   bool last_state;              // the last state of being pressed
   bool wait_release;            // the action has been invoked, waiting for release
} pin_group_t;


pin_group_t pin_group[NUM_GROUPS] = {
   { MAX_TIME, A3, {A2, A1, A0}, false, false },
   { MAX_TIME, 10, {11, 12, 13}, false, false },
   { MAX_TIME,  9, { 8,  7,  6}, false, false },
   { MAX_TIME,  2, { 3,  4,  5}, false, false }
};
// to define less than max outputs, use same output pins a more than one time


void setup()
{
   for( uint8_t g = 0; g < NUM_GROUPS; ++g )
   {
      digitalWrite( pin_group[g].in_pin, HIGH );
      pinMode( pin_group[g].in_pin, INPUT_PULLUP );
#if USE_EEPROM
      pin_group[g].last_state = EEPROM.read( g ) > 0;
#endif
      for( uint8_t o = 0; o < NUM_OUTS; ++o )
      {
         pinMode( pin_group[g].out_pins[o], OUTPUT );
         digitalWrite( pin_group[g].out_pins[o], pin_group[g].last_state ? HIGH : LOW );
      }
   }
   // workaround: pull reset after startup
   pinMode( A3, OUTPUT );
   digitalWrite( A3, LOW );
   delay(50);
   digitalWrite( A3, HIGH );
   pinMode( A3, INPUT_PULLUP );
}


void loop()
{
   for( int g = 0; g < NUM_GROUPS; ++g )
   {
      if( digitalRead( pin_group[g].in_pin ) )
      {
         // signal is high: not pressed
         // reset state values
         pin_group[g].time_pressed = MAX_TIME;
         pin_group[g].wait_release = false;
      }
      else
      {
         // signal is low: pressed
         
         // when press is new, note timestamp
         if( pin_group[g].time_pressed == MAX_TIME )
         {
            // it was not pressed before
            pin_group[g].time_pressed = millis();
         }
         
         // make sure the following code only runs once per press
         if( !(pin_group[g].wait_release) )
         {
            if( (millis() - pin_group[g].time_pressed) > HoldTime )
            {
               // press was long enough, toggle output pins
               bool state = !(pin_group[g].last_state);
               for( uint8_t o = 0; o < NUM_OUTS; ++o )
               {
                  digitalWrite( pin_group[g].out_pins[o], state ? HIGH : LOW );
               }
               pin_group[g].last_state = state;
#if USE_EEPROM
               EEPROM.write( g, state ? 1 : 0 );
#endif
               pin_group[g].wait_release = true;
            }
         }
      }
   }
}
