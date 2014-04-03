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
   //InitializeObstacleAvoidanceSystem();
   
   MoveForward( 1200 );
   
   //MoveForward( 60 ); 
   //Rotate( 90 ); 


   InitializeNavigationSystem();

   Dijkstra( 17, 18 );
    
   EnableInterrupts;    
   CommenceTurnByTurnExecution();
   
   for (;;); 
   //initializePeriodicObjectDetection();
}
