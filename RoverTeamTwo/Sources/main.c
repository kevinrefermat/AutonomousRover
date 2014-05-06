#include <hidef.h>
#include "derivative.h"

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"
#include "PositioningSystem.h"
#include "Math.h"
#include "Triangulation.h"
#include "Celebratory.h"

#include "Compass.h"
#include "I2C.h"

static inches_t distance0, distance1, distance3;
static coordinates_t myCoordinates;
static beaconGroup_t beaconGroup;
static nodeNumber_t closestNode;

static nextState_t nextState;
static nodeNumber_t nextTargetIndex;

static nodeNumber_t targetNodes[] = { 2, 3, 1 };

void main( void )
{  
   InitializeCelebratory();
   InitializeTimers();
   InitializePositioningSystem();
   InitializeObstacleAvoidanceSystem();
   InitializeNavigationSystem( &targetNodes );
   InitializeMotorControlSystem();
   InitializeCompass();
   //CalibrateCompass();
   //Delay( 10000 );
   
   nextState = PursueTarget;
   
   SetRoversBearing( 0 );
   SetRoversPosition( 36, 36 );
   
   for ( ; ; )
   {
      switch ( nextState )
      {
         case FindClosestTarget:
            nextState = FindNextTarget( &nextTargetIndex );
            break;
         case PursueTarget:
            nextState = NavigateToTarget( nextTargetIndex );
            break;
         case SenseAndPlaceObstacle:
            DetermineRoversPosition( GetRoversPosition() );
            nextState = DetectAndPlaceObstacle();
            break;
         case TimeToCelebrate:
            Celebrate();
            for(;;);
            break;
         default:
            nextState = FindClosestTarget;
            break;
      }
   }
}
