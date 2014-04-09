#include "MC9S12C128.h"     
#include <hidef.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"

/*** Static Constant Definitions ***/

static const pulseCount_t PulsesPerInch = 12;
static const pulseCount_t PulsesPerFoot = 137;
static const pulseCount_t PulsesPerFiveFeet = 712;
static const pulseCount_t PulsesPerTwentyFiveFeet = 3560;

static const pulseCount_t PULSES_PER_DEGREE_NUMERATOR = 100;
static const pulseCount_t PULSES_PER_DEGREE_DENOMINATOR = 72;

static const pulseCount_t MaxPower = 0xFF;

/*** Static Variables ***/
static timerCount_t TreadStabilizationTimerCounterOffset;
static pulseCount_t NonPulseAccumulatorPulseCount = 0;
static boolean_t lastPulseValue = 0;
static boolean_t currentPulseValue = 0;
static registerValue8_t LeftTreadPower = 0xFF;
static registerValue8_t RightTreadPower = 0xFF;


/*** Flags ***/
static boolean_t RoverInMotionFlag = 0;

/*** Constant Definitions ***/

const direction_t FORWARD_MOTION = 0x0;
const direction_t REVERSE_MOTION = 0x1;
const direction_t STOP_MOTION = 0x2;
const direction_t ROTATE_MOTION = 0x3;


/*** Functions ***/

void InitializeMotorControlSystem()
{
	MOTOR_DRIVE_DDR |= 0x0F;
	
   PWMPOL_PPOL2 = 1;
	PWMPOL_PPOL3 = 1;
	
	PWMCLK_PCLK2 = 0;
	PWMCLK_PCLK3 = 0;
	
	//PWMPRCLK_PCKB = 0x7;
	PWMPRCLK_PCKB = 0x0;
	
	PWMCAE_CAE2 = 0;
	PWMCAE_CAE3 = 0;
	
	PWMCTL_CON23 = 0;
	
	SetLeftTreadDrivePower( LeftTreadPower );
	SetRightTreadDrivePower( RightTreadPower );
	
	StopMotion();
}

void EnableTreadStabilization()
{
   milliseconds_t period = 1000 / ( 4 * DC_MOTOR_ENCODER_FREQ );
    
    // set ioc2 to output compare
   TIOS_IOS2 = 1;
   
   // disconnect timer from pin oc2
   TCTL2_OL2 = 0;
   TCTL2_OM2 = 0;
 
   if ( period > MAX_PERIOD_OF_INTERRUPT_MS )
   {
      period = MAX_PERIOD_OF_INTERRUPT_MS;  
   }
 
   TreadStabilizationTimerCounterOffset = period * TIMER_COUNTER_TICKS_PER_MS;

   SetTreadStabilizationTimer();   
   
   // Clear flag
   TFLG1_C2F = 1;
   
   // enable interrupt caused by channel 2 for periodic tread stabilization check
   TIE_C2I = 1;
}

void DisableTreadStabilization()
{
   TIE_C2I = 0;
}

static void SetTreadStabilizationTimer()
{
   TC2 = TCNT + TreadStabilizationTimerCounterOffset;  
}

static void SetLeftTreadDrivePower( registerValue8_t power )
{
	MOTOR_DRIVE_LEFT_DUTY = power;
	MOTOR_DRIVE_LEFT_PERIOD = 0xFF;
}

static void SetRightTreadDrivePower( registerValue8_t power )
{
	MOTOR_DRIVE_RIGHT_DUTY = power;
	MOTOR_DRIVE_RIGHT_PERIOD = 0xFF;
}

static void LeftTreadForward()
{
   MOTOR_DRIVE_LEFT_IN_0 = 1;
	MOTOR_DRIVE_LEFT_IN_1 = 0;
}

static void RightTreadForward()
{
   MOTOR_DRIVE_RIGHT_IN_0 = 0;
	MOTOR_DRIVE_RIGHT_IN_1 = 1;
}

