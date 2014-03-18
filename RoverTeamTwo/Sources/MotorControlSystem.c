#include "MC9S12C128.h"     
#include <hidef.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"

/*** Static Constant Definitions ***/

static const pulseCount_t PULSES_PER_INCH = 12;
static const pulseCount_t PULSES_PER_FOOT = 137;
static const pulseCount_t PULSES_PER_FIVE_FEET = 712;
static const pulseCount_t PULSES_PER_TWENTY_FIVE_FEET = 3560;

static const pulseCount_t PULSES_PER_DEGREE_NUMERATOR = 100;
static const pulseCount_t PULSES_PER_DEGREE_DENOMINATOR = 72;


/*** Constant Definitions ***/

const direction_t FORWARD_MOTION = 0x0;
const direction_t REVERSE_MOTION = 0x1;
const direction_t STOP_MOTION = 0x2;
const direction_t ROTATE_MOTION = 0x3;


/*** Functions ***/

void initializeMotorControlSystem()
{
	DDRA = 0xFF;
	stopMotion();
}

static void leftTreadForward()
{
  MOTOR_DRIVE_LEFT_IN_0 = 1;
	MOTOR_DRIVE_LEFT_IN_1 = 0;
}

static void rightTreadForward()
{
  MOTOR_DRIVE_RIGHT_IN_0 = 0;
	MOTOR_DRIVE_RIGHT_IN_1 = 1;
}

static void leftTreadReverse()
{
  MOTOR_DRIVE_LEFT_IN_0 = 0;
	MOTOR_DRIVE_LEFT_IN_1 = 1;
}

static void rightTreadReverse()
{
  MOTOR_DRIVE_RIGHT_IN_0 = 1;
	MOTOR_DRIVE_RIGHT_IN_1 = 0;
}

static void brakeTreads()
{
  MOTOR_DRIVE_IO |= 0x0F; 
}

static void disableTreads()
{
  MOTOR_DRIVE_IO &= 0xCF;
}

static void enableTreads()
{
  MOTOR_DRIVE_IO |= 0x30;
}


void moveForward( inches_t distance )
{ 
 	DisableInterrupts;
  initializePulseAccumulator( distanceToPulses( distance ) ); 
  disableTreads();
	
	leftTreadForward();
	rightTreadForward();
	
 	RoverInMotionFlag = True;	
 	                    
 	enableTreads();
 	EnableInterrupts;
}

void moveReverse( inches_t distance )
{ 
	DisableInterrupts;
  initializePulseAccumulator( distanceToPulses( distance ) ); 
  disableTreads();
	
	leftTreadReverse();
	rightTreadReverse();
	
 	RoverInMotionFlag = True;
 	
 	enableTreads();
 	EnableInterrupts;	
}

void rotate( degree_t degrees )
{
  DisableInterrupts;
  initializePulseAccumulator( degreesToPulses( degrees ) );
  disableTreads();
  if ( degrees > 0 )
  {
    leftTreadReverse();
    rightTreadForward();
	  
	  RoverInMotionFlag = True;
  }
  else if ( degrees < 0 )
  {    
    leftTreadForward();
    rightTreadReverse();
	          
    RoverInMotionFlag = True;   
  }
  else
  {
    stopMotion();
  }                
  
  enableTreads();
  EnableInterrupts;            
}

void stopMotion( void )
{
	DisableInterrupts;
	
  disableTreads();
  brakeTreads();
  enableTreads();
  delay( WAIT_FOR_ROVER_TO_ACTUALLY_STOP_DELAY );
	
	// Clear the PA overflow flag and stop the PA
	// Writing a 1 to the PAOVF flag clears it but TEN in TSCR1 must be enabled.
	// Page 320 68HC12 book and 456 of the HCS12 manual.    
	// Disable pulse accumulator and PA interrupts
	PACTL_PAEN = 0;
	PAFLG_PAOVF = 1;
	
	RoverInMotionFlag = False;
	EnableInterrupts;
}

static void initializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt )
{
  // Write the negative number of encoder pulses to PACNT and enable PAOVI to interrupt when
	PACNT = ~numberOfPulsesTillInterrupt + 1;
	
	// Initialize pulse accumulator for encoders.
	PACTL = 0x52;
}

static pulseCount_t distanceToPulses( inches_t distance ) 
{
   pulseCount_t inches, feet, fiveFeet, twentyFiveFeet;
   twentyFiveFeet = distance / ( 25 * 12 );
   distance -= twentyFiveFeet * 25 * 12;
   fiveFeet = distance / ( 5 * 12 );
   distance -= fiveFeet * 5 * 12;
   feet = distance / 12;
   distance -= feet * 12;
   inches = distance;
   twentyFiveFeet *= PULSES_PER_TWENTY_FIVE_FEET;
   fiveFeet *= PULSES_PER_FIVE_FEET;
   feet *= PULSES_PER_FOOT;
   inches *= PULSES_PER_INCH;
	return twentyFiveFeet + fiveFeet + feet + inches;
}

static pulseCount_t degreesToPulses( degree_t degrees) 
{  
	return ( ( pulseCount_t ) ( ( 2 * ( degrees >= 0 ) - 1 ) * degrees ) ) * PULSES_PER_DEGREE_NUMERATOR / PULSES_PER_DEGREE_DENOMINATOR;
}

void CommenceTurnByTurnExecution()
{
   ExecuteNextTurnByTurnInstruction();
}

boolean_t ExecuteNextTurnByTurnInstruction()
{
   turnByTurnElement_t* TurnByTurnElement;
   if ( HasNextTurnByTurnElement() )
   {
      TurnByTurnElement = GetNextTurnByTurnElement();
      switch ( TurnByTurnElement->typeOfMotion )
      {
         case FORWARD_MOTION: moveForward( TurnByTurnElement->value ); break;
         case REVERSE_MOTION: moveReverse( TurnByTurnElement->value ); break;
         case ROTATE_MOTION: rotate( TurnByTurnElement->value ); break;
         default: break;
      }
      return True;
   } 
   return False;
}

interrupt VectorNumber_Vtimpaovf void motionCompleted()
{
	stopMotion();
	ExecuteNextTurnByTurnInstruction();
}