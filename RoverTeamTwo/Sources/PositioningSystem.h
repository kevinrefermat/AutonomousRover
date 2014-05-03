#ifndef _POSITIONING_SYSTEM_H_
#define _POSITIONING_SYSTEM_H_


#include "Rover.h"

void InitializePositioningSystem( void );
inches_t GetAccurateLineOfSightDistanceToBeacon( beaconId_t );
void DetermineRoversPosition( void );


inches_t GetLineOfSightDistanceToBeacon( beaconId_t beaconId );
static boolean_t waitForAndDetectReceivedSonarPulse( void );
void SetDistanceToBeacon( beaconId_t beaconId, inches_t distance );
inches_t GetDistanceToBeacon( beaconId_t beaconId );

#endif
