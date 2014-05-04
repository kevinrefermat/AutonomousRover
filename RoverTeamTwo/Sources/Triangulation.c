#include "Rover.h" 
#include "Triangulation.h"
#include "PositioningSystem.h"
#include "Math.h"
#include "MC9S12C128.h"

/*** CONSTANTS ***/

static const inches_t CoarseResolution = 18;
static const inches_t UninitializedDistance = -4;

static const coordinates_t beaconCoords[ NUMBER_OF_BEACONS ] = 
{
   { 94, 149 },
   { 409, 400 },
   { 157, 335 },
   { 346, 142 },
   { 95, 509 }
};

/*** STATIC VARIABLES ***/

static inches_t triangulationError = MAX_SIGNED_16_BIT_VALUE;

/*** FUNCTIONS ***/

coordinates_t Triangulate( beaconId_t firstBeacon, beaconId_t secondBeacon, beaconId_t thirdBeacon )
{
   Byte i;
   inches_t x, y, totalError, yFineUpperBound, yFineLowerBound, xFineUpperBound, xFineLowerBound, firstDistance, secondDistance, thirdDistance;
   coordinates_t tempCoords, coarseCoords, closestCoords;
   
   firstDistance = GetDistanceToBeacon( firstBeacon );
   secondDistance = GetDistanceToBeacon( secondBeacon );
   thirdDistance = GetDistanceToBeacon( thirdBeacon );
   
   triangulationError = MAX_SIGNED_16_BIT_VALUE;
   coarseCoords.x = UninitializedDistance;
   coarseCoords.y = UninitializedDistance;

   for ( y = CoarseResolution; y <= 720 - CoarseResolution; y += CoarseResolution )
   {
      for ( x = CoarseResolution; x <= 450 - CoarseResolution; x += CoarseResolution )
      {
         tempCoords.x = x;
         tempCoords.y = y; 
         totalError = abs16( Distance( tempCoords, beaconCoords[ firstBeacon ] ) - firstDistance );
         totalError = totalError + abs16( Distance( tempCoords,  beaconCoords[ secondBeacon ] ) - secondDistance );
         totalError = totalError + abs16( Distance( tempCoords,  beaconCoords[ thirdBeacon ] ) - thirdDistance );
         
         if ( totalError < triangulationError )
         {
            triangulationError = totalError;
            coarseCoords.x = x;
            coarseCoords.y = y;
         }
      }
   }
   yFineLowerBound = coarseCoords.y - CoarseResolution;
   yFineUpperBound = coarseCoords.y + CoarseResolution;
   xFineLowerBound = coarseCoords.x - CoarseResolution;
   xFineUpperBound = coarseCoords.x + CoarseResolution;
   triangulationError = MAX_SIGNED_16_BIT_VALUE;

   for ( y = yFineLowerBound; y <= yFineUpperBound; y++ )
   {
      for ( x = xFineLowerBound; x <= xFineUpperBound; x++ )
      {
         tempCoords.x = x;
         tempCoords.y = y; 
         totalError = abs16( Distance( tempCoords, beaconCoords[ firstBeacon ] ) - firstDistance );
         totalError = totalError + abs16( Distance( tempCoords,  beaconCoords[ secondBeacon ] ) - secondDistance );
         totalError = totalError + abs16( Distance( tempCoords,  beaconCoords[ thirdBeacon ] ) - thirdDistance );
         
         if ( totalError < triangulationError )
         {
            triangulationError = totalError;
            closestCoords.x = x;
            closestCoords.y = y;
         }
      }
   }
   return closestCoords;
}

inches_t GetTriangulationError()
{
   return triangulationError;
}