static void LeftTreadReverse()
{
   MOTOR_DRIVE_LEFT_IN_0 = 0;
	MOTOR_DRIVE_LEFT_IN_1 = 1;
}

static void RightTreadReverse()
{
   MOTOR_DRIVE_RIGHT_IN_0 = 1;
	MOTOR_DRIVE_RIGHT_IN_1 = 0;
}

static void BrakeTreads()
{
   MOTOR_DRIVE_IO |= 0x0F; 
}

static void DisableTreads()
{
   MOTOR_DRIVE_LEFT_ENABLE = 0;
   MOTOR_DRIVE_RIGHT_ENABLE = 0;
}

static void EnableTreads()
{
   MOTOR_DRIVE_LEFT_ENABLE = 1;
   MOTOR_DRIVE_RIGHT_ENABLE = 1;
} 


void MoveForward( inches_t distance )
{ 
 	DisableInterrupts;
   InitializePulseAccumulator( DistanceToPulses( distance ) ); 
   DisableTreads();
	
	LeftTreadForward();
	RightTreadForward();
	
 	SetRoverInMotionFlag();	
 	                    
 	EnableTreads();
 	EnableInterrupts;
}

void MoveReverse( inches_t distance )
{ 
	DisableInterrupts;
   InitializePulseAccumulator( DistanceToPulses( distance ) ); 
   DisableTreads();
	
	LeftTreadReverse();
	RightTreadReverse();
	
 	SetRoverInMotionFlag();
 	
 	EnableTreads();
 	EnableInterrupts;	
}

void Rotate( degree_t degrees )
{
   DisableInterrupts;
   InitializePulseAccumulator( DegreesToPulses( degrees ) );
   DisableTreads();
   if ( degrees == 0 )
   {
      StopMotion();
   }
   else
   {
      if ( degrees > 0 )
      {
         LeftTreadReverse();
         RightTreadForward();
      }
      else if ( degrees < 0 )
      {    
         LeftTreadForward();
         RightTreadReverse();
	   }       
      SetRoverInMotionFlag();   
   }
   
   EnableTreads();
   EnableInterrupts;            
}

void StopMotion( void )
{
	DisableInterrupts;
	
   DisableTreads();
   BrakeTreads();
   EnableTreads();
   Delay( WAIT_FOR_ROVER_TO_ACTUALLY_STOP_DELAY );
	
	// Clear the PA overflow flag and stop the PA
	// Writing a 1 to the PAOVF flag clears it but TEN in TSCR1 must be enabled.
	// Page 320 68HC12 book and 456 of the HCS12 manual.    
	// Disable pulse accumulator and PA interrupts
	PACTL_PAEN = 0;
	PAFLG_PAOVF = 1;
	
	ClearRoverInMotionFlag();
	EnableInterrupts;
}

boolean_t GetRoverInMotionFlag()
{
   return RoverInMotionFlag;
}

/* STATIC FUNCTIONS */

static void SetRoverInMotionFlag()
{
   RoverInMotionFlag = True;
}

static void ClearRoverInMotionFlag()
{
   RoverInMotionFlag = False;
}

static void InitializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt )
{
   // Write the negative number of encoder pulses to PACNT and enable PAOVI to interrupt when
	PACNT = ~numberOfPulsesTillInterrupt + 1;
	NonPulseAccumulatorPulseCount = ~numberOfPulsesTillInterrupt + 1;
	
	// Initialize pulse accumulator for encoders.
	PACTL = 0x52;
	
	EnableTreadStabilization(); 
}

static pulseCount_t DistanceToPulses( inches_t distance ) 
{
   pulseCount_t inches, feet, fiveFeet, twentyFiveFeet;
   twentyFiveFeet = distance / ( 25 * 12 );
   distance -= twentyFiveFeet * 25 * 12;
   fiveFeet = distance / ( 5 * 12 );
   distance -= fiveFeet * 5 * 12;
   feet = distance / 12;
   distance -= feet * 12;
   inches = distance;
   twentyFiveFeet *= PulsesPerTwentyFiveFeet;
   fiveFeet *= PulsesPerFiveFeet;
   feet *= PulsesPerFoot;
   inches *= PulsesPerInch;
	return twentyFiveFeet + fiveFeet + feet + inches;
}

