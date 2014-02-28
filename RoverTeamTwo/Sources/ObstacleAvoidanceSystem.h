#include "Rover.h"

void initializePeriodicObjectDetection( void );

inches_t detectClosestObstacle( void );

static void outputPulseToPing( void );

static timerCount_t measureReturnPulseFromPing( void );

void setPingRotationalPosition( degree_t degrees );          

static registerValue_t degreesToClockCycles( degree_t degrees );

static void updatePingDelay( void );
