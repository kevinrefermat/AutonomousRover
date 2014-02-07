#ifndef _ROVER_H_
#define _ROVER_H_


/*** TYPE DECLARATIONS ***/

typedef unsigned char boolean_t;
typedef unsigned short gridUnit_t;
typedef signed short degree_t;
typedef unsigned short pulseCount_t;
typedef unsigned char direction_t;


typedef struct coordinates
{
   gridUnit_t m_X;
   gridUnit_t m_Y;
};

/*** ROVER IO CONTROL ***/

extern const direction_t FORWARD_MOTION;
extern const direction_t REVERSE_MOTION;
extern const direction_t STOP_MOTION;

#define MOTOR_DRIVE_LEFT_IN_0 PORTA_BIT0
#define MOTOR_DRIVE_LEFT_IN_1 PORTA_BIT1
#define MOTOR_DRIVE_RIGHT_IN_0 PORTA_BIT2
#define MOTOR_DRIVE_RIGHT_IN_1 PORTA_BIT3


/*** CONSTANTS ***/

extern const boolean_t False;
extern const boolean_t True;


/*** FLAGS ***/

extern boolean_t RoverInMotionFlag;


#endif
