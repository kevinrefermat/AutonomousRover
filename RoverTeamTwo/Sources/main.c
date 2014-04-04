#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"

void main( void )
{  
   TurnOffErrorLight();
   InitializeTimers();
   
   InitializeMotorControlSystem();
   0();
   //EnablePeriodicObstacleDetection( 300 );

   

   MoveForward( 1200 );
   
   //MoveForward( 60 ); 
   //Rotate( 90 ); 
   for(;;);


   InitializeNavigationSystem();

   Dijkstra( 17, 18 );
    
   EnableInterrupts;    
   CommenceTurnByTurnExecution();
   
   for (;;); 
   //initializePeriodicObjectDetection();
}
