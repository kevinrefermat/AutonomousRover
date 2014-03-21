#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"

static const registerValue8_t LOOK_LEFT_PWM = 18;
static const registerValue8_t LOOK_RIGHT_PWM = 3;

static const timerCount_t PING_UPDATE_DELAY_CLOCK_CYCLES = 20800;
static const inches_t OBSTACLE_IS_NEAR_THRESHOLD = 24;

void InitializeObstacleAvoidanceSystem()
{  
   // disable interrupt caused by channel 0 for measuring PING echo
   TIE_C0I = 0;
   
   // enable interrupt caused by channel 1 for periodic PING check
   TIE_C1I = 1;

   // set ioc0 to input capture
   TIOS_IOS1 = 0;
   
   // set ic1 to output compare
   TIOS_IOS1 = 1;
      
   // disconnect timer from pin oc1
   TCTL2 &= 0xF3;
   
   SetPingRotationalPosition( 0 );
   Delay( 400 );
   CheckForObstacles();
   UpdatePingDelay();  
} 

static void UpdatePingDelay()
{
  TC1 = TCNT + PING_UPDATE_DELAY_CLOCK_CYCLES; 
}

inches_t DetectClosestObstacle()
{
   timerCount_t lengthOfEchoInClockCycles;
   
   DisableInterrupts;
   
   OutputPulseToPing();
   lengthOfEchoInClockCycles = MeasureReturnPulseFromPing() * TIMER_COUNTER_PRESCALE;
   
   EnableInterrupts;
   return ( inches_t ) lengthOfEchoInClockCycles / CLOCK_CYCLES_PER_INCH / 2;
}

static void OutputPulseToPing()
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

static timerCount_t MeasureReturnPulseFromPing()
{
   timerCount_t risingEdge;
   
   OBJECT_DETECTION_DDR = 0;
   
   // capture rising edge
   TCTL4_EDG0x = 0x01;
   
   TFLG1_C0F = 1;
   while ( !( TFLG1_C0F ) ); 
   TFLG1_C0F = 1;
   
   risingEdge = TC0;
   
   TCTL4_EDG0x = 0x02;
  
   while ( !( TFLG1_C0F ) );
   TFLG1_C0F = 1;

   if ( risingEdge < TC0 )
   {
      return TC0 - risingEdge;
   }
   else
   {
      return ( ~risingEdge + 1 ) + TC0;
   }
}

void SetPingRotationalPosition( degree_t degrees )
{
  PWMPOL_PPOL0 = 1;
  PWMCLK_PCLK0 = 1;
  PWMPRCLK_PCKA = 0x0;
  PWMCAE_CAE0 = 0;
  PWMCTL_CON01 = 0;
  
  //Divide unscaled bus clock by (128)*(2)
  PWMSCLA = 0x80;
  PWMPER0 = 167;
  
  PWMDTY0 = DegreesToClockCycles( degrees );
  
  PWME_PWME0 = 1;
}

static registerValue8_t DegreesToClockCycles( degree_t degrees )
{ 
  registerValue8_t clockCycles;
  degrees += 90;
  clockCycles = degrees;
  clockCycles = clockCycles * ( LOOK_LEFT_PWM - LOOK_RIGHT_PWM ) / 180 + LOOK_RIGHT_PWM;
  return clockCycles;
}

void CheckForObstacles()
{
   if ( DetectClosestObstacle() <= OBSTACLE_IS_NEAR_THRESHOLD )
   {
      // do something!
   }
}

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles()
{
   CheckForObstacles();
   UpdatePingDelay();
   TFLG1_C1F = 1;
}
  


