#ifndef _POSITIONING_SYSTEM_H_
#define _POSITIONING_SYSTEM_H_


#include "Rover.h"

typedef struct
{
   beaconId_t firstBeacon;
   beaconId_t secondBeacon;
   beaconId_t thirdBeacon;
} beaconGroup_t;

void InitializePositioningSystem( void );
inches_t GetAccurateLineOfSightDistanceToBeacon( beaconId_t );
void DetermineRoversPosition( void );
beaconGroup_t * GetBeaconGroup( coordinates_t approximateCoordinates );



inches_t GetLineOfSightDistanceToBeacon( beaconId_t beaconId );
static boolean_t waitForAndDetectReceivedSonarPulse( void );
void SetDistanceToBeacon( beaconId_t beaconId, inches_t distance );
inches_t GetDistanceToBeacon( beaconId_t beaconId );

#endif
