#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"

static const registerValue8_t LOOK_LEFT_PWM = 18;
static const registerValue8_t LOOK_RIGHT_PWM = 3;

static const inches_t OBSTACLE_IS_NEAR_THRESHOLD = 24;

static milliseconds_t PeriodicPingPeriod;

void EnableObstacleAvoidanceSystem()
{  
   // disable interrupt caused by channel 0 for measuring PING echo
   TIE_C0I = 0;

   // set ioc0 to input capture
   TIOS_IOS0 = 0;
      
   // internal pull device enabled
   PERT_PERT0 = 1;

   // pull device is a pull down device
   PPST_PPST0 = 1;
} 

void EnablePeriodicObstacleDetection( milliseconds_t period )
{
   SetPingRotationalPosition( 0 );
   
   // enable interrupt caused by channel 1 for periodic PING check
   TIE_C1I = 1;
   
   // set ic1 to output compare
   TIOS_IOS1 = 1;
   
   // disconnect timer from pin oc1
   // previous functional line that replaced lower two lines ---> TCTL2 &= 0xF3;
   TCTL2_OL1 = 0;
   TCTL2_OM1 = 0;

   // MATH PROOF
   // ms * ( cycles / s ) * ( 1 / prescaler ) * ( s / ms )
   // = period * ( 2000000 cycles / s ) * ( 1 tcntCycle / 32 cycles ) * ( 1 s / 1000 ms )
   // = 62.5
   // ~= 63
   
   period = period <= 1000 ? period : 1000; // 1000 * 63 is barely less than the max 16-bit value 65535
   PeriodicPingPeriod = period * 63;

   UpdatePingDelay( PeriodicPingPeriod ); 
}

void DisablePeriodicObstacleDetection()
{
   // Disable periodic ping interrupt
   TIE_C1I = 0;
}

static void UpdatePingDelay( timerCount_t tcntDelayCycles )
{
  TC1 = TCNT + tcntDelayCycles;; 
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

void PeriodicCheckForObstacles()
{
   if ( DetectClosestObstacle() <= OBSTACLE_IS_NEAR_THRESHOLD )
   {
      // do something!
   }
}

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles()
{
   PeriodicCheckForObstacles();
   SetPingRotationalPosition( 0 );
   UpdatePingDelay( PeriodicPingPeriod );
   TFLG1_C1F = 1;
}
  


