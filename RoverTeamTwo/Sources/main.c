#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"
#include "PositioningSystem.h"


#include "Compass.h"
#include "I2C.h"

static inches_t distance;

void main( void )
{  
   Byte i;
   InitializeTimers();
   InitializePositioningSystem();
   for ( ; ; )
   {      
      distance = GetDistanceToBeacon( 2 );
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
