#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"
#include "MotorControlSystem.h"

static const registerValue8_t LookLeftPwmDuty = 18;
static const registerValue8_t LookRightPwmDuty = 3;

static const inches_t ObstacleDetectionThreshhold = 24;

static timerCount_t PingPeriodTimerCounterOffset;
static degree_t CurrentPingAngle;

// MAKE SURE THERE IS NO OVERFLOW IN THE MEASURE RETURN PULSE CONSIDERING VARIABLE TIMER_PRESCALER


void InitializeObstacleAvoidanceSystem()
{  
   // disable interrupt caused by channel 0 for measuring PING echo
   TIE_C0I = 0;

   // set ioc0 to input capture
   TIOS_IOS0 = 0;
      
   // internal pull device enabled
   PERT_PERT0 = 1;

   // pull device is a pull down device
   PPST_PPST0 = 1;
   
   // Clear flag
   TFLG1_C1F = 1;
   
   SetPingRotationalPosition( 0 );
   Delay( 300 );
} 

void EnablePeriodicObstacleDetection( milliseconds_t period )
{
   // set ic1 to output compare
   TIOS_IOS1 = 1;
   
   // disconnect timer from pin oc1
   // previous functional line that replaced lower two lines ---> TCTL2 &= 0xF3;
   TCTL2_OL1 = 0;
   TCTL2_OM1 = 0;
 
   if ( period > MAX_PERIOD_OF_INTERRUPT_MS )
   {
      period = MAX_PERIOD_OF_INTERRUPT_MS;  
   }
 
   PingPeriodTimerCounterOffset = period * TIMER_COUNTER_TICKS_PER_MS;

   SetPingTimer();    
   
   // Clear flag
   TFLG1_C1F = 1;
   
   // enable interrupt caused by channel 1 for periodic PING check
   TIE_C1I = 1;
}

void DisablePeriodicObstacleDetection()
{
   // Disable periodic ping interrupt
   TIE_C1I = 0;
}

static void SetPingTimer()
{
   TC1 = TCNT + PingPeriodTimerCounterOffset;
}

inches_t DetectClosestObstacle()
{
   timerCount_t lengthOfEchoInClockCycles;
   
   DisableInterrupts;
   
   OutputPulseToPing();
   lengthOfEchoInClockCycles = MeasureReturnPulseFromPing();
   lengthOfEchoInClockCycles *= TIMER_COUNTER_PRESCALE;
   
   EnableInterrupts;
   
   return ( inches_t ) lengthOfEchoInClockCycles / CLOCK_TICKS_PER_INCH_OF_SOUND_TRAVEL / 2 ;
}

static void OutputPulseToPing()
{
// creates a 5us pulse. It looks like it would make 3.5us pulse but it's actually 5us
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
   registerValue8_t clockCycles;

   PWMPOL_PPOL0 = 1;
   PWMCLK_PCLK0 = 1;
   PWMPRCLK_PCKA = 0x0;
   PWMCAE_CAE0 = 0;
   PWMCTL_CON01 = 0;
  
   //Divide unscaled bus clock by (128)*(2)
   PWMSCLA = 0x80;
   PWMPER0 = 167;
  
   degrees += 90;
   clockCycles = ( ( degree_t ) ( degrees * ( LookLeftPwmDuty - LookRightPwmDuty ) / 180 ) ) + LookRightPwmDuty;
   PWMDTY0 = clockCycles;
  
   PWME_PWME0 = 1;
   CurrentPingAngle = degrees - 90;
}

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles()
{
   SetPingTimer();
   if ( CurrentPingAngle != 0 )
   {
      SetPingRotationalPosition( 0 );
      Delay( 300000 );
   }
   if ( DetectClosestObstacle() <= ObstacleDetectionThreshhold )
   {
      /*********** IMPLEMENT MORE ELEGANT SOLUTION **************/
      StopMotion();
   }
   
   // Clear flag
   TFLG1_C1F = 1;
}