static pulseCount_t DegreesToPulses( degree_t degrees) 
{  
	return ( ( pulseCount_t ) ( ( 2 * ( degrees >= 0 ) - 1 ) * degrees ) ) * PULSES_PER_DEGREE_NUMERATOR / PULSES_PER_DEGREE_DENOMINATOR;
}

void CommenceTurnByTurnExecution()
{
   ExecuteNextTurnByTurnInstruction();
}

static boolean_t ExecuteNextTurnByTurnInstruction()
{
   turnByTurnElement_t* TurnByTurnElement;
   if ( HasNextTurnByTurnElement() )
   {
      TurnByTurnElement = GetNextTurnByTurnElement();
      switch ( TurnByTurnElement->typeOfMotion )
      {
         case FORWARD_MOTION: MoveForward( TurnByTurnElement->value ); break;
         case REVERSE_MOTION: MoveReverse( TurnByTurnElement->value ); break;
         case ROTATE_MOTION: Rotate( TurnByTurnElement->value ); break;
         default: break;
      }
      return True;
   } 
   return False;
}

interrupt VectorNumber_Vtimpaovf void MotionCompleted()
{
	StopMotion();
	DisableTreadStabilization();
	ExecuteNextTurnByTurnInstruction();
}

interrupt VectorNumber_Vtimch2 void PeriodicTreadStabilization()
{
   pulseCount_t PulseAccumulatorPulseCount, threshhold, pulsesRemaining;
   PulseAccumulatorPulseCount = PACNT;
   pulsesRemaining = ~PACNT + 1;
   threshhold = pulsesRemaining / 200;
   
   /*** TEST ***/
   DDRB_BIT1 = 1;
   PORTB_BIT1 = 0;
   PORTB_BIT1 = 1;
   PORTB_BIT1 = 0;
   /*** END TEST ***/
   
   SetTreadStabilizationTimer();
   
   DDRB_BIT0 = 0;
   currentPulseValue = PORTB_BIT0;
   
   // count pulses
   if ( lastPulseValue == 0 && currentPulseValue == 1 )
   {
      NonPulseAccumulatorPulseCount++;
      if ( PulseAccumulatorPulseCount % 35 == 0 )
      {
         if ( PulseAccumulatorPulseCount < NonPulseAccumulatorPulseCount )  // right tread needs to speed up
         {
            if ( RightTreadPower == MaxPower )
            {
               LeftTreadPower -= threshhold;
               DDRB_BIT5 = 1;
               DDRB_BIT7 = 1;
               PORTB_BIT5 = 0;
               PORTB_BIT7 = 1;
            }
            else
            {
               /*** for the sitch where there's an overflow RightTreadPower should be topped off and the rest should decrement from LeftTreadPower ***/
               if ( RightTreadPower + threshhold < RightTreadPower )
               {
                  RightTreadPower = MaxPower;
               }
               else
               {
                  RightTreadPower += threshhold;
               }
            }         
         }
         else if ( PulseAccumulatorPulseCount > NonPulseAccumulatorPulseCount )  // left tread needs to speed up
         {
            if ( LeftTreadPower == MaxPower )
            {
               RightTreadPower -= threshhold;
               DDRB_BIT5 = 1;
               DDRB_BIT7 = 1;
               PORTB_BIT5 = 1;
               PORTB_BIT7 = 0;  
            }
            else
            {
               if ( LeftTreadPower + threshhold < LeftTreadPower )
               {
                   LeftTreadPower = MaxPower;
               }
               else
               {
                  LeftTreadPower += threshhold;
               }
            }     
         }
      }
      SetLeftTreadDrivePower( LeftTreadPower );
      SetRightTreadDrivePower( RightTreadPower );
   }
   lastPulseValue = currentPulseValue;
   
   // Clear flag
   TFLG1_C2F = 1;
}
