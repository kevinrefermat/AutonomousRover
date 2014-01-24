#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_

#include "main.h"
#include "MC9S12C128.h"



static const pulseCount_t PULSES_PER_FOOT = 58;

static const direction_t MOVE_FORWARD = 0;
static const direction_t MOVE_BACKWARD = 1;

void initializeMotorControlSystem();

boolean_t move( direction_t direction, gridUnit_t distance );

void stopMotion();

int distanceToPulses( gridUnit_t distance );

interrupt VectorNumber_Vtimpaovf void motionCompleted();

#endif
