
// rather simple programmable switch
// by SvOlli

#define USE_EEPROM         (1)
#define STARTUP_DONE_PIN   (A3)  // use -1 to turn feature off
#define STARTUP_DONE_VALUE (LOW) // trigger HIGH or LOW on startup?
#define TRIGGER_TIME       (50)  // ms
#define DEBOUNCE_DELAY     (30)  // ms

#define NUM_OUTS           (3)
#define MAX_TIME           (0xFFFFFFFF)

#if USE_EEPROM
#include <EEPROM.h>
#endif

typedef enum : uint8_t
{
   MODE_DISABLED = 0,
   MODE_TOGGLE,
   MODE_TRIGGER,
}
my_mode_t;

typedef enum : uint8_t
{
   STATE_RESET = 0,
   STATE_WAIT_RELEASE = 1,
   STATE_FIRST_PRESS = 2,
   STATE_FIRST_RELEASE = 3
}
my_fsm_t;

typedef struct {
   uint32_t start_millis;        // internal: time stamp of press event
   int16_t hold_time;            // config:   time needed to press for action
   my_mode_t mode;               // config:   what to do when triggered
   int8_t in_pin;                // config:   pin to read
   int8_t out_pins[NUM_OUTS];    // config:   pins to write
   bool last_state;              // config:   state of untriggered output (trigger)
                                 // config:   the state upon startup without EEPROM (toggle)
   my_fsm_t fsm_state;           // internal: the action has been invoked, waiting for release
} pin_group_t;


pin_group_t pin_group[] = {
   // intern   time  output mode  in_pin  out_pins   line   internal state
   { MAX_TIME, 1500, MODE_TOGGLE,  A3, {A2, A1, A0}, true,  STATE_RESET },
   // for Arduino Pro Micro change     {11, 12, 13} to {14, 15, 16}
   { MAX_TIME, 1500, MODE_TRIGGER, 10, {11, 12, 13}, true,  STATE_RESET },
   { MAX_TIME, 1500, MODE_TOGGLE,   9, { 8,  7,  6}, true,  STATE_RESET },
   { MAX_TIME, 1500, MODE_TRIGGER,  2, { 3,  4,  5}, true,  STATE_RESET }
};
// to define less than max outputs, use same output pins a more than one time

const uint8_t NUM_GROUPS = (sizeof(pin_group) / sizeof(pin_group[0]));


void setup()
{
#if STARTUP_DONE_PIN >= 0
   uint8_t startup_done_pin_mode = 0xFF;
   uint8_t startup_done_output_cache = 0xFF;
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
         if( pin_group[g].out_pins[o] < 0 )
         {
            digitalWrite( -pin_group[g].out_pins[o], pin_group[g].last_state ? LOW : HIGH );
         }
         else
         {
            digitalWrite( pin_group[g].out_pins[o], pin_group[g].last_state ? HIGH : LOW );
         }
#if STARTUP_DONE_PIN >= 0
         if( STARTUP_DONE_PIN == pin_group[g].out_pins[o] )
         {
            startup_done_pin_mode = OUTPUT;
            startup_done_output_cache = digitalRead( STARTUP_DONE_PIN );
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
   // if startup pin mode has been saved, it needs to be applied again
   if( startup_done_pin_mode < 0xFF )
   {
      pinMode( STARTUP_DONE_PIN, startup_done_pin_mode );
      if( startup_done_pin_mode == OUTPUT )
      {
         delay( TRIGGER_TIME );
         digitalWrite( STARTUP_DONE_PIN, startup_done_output_cache );
      }
   }
#endif
}


void run_output( uint8_t g )
{
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
      // don't do anything on misconfiguration
      break;
   }
}

void loop()
{
   for( int g = 0; g < NUM_GROUPS; ++g )
   {
      // check if running in long press mode
      if( pin_group[g].hold_time > 0 )
      {
         if( digitalRead( pin_group[g].in_pin ) )
         {
            // signal is high: not pressed
            // reset state values
            pin_group[g].start_millis = MAX_TIME;
            pin_group[g].fsm_state    = STATE_RESET;
         }
         else
         {
            // signal is low: pressed

            // when press is new, note timestamp
            if( pin_group[g].start_millis == MAX_TIME )
            {
               // it was not pressed before
               pin_group[g].start_millis = millis();
            }

            // make sure the following code only runs once per press
            if( pin_group[g].fsm_state == STATE_RESET )
            {
               if( millis() > (pin_group[g].start_millis + pin_group[g].hold_time) )
               {
                  // press was long enough, do something...
                  run_output( g );
                  pin_group[g].fsm_state = STATE_WAIT_RELEASE;
               }
            }
         }
      }
      // check if running in long press mode
      else if( pin_group[g].hold_time < 0 )
      {
         bool pressed = !digitalRead( pin_group[g].in_pin );
         switch( pin_group[g].fsm_state )
         {
         case STATE_RESET: // 0 <-- state nr for checking debug output
            pin_group[g].start_millis = MAX_TIME;
            if( pressed )
            {
               // debounce: ugly, but surpizingly working
               delay( DEBOUNCE_DELAY );
               // pressed for first time, waiting for release
               pin_group[g].fsm_state = STATE_FIRST_PRESS;
            }
            break;
         case STATE_FIRST_PRESS: // 2
            if( !pressed )
            {
               // debounce: ugly, but surpizingly working
               delay( DEBOUNCE_DELAY );
               // double press time starts with release
               pin_group[g].start_millis = millis();
               pin_group[g].fsm_state = STATE_FIRST_RELEASE;
            }
            break;
         case STATE_FIRST_RELEASE: // 3
            if( pressed )
            {
               // pressed again fast enough: do something and wait for release
               run_output( g );
               pin_group[g].fsm_state = STATE_WAIT_RELEASE;
            }
            else if( millis() > pin_group[g].start_millis - pin_group[g].hold_time )
            {
               // not pressed fast enough: fall back to first square
               pin_group[g].fsm_state = STATE_RESET;
            }
            break;
         case STATE_WAIT_RELEASE: // 1
            if( !pressed )
            {
               // debounce: ugly, but surpizingly working
               delay( DEBOUNCE_DELAY );
               // start over again
               pin_group[g].fsm_state = STATE_RESET;
            }
            break;
         default:
            // this should never happen
            break;
         }
      }
   }
}
