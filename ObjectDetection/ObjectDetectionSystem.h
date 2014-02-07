#include "Rover.h"

typedef unsigned short milliseconds_t;
typedef unsigned int clockCycle_t;
typedef unsigned char inches_t;

#define OBJECT_DETECTION_PIN PORTT_BIT0
#define OBJECT_DETECTION_DDR DDRT_BIT0

// implement functionality of rotating sonar head

void initializePeriodicObjectDetection( milliseconds_t period );

inches_t detectClosestObstacle();
