#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"


#include "Compass.h"
#include "I2C.h"


void main( void )
{  
   InitializeTimers();
   InitializeMotorControlSystem();
   
   MoveForward( 10000 );
   for (;;);
}
