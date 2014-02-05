#ifndef _ROVER_H_
#define _ROVER_H_

#include "MC9S12C128.h"


/*** TYPE DECLARATIONS ***/

typedef unsigned char boolean_t;
typedef unsigned short gridUnit_t;
typedef signed short degree_t;
typedef unsigned short pulseCount_t;
typedef unsigned char direction_t;


/*** ROVER IO CONTROL ***/

extern const direction_t FORWARD_MOTION;
extern const direction_t REVERSE_MOTION;
extern const direction_t STOP_MOTION;

#define MOTOR_DRIVE_LEFT_IN_0 PORTA_BIT0
#define MOTOR_DRIVE_LEFT_IN_1 PORTA_BIT1
#define MOTOR_DRIVE_RIGHT_IN_0 PORTA_BIT2
#define MOTOR_DRIVE_RIGHT_IN_1 PORTA_BIT3
#define MOTOR_DRIVE_LEFT_ENABLE PORTA_BIT4
#define MOTOR_DRIVE_RIGHT_ENABLE PORTA_BIT5

/*typedef union
{
  unsigned char _Port
  unsigned char LeftTread   :2;
  unsigned char RightTread  :2;
  unsigned char LeftEnable  :1;
  unsigned char RightEnable :1;
  unsigned char             :2;  
} RoverMotorControl;

extern volatile RoverMotorControl MotorControlSystem @(REG_BASE + 0x00000000UL);
          */

/*** CONSTANTS ***/

extern const boolean_t False;
extern const boolean_t True;


/*** FLAGS ***/

extern boolean_t RoverInMotionFlag;


#endif
