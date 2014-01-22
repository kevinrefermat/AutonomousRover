#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_

#include "main.h"

static const unsigned char PULSES_PER_FOOT = 10;
static const unsigned short FORWARD_MOTION = 0x2;
static const unsigned short BACKWARD_MOTION = 0x1;

interrupt Vtimpaovf void motionCompleted();

void initializeMotorControlSystem();

void moveForward( distance_t distance );

void moveBackward( distance_t distance );

void rotate( rotation_t degrees )

pulseCount_t distanceToPulses( distance_t distance );

#endif