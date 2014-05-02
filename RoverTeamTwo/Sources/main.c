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

static inches_t distance0;
static coordinates_t myCoordinates;
static inches_t obstacleDistance[ 151 ];

void main( void )
{  
   degree_t i;
   InitializeTimers();
   InitializePositioningSystem();
   
   TurnOnErrorLight();
   Delay( 1000 );
   TurnOffErrorLight();
   myCoordinates = Triangulate( 0, 1, 2 );
   TurnOnErrorLight();
   for(;;);
   
   
   
   InitializePositioningSystem();
   for ( ; ; )
   {
      distance0 = GetDistanceToBeacon( 2 ); 
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
