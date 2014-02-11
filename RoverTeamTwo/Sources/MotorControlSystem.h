#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_


#include "MC9S12C128.h"
#include "Rover.h"


/*** Function Prototypes ***/

void initializeMotorControlSystem( void );

void moveForward( inches_t distance );

void moveReverse( inches_t distance );

void rotate( degree_t degrees );

void stopMotion( void );


/*** Static Function Prototypes ***/

static void leftTreadForward();

static void leftTreadReverse();

static void rightTreadForward();

static void rightTreadReverse();   

static void brakeTreads();

static void disableTreads();

static void enableTreads();

static void initializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt );

static pulseCount_t distanceToPulses( inches_t distance );

static pulseCount_t degreesToPulses( degree_t degrees );


/*** Interrupt Service Routine ***/

interrupt VectorNumber_Vtimpaovf void motionCompleted();


#endif
