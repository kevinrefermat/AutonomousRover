#include <hidef.h>
#include "derivative.h"

#include "Rover.h"
#include "MotorControlSystem.h"

int main()
{   
    inches_t distance;
    EnableInterrupts;
    initializeTimers();
    
    initializeMotorControlSystem();
     
    /*
    for ( ; ; )
    { 
      moveForward( 2 );
      while( RoverInMotionFlag == True );
      
      moveReverse( 2 );
      while( RoverInMotionFlag == True );
      
      rotate( 90 );
      while( RoverInMotionFlag == True );
      
      rotate( -90 );
      while( RoverInMotionFlag == True );
    }
     */     
    
    distance = detectClosestObstacle();
    for (;;);
    return 0;
}
