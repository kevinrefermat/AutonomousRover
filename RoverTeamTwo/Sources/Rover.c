#include "Rover.h"

/*** Constants ***/

const Byte False = 0;
const Byte True = 1;

const LWord CLOCK_SPEED_KHZ = 2000;
const Word SPEED_OF_SOUND_INCH_PER_SEC = 13506;
const timerCount_t CLOCK_CYCLES_PER_INCH = 148;
const Byte INCHES_PER_FOOT = 12;

const Byte TIMER_COUNTER_PRESCALE = 32;

const Byte PING_FREQUENCY = 3;


// At 100000 with no load at all it works. With load it must be higher
const microseconds_t WAIT_FOR_ROVER_TO_ACTUALLY_STOP_DELAY = 200000;

/*** GLOBAL VARIABLE ***/

extern degree_t pingAngle = -75;


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
   // enable timer and disable fast flag clear
   TSCR1 = 0x80;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 32
   TSCR2 = 0x05;                            
   
   // disable interrupt caused by channel 0 for measuring PING echo
   TIE &= 0xFE;
   
   /*********************************** UN DISABLE CH1 INTERRUPT ******
   // enable interrupt caused by channel 1 for periodic PING check
   TIE_C1I = 1;
   ********************************************************************/
   TIE_C1I = 0;

   
   // set ioc0 to input capture
   TIOS &= 0xFE;
   // set ic1 to output compare
   TIOS_IOS1 = 1;
   
   // pull up or down device enabled
   PERT |= 0x01;
   PPST |= 0x01;
   
   // disconnect timer from pin oc1
   TCTL2 &= 0xF3;

}