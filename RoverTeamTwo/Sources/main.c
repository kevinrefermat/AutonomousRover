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
static coordinates_t coordinates;
static inches_t obstacleDistance[ 151 ];

void main( void )
{  
   degree_t i;
   InitializeTimers();
   
   InitializeObstacleAvoidanceSystem();
  
   GetEdgesOfObstacle();
   for(;;);
   
   Delay( 500 );
   for ( i = -75; i <= 75; i++ )
   {  
      SetPingRotationalPosition( i );
      Delay( 100 );
      obstacleDistance[ i + 75 ] = DetectClosestObstacle();
   }
   
   for (;;);
   
   
   
   InitializePositioningSystem();
   for ( ; ; )
   {
      distance0 = GetDistanceToBeacon( 3 ); 
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
