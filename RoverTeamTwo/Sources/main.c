#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"

void main( void )
{       
    InitializeNavigationSystem();
    SetRoverPosition( 120, 180 );
    AddObstacle( 10, 70, 650, 500 );
    Dijkstra( 17, 18 );
    
    
    
    
    
    EnableInterrupts;
    initializeTimers();
    //initializePeriodicObjectDetection();
    initializeMotorControlSystem();

    moveForward( 10000 );
    for( ; ; )
    { 
      if ( detectClosestObstacle() < 10 ) break; 
      setPingRotationalPosition( -20 );
      delay( 150000 );
      if ( detectClosestObstacle() < 10 ) break;
      setPingRotationalPosition( 0 );
      delay( 150000 );
      if ( detectClosestObstacle() < 10 ) break;
      setPingRotationalPosition( 20 );
      delay( 150000 );
      if ( detectClosestObstacle() < 10 ) break;
      setPingRotationalPosition( 0 );
      delay( 150000 );
    }
    stopMotion();
    moveReverse( 36 );
    /*
    for ( ; ; )
    { 
      moveForward( 12 );
      while( RoverInMotionFlag == True );
      
      moveReverse( 12 );
      while( RoverInMotionFlag == True );
      
      rotate( 90 );
      while( RoverInMotionFlag == True );
      
      rotate( -90 );
      while( RoverInMotionFlag == True );
    }
    */
    for (;;);
}
