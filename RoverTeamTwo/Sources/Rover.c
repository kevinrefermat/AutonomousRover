#include "Rover.h"

/*** Constants ***/

const Byte False = 0;
const Byte True = 1;

const LWord CLOCK_SPEED_HZ = 2000000;
const Word SPEED_OF_SOUND_INCH_PER_SEC = 13506;
const timerCount_t CLOCK_CYCLES_PER_INCH = CLOCK_SPEED_HZ / SPEED_OF_SOUND_INCH_PER_SEC;

// At 100000 with no load at all it works. With load it must be higher
const microseconds_t WAIT_FOR_ROVER_TO_ACTUALLY_STOP_DELAY = 200000;


/*** Flags ***/ 

boolean_t RoverInMotionFlag = False;


/*** USEFUL FUNCTIONS ***/

// actual delay time is 12.5us (of overhead) and 10us for each
// iteration of the for loop          
void delay( microseconds_t time )
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

void initializeTimers()
{
//********************************
// This section can be in a global initialization

   // enable timer and disable fast flag clear
   TSCR1 = 0x80;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 1
   TSCR2 = 0x00;                            
   
   // disable interrupt caused by channel 0
   TIE &= 0xFE;
   
   // pull up or down device enabled
   PERT |= 0x01;
   
   PPST |= 0x01;


//********************************
   
}
