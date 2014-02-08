#include "Rover.h"

// PULL DOWN RESISTOR REQUIRED AT THE PING))) SENSOR

void initializePeriodicObjectDetection( milliseconds_t period );

inches_t detectClosestObstacle();

void setPingRotationalPosition( degree_t degrees );

static void outputPulseToPing();

static timerCount_t measureReturnPulseFromPing();
