#include "Rover.h"

/*** Constants ***/

const boolean_t False = 0;
const boolean_t True = 1;

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
   Byte offset = 0;

   // enable timer and disable fast flag clear
   TSCR1 = 0x80;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 1 and is last 3 bits
   TSCR2 = 0x00;                            
   
   // CODE ERROR CONDITION; POSSIBLY LEAVE ONE LED ON CHIP ENABLED FOR PORTB
   switch ( TIMER_COUNTER_PRESCALE )
   {
      case 1: 
         break;
      case 2:
         offset = 0x01;
         break;
      case 4:
         offset = 0x02;
         break;
      case 8:
         offset = 0x03;
         break;
      case 16:
         offset = 0x04;
         break;
      case 32:
         offset = 0x05;
         break;
      case 64:
         offset = 0x06;
         break;
      case 128: 
         offset = 0x07;
         break;
   }
   TSCR2 += offset;
}
