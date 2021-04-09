
// rather simple programmable switch
// by SvOlli

#define USE_EEPROM         (1)
#define STARTUP_DONE_PIN   (A3)  // use -1 to turn feature off
#define STARTUP_DONE_VALUE (LOW) // trigger HIGH of LOW on startup?
#define TRIGGER_TIME       (50)  // ms

#define NUM_GROUPS         (4)
#define NUM_OUTS           (3)
#define MAX_TIME           (0xFFFFFFFF)

// misusing an uint8_t as enum
#define MODE_DISABLED      (0)
#define MODE_TOGGLE        (1)
#define MODE_TRIGGER       (2)

#if USE_EEPROM
#include <EEPROM.h>
#endif


typedef struct {
   uint32_t time_pressed;        // internal: time stamp of press event
   uint16_t hold_time;           // config:   time needed to press for action
   uint8_t mode;                 // config:   what to do when triggered
   uint8_t in_pin;               // config:   pin to read
   int8_t out_pins[NUM_OUTS];    // config:   pins to write
   bool last_state;              // config:   state of untriggered output (trigger)
                                 // config:   the state upon startup without EEPROM (toggle)
   bool wait_release;            // internal: the action has been invoked, waiting for release
} pin_group_t;


pin_group_t pin_group[NUM_GROUPS] = {
   // intern   time     mode     in_pin  out_pins    line   intern
   { MAX_TIME, 1500, MODE_TOGGLE,  A3, {A2, A1, A0}, true,  false },
   // for Arduino Pro Micro change     {11, 12, 13} to {14, 15, 16}
   { MAX_TIME, 1500, MODE_TRIGGER, 10, {11, 12, 13}, true,  false },
   { MAX_TIME, 1500, MODE_TOGGLE,   9, { 8,  7,  6}, true,  false },
   { MAX_TIME, 1500, MODE_TRIGGER,  2, { 3,  4,  5}, true,  false }
};
// to define less than max outputs, use same output pins a more than one time


void setup()
{
#if STARTUP_DONE_PIN >= 0
   uint8_t startup_done_pin_mode = 0xFF;
#endif
   for( uint8_t g = 0; g < NUM_GROUPS; ++g )
   {
      digitalWrite( pin_group[g].in_pin, HIGH );
      pinMode( pin_group[g].in_pin, INPUT_PULLUP );
#if STARTUP_DONE_PIN >= 0
      if( STARTUP_DONE_PIN == pin_group[g].in_pin )
      {
         startup_done_pin_mode = INPUT_PULLUP;
      }
#endif
#if USE_EEPROM
      if( pin_group[g].mode == MODE_TOGGLE )
      {
         pin_group[g].last_state = EEPROM.read( g ) > 0;
      }
#endif
      for( uint8_t o = 0; o < NUM_OUTS; ++o )
      {
         pinMode( pin_group[g].out_pins[o], OUTPUT );
         digitalWrite( pin_group[g].out_pins[o], pin_group[g].last_state ? HIGH : LOW );
#if STARTUP_DONE_PIN >= 0
         if( STARTUP_DONE_PIN == pin_group[g].out_pins[o] )
         {
            startup_done_pin_mode = OUTPUT;
         }
#endif
      }
   }
#if STARTUP_DONE_PIN >= 0
   // workaround: pull reset after startup
   pinMode( STARTUP_DONE_PIN, OUTPUT );
   digitalWrite( STARTUP_DONE_PIN, STARTUP_DONE_VALUE );
   delay( TRIGGER_TIME );
   digitalWrite( STARTUP_DONE_PIN, !STARTUP_DONE_VALUE );
   if( startup_done_pin_mode < 0xFF )
   {
      pinMode( STARTUP_DONE_PIN, startup_done_pin_mode );
   }
#endif
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
            if( millis() > (pin_group[g].time_pressed + pin_group[g].hold_time) )
            {
               // press was long enough, toggle output pins
               bool state = !(pin_group[g].last_state);
               switch( pin_group[g].mode )
               {
               case MODE_TOGGLE:
                  for( uint8_t o = 0; o < NUM_OUTS; ++o )
                  {
                     if( pin_group[g].out_pins[o] < 0 )
                     {
                        digitalWrite( -pin_group[g].out_pins[o], state ? LOW : HIGH );
                     }
                     else
                     {
                        digitalWrite( pin_group[g].out_pins[o], state ? HIGH : LOW );
                     }
                  }
                  pin_group[g].last_state = state;
#if USE_EEPROM
                  EEPROM.write( g, state ? 1 : 0 );
#endif
                  break;
               case MODE_TRIGGER:
                  for( uint8_t o = 0; o < NUM_OUTS; ++o )
                  {
                     if( pin_group[g].out_pins[o] < 0 )
                     {
                        digitalWrite( -pin_group[g].out_pins[o], state ? LOW : HIGH );
                     }
                     else
                     {
                        digitalWrite( pin_group[g].out_pins[o], state ? HIGH : LOW );
                     }
                  }
                  delay( TRIGGER_TIME );
                  for( uint8_t o = 0; o < NUM_OUTS; ++o )
                  {
                     if( pin_group[g].out_pins[o] < 0 )
                     {
                        digitalWrite( -pin_group[g].out_pins[o], state ? HIGH : LOW );
                     }
                     else
                     {
                        // note: double reverse logic
                        digitalWrite( pin_group[g].out_pins[o], state ? LOW : HIGH );
                     }
                  }
                  break;
               default:
                  /* don't do anything on misconfiguration */
                  break;
               }
               pin_group[g].wait_release = true;
            }
         }
      }
   }
}
