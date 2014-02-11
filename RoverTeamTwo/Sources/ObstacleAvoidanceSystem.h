#include "Rover.h"

// PULL DOWN RESISTOR REQUIRED AT THE PING))) SENSOR

void initializePeriodicObjectDetection();

inches_t detectClosestObstacle();

static void outputPulseToPing();

static timerCount_t measureReturnPulseFromPing();

void setPingRotationalPosition( degree_t degrees );          

static registerValue_t degreesToClockCycles( degree_t degrees );

static void updatePingDelay();
