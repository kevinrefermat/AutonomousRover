#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"

static registerValue_t LOOK_LEFT_PWM = 18;
static registerValue_t LOOK_RIGHT_PWM = 3;

static timerCount_t PING_UPDATE_DELAY_CLOCK_CYCLES = 62000;

void initializePeriodicObjectDetection()
{
   updatePingDelay();  
} 

static void updatePingDelay()
{
  TC1 = TCNT + PING_UPDATE_DELAY_CLOCK_CYCLES; 
}

inches_t detectClosestObstacle()
{
<<<<<<< HEAD
   timerCount_t lengthOfEcho;
=======
   timerCount_t lengthOfEchoInClockCycles;
>>>>>>> Motors work and ping servo works. last commit before faculty review
   
   DisableInterrupts;
   
   outputPulseToPing();
<<<<<<< HEAD
   lengthOfEchoInClockCycles = measureReturnPulseFromPing();
   
   EnableInterrupts;
  
=======
   lengthOfEchoInClockCycles = measureReturnPulseFromPing() * TIMER_COUNTER_PRESCALE;
   
   EnableInterrupts;
>>>>>>> Motors work and ping servo works. last commit before faculty review
   return ( inches_t ) lengthOfEchoInClockCycles / CLOCK_CYCLES_PER_INCH / 2;
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


   // enable channel 0 to be input capture
   TIOS &= 0xFE;
   
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
<<<<<<< HEAD
=======

void setPingRotationalPosition( degree_t degrees )
{
  PWMPOL_PPOL0 = 1;
  PWMCLK_PCLK0 = 1;
  PWMPRCLK_PCKA = 0x0;
  PWMCAE_CAE0 = 0;
  PWMCTL_CON01 = 0;
  
  //Divide unscaled bus clock by (128)*(2)
  PWMSCLA = 0x80;
  PWMPER0 = 167;
  
  PWMDTY0 = degreesToClockCycles( degrees );
  
  PWME_PWME0 = 1;
}

static registerValue_t degreesToClockCycles( degree_t degrees )
{ 
  registerValue_t clockCycles;
  degrees += 90;
  clockCycles = degrees;
  clockCycles = clockCycles * ( LOOK_LEFT_PWM - LOOK_RIGHT_PWM ) / 180 + LOOK_RIGHT_PWM;
  return clockCycles;
}

interrupt VectorNumber_Vtimch1 void updateAndUseThePing()
{
	//detectClosestObstacle();
	updatePingDelay();
	setPingRotationalPosition( pingAngle );
	pingAngle += 10;
}
  


>>>>>>> Motors work and ping servo works. last commit before faculty review
