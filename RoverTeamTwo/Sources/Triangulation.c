#include "Rover.h" 
#include "Triangulation.h"
#include "PositioningSystem.h"
#include "Math.h"
#include "MC9S12C128.h"

static const inches_t CoarseResolution = 18;

static const inches_t UninitializedDistance = -4;
static const inches_t DistanceToBeaconsFailed = -5;

static const coordinates_t beaconCoords[ NUMBER_OF_BEACONS ] = 
{
   { 0, 720 },
   { 0, 0 },
   { 450, 0 },
   { 450, 720 },
   { 225, 360 }
};

static inches_t distanceToBeacon[ NUMBER_OF_BEACONS ];

static void ResetTriangulationVariables()
{
   Byte i;
   for ( i = 0; i < NUMBER_OF_BEACONS; i++ )
   {
      distanceToBeacon[ i ] = UninitializedDistance;
   }
}

coordinates_t Triangulate( beaconId_t firstBeacon, beaconId_t secondBeacon, beaconId_t thirdBeacon )
{
   Byte i;
   inches_t x, y, minTotalError, totalError;
   coordinates_t firstBeaconCoords, secondBeaconCoords, thirdBeaconCoords, tempCoords, closestCoords;
   
   ResetTriangulationVariables();
   
   distanceToBeacon[ firstBeacon ] = GetAccurateDistanceToBeacon( firstBeacon );
   distanceToBeacon[ secondBeacon ] = GetAccurateDistanceToBeacon( secondBeacon );
   distanceToBeacon[ thirdBeacon ] = GetAccurateDistanceToBeacon( thirdBeacon );
   
   if ( distanceToBeacon[ firstBeacon ] < 0 || distanceToBeacon[ secondBeacon ] < 0 || distanceToBeacon[ thirdBeacon ] < 0 )
   {
      closestCoords.x = DistanceToBeaconsFailed;
      closestCoords.y = DistanceToBeaconsFailed;
      return closestCoords;
   }   
   
   minTotalError = MAX_16_BIT_VALUE;
   closestCoords.x = UninitializedDistance;
   closestCoords.y = UninitializedDistance;

   for ( y = CoarseResolution; y <= 720 - CoarseResolution; y += CoarseResolution )
   {
      for ( x = CoarseResolution; x <= 450 - CoarseResolution; x += CoarseResolution )
      {
         tempCoords.x = x;
         tempCoords.y = y; 
         totalError = abs16( Distance( tempCoords, firstBeaconCoords ) - distanceToBeacon[ firstBeacon ] );
         totalError = totalError + abs16( Distance( tempCoords, secondBeaconCoords ) - distanceToBeacon[ secondBeacon ] );
         totalError = totalError + abs16( Distance( tempCoords, thirdBeaconCoords ) - distanceToBeacon[ thirdBeacon ] );
         
         if ( totalError < minTotalError )
         {
            minTotalError = totalError;
            closestCoords.x = x;
            closestCoords.y = y;
         }
      }
   }
   return closestCoords;
}