#ifndef _POSITIONING_SYSTEM_H_
#define _POSITIONING_SYSTEM_H_


#include "Rover.h"

void InitializePositioningSystem( void );
inches_t GetAccurateDistanceToBeacon( beaconId_t );

inches_t GetLineOfSightDistanceToBeacon( beaconId_t beaconId );
static boolean_t waitForAndDetectReceivedSonarPulse( void );

#endif
