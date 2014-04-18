#include "Rover.h"

/*** USEFUL FUNCTIONS ***/

void Delay( milliseconds_t time )
{  
  milliseconds_t i, j;
  
  for ( i = 0; i < time; i++ )
  {
     // 1 millisecond loop
     for ( j = 0; j < 284; j++ )
     {
       _asm
       {
         nop 
       }
     }
   }  
}

void InitializeTimers()
{
   Byte offset, timerCounterPrescale;
   offset = 0;

   // enable timer and disable fast flag clear
   TSCR1 = 0x80;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 1 and is last 3 bits
   TSCR2 = 0x00;
                               
   timerCounterPrescale = TIMER_COUNTER_PRESCALE;
   
   switch ( timerCounterPrescale )
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
      default:
         TurnOnErrorLight();
         break;
   }
   TSCR2 += offset;
}

void TurnOnErrorLight()
{
   DDRB_BIT7 = 1;
   PORTB_BIT7 = 0;
}

void TurnOffErrorLight()
{
   DDRB_BIT7 = 1;
   PORTB_BIT7 = 1;
}
