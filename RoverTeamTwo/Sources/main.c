#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"


#include "Compass.h"
#include "I2C.h"

#define SIZE 360

static degree_t bearings[ SIZE ];

void main( void )
{  
   sWord i;

   TurnOffErrorLight();
   

   InitializeCompass();
   
   for ( i = 0; i < SIZE; i++ )
   {
      if ( i % 10 == 0 ) TurnOnErrorLight();
      bearings[ i ] = GetAnAccurateCompassReading(); 
      if ( i % 10 - 5 == 0 ) TurnOffErrorLight();
   }
   
   TurnOffErrorLight();
   for( ; ; );
   //InitializeTimers();

   //InitializeMotorControlSystem();
   //EnablePeriodicObstacleDetection( 300 );



   //MoveForward( 1200 );

   //MoveForward( 60 ); 
   //Rotate( 90 ); 
   


   InitializeNavigationSystem();

   Dijkstra( 17, 18 );
    
   EnableInterrupts;    
   CommenceTurnByTurnExecution();
   
   for (;;); 
   //initializePeriodicObjectDetection();
}
