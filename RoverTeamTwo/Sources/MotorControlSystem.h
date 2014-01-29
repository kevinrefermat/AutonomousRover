#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_


#include "MC9S12C128.h"
#include "Rover.h"


/*** Function Prototypes ***/

void initializeMotorControlSystem( void );

boolean_t move( direction_t direction, gridUnit_t distance );

void stopMotion( void );

static int distanceToPulses( gridUnit_t distance );

interrupt VectorNumber_Vtimpaovf void motionCompleted();


#endif
