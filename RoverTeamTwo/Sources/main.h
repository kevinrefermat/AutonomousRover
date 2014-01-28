#ifndef _MAIN_H_
#define _MAIN_H_

#include "MC9S12C128.h"


/*** ROVER IO CONTROL ***/

// not sure if this should be extern or not
struct RoverIOControl
{
	unsigned short MotorControl : 2;
};

extern const direction_t FORWARD_MOTION;
extern const direction_t REVERSE_MOTION;
extern const direction_t STOP_MOTION;

/*** TYPE DECLARATIONS ***/

typedef unsigned char boolean_t;

typedef unsigned short gridUnit_t;
typedef unsigned short degree_t;
typedef unsigned short pulseCount_t;
typedef unsigned char direction_t;

/*** CONSTANTS ***/

static const boolean_t False = 0;
static const boolean_t True = 1;

#endif
