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

static void leftTreadForward( void );

static void leftTreadReverse( void );

static void rightTreadForward( void );

static void rightTreadReverse( void );   

static void brakeTreads( void );

static void disableTreads( void );

static void enableTreads( void );

static void initializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt );

static pulseCount_t distanceToPulses( inches_t distance );

static pulseCount_t degreesToPulses( degree_t degrees );

void CommenceTurnByTurnExecution( void );

boolean_t ExecuteNextTurnByTurnInstruction( void );


/*** Interrupt Service Routine ***/

interrupt VectorNumber_Vtimpaovf void motionCompleted( void );


#endif
