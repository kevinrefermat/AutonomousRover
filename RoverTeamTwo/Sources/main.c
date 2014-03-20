#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"

void main( void )
{   
   InitializeTimers();
   InitializeMotorControlSystem();
   
   //MoveForward( 60 ); 
   //Rotate( 90 ); 
   //for (;;); 

   InitializeNavigationSystem();
     
   SetRoverPosition( 120, 180 );
   AddObstacle( 10, 70, 650, 500 );
   Dijkstra( 17, 18 );
    
   EnableInterrupts;    
   CommenceTurnByTurnExecution();
   
   for (;;); 
   //initializePeriodicObjectDetection();
}
