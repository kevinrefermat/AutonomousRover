#include "MC9S12C128.h"
#include "Rover.h"
#include "MotorControlSystem.h"


/*** Constant Definitions ***/

static const pulseCount_t PULSES_PER_FOOT = 58;
static const unsigned char FEET_PER_GRID_UNIT = 1; 
static const pulseCount_t PULSES_PER_GRID_UNIT = PULSES_PER_FOOT * FEET_PER_GRID_UNIT;

static const pulseCount_t PULSES_PER_DEGREE = 2;

const direction_t FORWARD_MOTION = 0x0;
const direction_t REVERSE_MOTION = 0x1;
const direction_t STOP_MOTION = 0x2;


/*** Functions ***/

void initializeMotorControlSystem( void )
{
  // TEN must be set to allow flag clearing
  TSCR1_TEN = 1;
	
	// Set pins for output
	DDRA = 0xFF;
	stopMotion();
}

void stopMotion( void )
{
	MOTOR_DRIVE_LEFT_IN_0 = 1;
	MOTOR_DRIVE_LEFT_IN_1 = 1;
	MOTOR_DRIVE_RIGHT_IN_0 = 1;
	MOTOR_DRIVE_RIGHT_IN_1 = 1;
	
	// Clear the PA overflow flag and stop the PA
	// Writing a 1 to the PAOVF flag clears it but TEN in TSCR1 must be enabled.
	// Page 320 68HC12 book and 456 of the HCS12 manual.    
	// Disable pulse accumulator and PA interrupts
	PACTL_PAEN = 0;
	TSCR1_TEN = 1;
	PAFLG_PAOVF = 1;
	
	RoverInMotionFlag = False;
}

void moveForward( gridUnit_t distance )
{ 
  initializePulseAccumulator( distanceToPulses( distance ) );
	
	MOTOR_DRIVE_LEFT_IN_0 = 0;
	MOTOR_DRIVE_LEFT_IN_1 = 1; 
	MOTOR_DRIVE_RIGHT_IN_0 = 0;
	MOTOR_DRIVE_RIGHT_IN_1 = 1;
	
 	RoverInMotionFlag = True;	
}

void moveReverse( gridUnit_t distance )
{ 
  initializePulseAccumulator( distanceToPulses( distance ) );
	
	MOTOR_DRIVE_LEFT_IN_0 = 1; 
	MOTOR_DRIVE_LEFT_IN_1 = 0; 
	MOTOR_DRIVE_RIGHT_IN_0 = 1;
	MOTOR_DRIVE_RIGHT_IN_1 = 0;
	
 	RoverInMotionFlag = True;	
}

void rotate( degree_t degrees )
{
  initializePulseAccumulator( degreesToPulses( degrees ) );
  
  if ( degrees > 0 )
  {
    MOTOR_DRIVE_LEFT_IN_0 = 1; 
	  MOTOR_DRIVE_LEFT_IN_1 = 0;
	  MOTOR_DRIVE_RIGHT_IN_0 = 0;
	  MOTOR_DRIVE_RIGHT_IN_1 = 1;
	  
	  RoverInMotionFlag = True;
  }
  else if ( degrees < 0 )
  {    
    MOTOR_DRIVE_LEFT_IN_0 = 0; 
	  MOTOR_DRIVE_LEFT_IN_1 = 1;
	  MOTOR_DRIVE_RIGHT_IN_0 = 1;
	  MOTOR_DRIVE_RIGHT_IN_1 = 0;
	          
    RoverInMotionFlag = True;   
  }
  else
  {
    stopMotion();
  }                            
}
  

static void initializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt )
{
  // Write the negative number of encoder pulses to PACNT and enable PAOVI to interrupt when
	PACNT = ~numberOfPulsesTillInterrupt + 1;
	
	// Initialize pulse accumulator for encoders.
	PACTL = 0x52;
}

static pulseCount_t distanceToPulses( gridUnit_t distance ) 
{
	return PULSES_PER_FOOT * distance;
}

static pulseCount_t degreesToPulses( degree_t degrees) 
{
	if ( degrees >= 0 ) 
	{
	  return PULSES_PER_DEGREE * degrees;
	}
	else
	{
	  return -1 * PULSES_PER_DEGREE * degrees;
	}
}


interrupt VectorNumber_Vtimpaovf void motionCompleted()
{
	stopMotion();
}
