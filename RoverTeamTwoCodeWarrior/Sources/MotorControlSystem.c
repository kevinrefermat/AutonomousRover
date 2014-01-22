#include "MotorControlSystem.h"

// need to implement a flag to indicate when current motion operation is done.
// Currently, calling moveForward() will start motion and then return. Forward motion
// will cease when PAC reaches the count correlating with the distance. Thus, the loop
// moveForward() must not proceed when moveForward() returns but rather when the Rover
// reaches the end of its forward sprint.

void initializeMotorControlSystem()
{
	// Write interrupt vector for PAOVI to stop the rover.
}

void moveForward( unsigned char distance )
{
	// Initialize pulse accumulator for encoders.
	// Write the negative number of encoder pulses to PACNT and enable PAOVI to interrupt when
	// number of pulses have been detected.
	// Move 2 bit number into motor driver inputs to indicate forward motion.
}

void moveBackward( unsigned char distance )
{
}

int distanceToPulses( unsigned char distance ) 
{
	return PULSES_PER_FOOT * distance;
}
