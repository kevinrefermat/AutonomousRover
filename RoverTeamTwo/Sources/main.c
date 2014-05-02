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

void main( void )
{  
   degree_t i;
   InitializeTimers();
   InitializePositioningSystem();
   for ( ; ; )
   {
      distance0 = GetAccurateDistanceToBeacon( 0 ); 
      distance1 = GetAccurateDistanceToBeacon( 1 ); 
      distance3 = GetAccurateDistanceToBeacon( 3 );
      myCoordinates = Triangulate( 0, 1, 3 ); 
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
