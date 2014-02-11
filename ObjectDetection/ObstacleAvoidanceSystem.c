#include "Rover.h"

#include "ObstacleAvoidanceSystem.h"

static short outputPulseConstant = 10;

void initializePeriodicObjectDetection( milliseconds_t period );

inches_t detectClosestObstacle()
{
   DisableInterrupts;
   outputPulseToPing();
   measureReturnPulseFromPing();
   EnableInterrupts;
}

static void outputPulseToPing()
{
   // requires 2us minimum pulse with a typical pulse length of 5us
   short i;
   
   OBJECT_DETECTION_DDR = 1;

   OBJECT_DETECITON_PIN = 0;
   OBJECT_DETECITON_PIN = 1;
   for ( i = 0; i < outputPulseConstant; i++ );
   OBJECT_DETECITON_PIN = 0;
}

static clockCycle_t measureReturnPulseFromPing()
{
   clockCycle_t diff, edge1, pulseWidth;
   
   // not sure i need this
   OBJECT_DETECTION_DDR = 0;

   // enable channel 0 to be input capture
   TIOS &= 0xFE;

   // enable timer and fast flag clear
   TSCR = 0x90;

   // disable interrupts for this channel
   TMSK1 &= 0xFE;

   // clock prescaler = 1
   TMSK2 = 0x20;

   // ************ MAKE THIS VVVVV SO THAT BITS 1 and 0 are affected but no others
   // capture rising edge
   TCTL4 = 0x01;

   // clear the timer flag

}

