#ifndef _TRIANGULATION_H_
#define _TRIANGULATION_H_

#include "Rover.h" 
#include "MC9S12C128.h"                             

coordinates_t Triangulate( beaconId_t firstBeacon, beaconId_t secondBeacon, beaconId_t thirdBeacon );


static void ResetTriangulationSystem( void );

#endif