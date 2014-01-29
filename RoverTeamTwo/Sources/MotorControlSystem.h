#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_

#include "main.h"
#include "MC9S12C128.h"

static const pulseCount_t PULSES_PER_FOOT = 58;

const direction_t FORWARD_MOTION = 0;
const direction_t REVERSE_MOTION = 1;
const direction_t STOP_MOTION = 2;

void initializeMotorControlSystem();

boolean_t move( direction_t direction, gridUnit_t distance );

void stopMotion();

static int distanceToPulses( gridUnit_t distance );

interrupt VectorNumber_Vtimpaovf void motionCompleted();

#endif
