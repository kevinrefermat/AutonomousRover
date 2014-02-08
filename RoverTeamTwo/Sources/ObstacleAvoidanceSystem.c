#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"

void initializePeriodicObjectDetection( milliseconds_t period )
{
  
} 

inches_t detectClosestObstacle()
{
   timerCount_t lengthOfEcho;
   LWord distance;
   
   DisableInterrupts;
   
   outputPulseToPing();
   lengthOfEcho = measureReturnPulseFromPing();
   distance = ( LWord ) lengthOfEcho * 13506;
   distance = distance / CLOCK_SPEED_HZ;
   distance = distance / 2;
   
   EnableInterrupts;
   return ( inches_t ) distance;
}

static void outputPulseToPing()
{
// creates a 5us pulse
    OBJECT_DETECTION_DDR = 1;

    OBJECT_DETECTION_PIN = 0;
    OBJECT_DETECTION_PIN = 1;
    _asm
    {
      nop
      nop
      nop
      nop
      nop
      nop
    }
    OBJECT_DETECTION_PIN = 0;
}

static timerCount_t measureReturnPulseFromPing()
{
   timerCount_t risingEdge;
   
   OBJECT_DETECTION_DDR = 0;


/*
// This section can be in a global initialization

   // enable timer and fast flag clear
   TSCR1 = 0x90;
   
   // disable timer overflow interrupt; ch0 = simple modulus counter; clock prescale = 1
   TSCR2 = 0x00;                            
   
   // disable interrupt caused by channel 0
   TIE &= 0xFE;
   
   // pull up or down device enabled
   PERT |= 0x01;
   
   PPST |= 0x01;


*/

   // enable channel 0 to be input capture
   TIOS &= 0xFE;
   
   
   // ************ MAKE THIS VVVVV SO THAT BITS 1 and 0 are affected but no others
   // capture rising edge
   TCTL4_EDG0x = 0x01;
   
   TFLG1 = 0x01;
   while ( !( TFLG1 & 0x01 ) ); 
   TFLG1 = 0x01;
   
   risingEdge = TC0;
   
   TCTL4_EDG0x = 0x02;
  
   while ( !( TFLG1 & 0x01 ) );
   
   if ( risingEdge < TC0 )
   {
      return TC0 - risingEdge;
   }
   else
   {
      return ( ~risingEdge + 1 ) + TC0;
   }
}