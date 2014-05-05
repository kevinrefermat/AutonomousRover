#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"
#include "PositioningSystem.h"
#include "Math.h"
#include "Triangulation.h"


#include "Compass.h"
#include "I2C.h"

static inches_t distance0, distance1, distance3;
static coordinates_t myCoordinates;
static beaconGroup_t beaconGroup;
static nodeNumber_t closestNode;

static nextState_t nextState;
static nodeNumber_t nextTargetIndex;

static nodeNumber_t targetNodes[] = { 0, 1, 2 };

void main( void )
{  
   InitializeTimers();
   InitializePositioningSystem();
   InitializeObstacleAvoidanceSystem();
   InitializeNavigationSystem( &targetNodes );
   InitializeMotorControlSystem();
   InitializeCompass();
   //CalibrateCompass();
   //Delay( 10000 );
   
   nextState = FindClosestTarget;
   
   for ( ; ; )
   {
      switch ( nextState )
      {
         case FindClosestTarget:
            nextState = FindNextTarget( &nextState );
            break;
         case PursueTarget:
            nextState = NavigateToTarget( nextTargetIndex );
            break;
         case SenseAndPlaceObstacle:
            DetermineRoversPosition( GetRoversPosition() );
            nextState = FindClosestTarget;
            break;
         case TimeToCelebrate:
         default:
            nextState = FindClosestTarget;
            break;
      }
   }

   NavigateToTarget( 2 );
   
   for( ;; );
}
