#include "MC9S12C128.h"     
#include <hidef.h>
#include <stdtypes.h>

#include "Rover.h"
#include "ObstacleAvoidanceSystem.h"
#include "MotorControlSystem.h"

static const inches_t ObstacleDetectionThreshhold = 24;
static const coordinates_t OBSTACLE_NOT_SEEN_ERROR = { -1, -1 };

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
   

   PWMPOL_PPOL1 = 1;
   PWMCLK_PCLK1 = 0;
   PWMPRCLK_PCKA = 0x0; //busclock/1
   PWMCAE_CAE1 = 0;
   PWMCTL_CON01 = 1;
   
   PWMPER01 = 40000;
   SetPingRotationalPosition( 0 );
   
  
   PWME_PWME1 = 1;
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
   timerCount_t lengthOfEchoInTimerClockCycles;
   
   DisableInterrupts;
   
   OutputPulseToPing();
   lengthOfEchoInTimerClockCycles = MeasureReturnPulseFromPing();
   
   EnableInterrupts;
   
   return ( inches_t ) lengthOfEchoInTimerClockCycles / TIMER_CLOCK_TICKS_PER_INCH_OF_SOUND_TRAVEL / 2 ;
}

static void OutputPulseToPing()
{
// creates a 5us pulse. It looks like it would make 3.5us pulse but it's actually 5us
   OBSTACLE_DETECTION_DDR = 1;

   OBSTACLE_DETECTION_PIN = 0;
   OBSTACLE_DETECTION_PIN = 1;
   _asm
   {
      nop
      nop
      nop
      nop
      nop
      nop
   }
   OBSTACLE_DETECTION_PIN = 0;
}

static timerCount_t MeasureReturnPulseFromPing()
{
   timerCount_t risingEdge;
   
   OBSTACLE_DETECTION_DDR = 0;
   
   // capture rising edge
   TCTL4_EDG0x = 0x01;
   
   TFLG1_C0F = 1;
   while ( !( TFLG1_C0F ) ); 
   TFLG1_C0F = 1;
   
   risingEdge = TC0;
   
   TCTL4_EDG0x = 0x02;
  
   while ( !( TFLG1_C0F ) );
   TFLG1_C0F = 1;

   return TC0 - risingEdge;
}

void SetPingRotationalPosition( degree_t degrees )
{
   Byte dutyCount;
   static const Word LookLeftPwmDuty = 4418;
   static const Word LookRightPwmDuty = 828;
  
   degrees += 90;
   PWMDTY01 = ( 180 - degrees ) * 20 + LookRightPwmDuty;
   CurrentPingAngle = degrees - 90;
}

coordinates_t GetLeftEdgeOfObstacle()
{
   degree_t degree;
   static degree_t leftEdgeAngle;
   inches_t distance;
   static inches_t leftEdgeDistance;
   
   coordinates_t coordinates;
   
   static const degree_t StartSweepAngle = -75;
   static const degree_t FinishSweepAngle = 10;
   
   
   SetPingRotationalPosition( StartSweepAngle );
   Delay( 500 );
   degree = StartSweepAngle;
   
   //Find the background
   for ( ; ; degree++ )
   {
      if ( DetectClosestObstacle() > ObstacleDetectionThreshhold )
      {
         SetPingRotationalPosition( degree + 1 );
         Delay( 100 );
         break;
      }
      if ( degree == FinishSweepAngle )
      {
         SetPingRotationalPosition( 0 );
         Delay( 300 );
         return OBSTACLE_NOT_SEEN_ERROR;
      }
   }
   
   // Find the left edge
   for ( ; ; degree++ )
   {
      distance = DetectClosestObstacle();
      if ( distance <= ObstacleDetectionThreshhold )
      {
         leftEdgeAngle = degree + 25;
         leftEdgeDistance = distance;
         SetPingRotationalPosition( 0 );
         Delay( 300 );
         /******* FIX THISSSSS *********/
         coordinates.x = -2;
         coordinates.y = -2;
       
         return coordinates;
      }
      if ( degree == FinishSweepAngle )
      {
         SetPingRotationalPosition( 0 );
         Delay( 300 );
         return OBSTACLE_NOT_SEEN_ERROR;
      }
      SetPingRotationalPosition( degree + 1 );
      Delay( 100 );
   }
   
}

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles()
{
   SetPingTimer();
   if ( CurrentPingAngle != 0 )
   {
      SetPingRotationalPosition( 0 );
      Delay( 300 );
   }
   if ( DetectClosestObstacle() <= ObstacleDetectionThreshhold )
   {
      /*********** IMPLEMENT MORE ELEGANT SOLUTION **************/
      StopMotion();
   }
   
   TFLG1_C1F = 1;
}
