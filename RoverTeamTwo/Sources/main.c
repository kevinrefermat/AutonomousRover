#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "main.h"
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

void main( void )
{  
   degree_t i;
   InitializeTimers();
   InitializePositioningSystem();
   InitializeObstacleAvoidanceSystem();
   InitializeNavigationSystem();
   Dijkstra( 0, 7 );
   
   for(;;)
      distance0 = GetLineOfSightDistanceToBeacon( 4 );
   
   for( ;; )
      myCoordinates = Triangulate( 0, 1, 2 );

   myCoordinates = GetLeftEdgeOfObstacle();
   
   for ( ; ; )
   {
      distance0 = GetAccurateDistanceToBeacon( 0 );
   }
   waitForAndDetectReceivedSonarPulse();
   TurnOnErrorLight();
   for(;;); 
      
   InitializeTimers();
   InitializeMotorControlSystem();
   //CalibrateCompass();

   MoveForward( 10000 );
   for (;;);
}
 /*
nextStep_t PursueTarget( coordinates_t target )
{
   nodeNumber_t currentNode;
   
   GetCurrentLocation();
   Dijkstra( currentNode,   
}  */
