#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_


#include "MC9S12C128.h"
#include "Rover.h"


/*** Function Prototypes ***/

void initializeMotorControlSystem( void );

void moveForward( gridUnit_t distance );

void moveReverse( gridUnit_t distance );

void rotate( degree_t degrees );

void stopMotion( void );

static void initializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt );

static pulseCount_t distanceToPulses( gridUnit_t distance );

static pulseCount_t degreesToPulses( degree_t degrees );

interrupt VectorNumber_Vtimpaovf void motionCompleted();


#endif
