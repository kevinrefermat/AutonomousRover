#include "Rover.h"

/*** Constants ***/

const boolean_t False = 0;
const boolean_t True = 1;

const LWord CLOCK_SPEED_KHZ = 2000;
const Word SPEED_OF_SOUND_INCH_PER_SEC = 13506;
const timerCount_t CLOCK_CYCLES_PER_INCH = 148;
const Byte INCHES_PER_FOOT = 12;

const Byte TIMER_COUNTER_PRESCALE = 32;

// At 100000 with no load at all it works. With load it must be higher
const microseconds_t WAIT_FOR_ROVER_TO_ACTUALLY_STOP_DELAY = 200000;


/*** USEFUL FUNCTIONS ***/

// actual delay time is 12.5us (of overhead) and 10us for each
// iteration of the for loop          
void Delay( microseconds_t time )
{  
  microseconds_t i, iMax;
  iMax = time / 10;
  for ( i = 0; i < iMax; i++ )
  {
    _asm
    {
      nop
      nop
      nop
      nop
      nop 
    }
  }
}

void InitializeTimers()
{
   // enable timer and disable fast flag clear
   TSCR1 = 0x80;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 32
   TSCR2 = 0x05;                            
}
