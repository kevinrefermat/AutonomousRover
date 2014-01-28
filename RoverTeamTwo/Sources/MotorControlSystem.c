#include "MC9S12C128.h"
#include "MotorControlSystem.h"
#include "main.h"

// need to implement a flag to indicate when current motion operation is done.
// Currently, calling moveForward() will start motion and then return. Forward motion
// will cease when PAC reaches the count correlating with the distance. Thus, the loop
// moveForward() must not proceed when moveForward() returns but rather when the Rover
// reaches the end of its forward sprint.

void initializeMotorControlSystem()
{
	DDRA = 0xFF;
}

void stopMotion()
{
	PORTA = STOP_MOTION;
	
	// Disable pulse accumulator
	PACTL_PAEN = 0;
}

boolean_t move( direction_t direction, gridUnit_t distance )
{ 	
	// Clear the PA overflow flag and stop the PA
	// Writing a 1 to the PAOVF flag clears it but TEN in TSCR1 must be enabled.
	// Page 320 68HC12 book and 456 of the HCS12 manual.    		
	TSCR1_TEN = 1;
	PAFLG_PAOVF = 0x2;

  	// Write the negative number of encoder pulses to PACNT and enable PAOVI to interrupt when
	PACNT = ~distanceToPulses( distance ) + 1;
	
	// Initialize pulse accumulator for encoders.\
	// PACTL = 0x52;
	PACTL_PAEN = 1;
	PACTL_PAMOD = 0;
	PACTL_PEDGE = 1;
	PACTL_CLK = 0;
	PACTL_PAOVI = 1;
	PACTL_PAI = 0;
	
	// initialize motor drivers
	if ( direction == FORWARD_MOTION )
	{
		PORTA = FORWARD_MOTION;
	}
	else if ( direction == REVERSE_MOTION )
	{
		PORTA = REVERSE_MOTION;
	}
	else
	{
		stopMotion();
		return False;
	}
	return True;
}

int distanceToPulses( gridUnit_t distance ) 
{
	return PULSES_PER_FOOT * distance;
}


interrupt VectorNumber_Vtimpaovf void motionCompleted()
{
	stopMotion();
}
