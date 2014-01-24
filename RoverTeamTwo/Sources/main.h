#ifndef _MAIN_H_
#define _MAIN_H_

#include "MC9S12C128.h"


/*** ROVER IO CONTROL ***/

#define MOTOR_DRIVER_INPUT_0  PORTA_BIT0
#define MOTOR_DRIVER_INPUT_1  PORTA_BIT1


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