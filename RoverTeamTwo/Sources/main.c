#include <hidef.h>
#include "derivative.h"

#include "Rover.h"
#include "MotorControlSystem.h"

int main()
{   
    EnableInterrupts;
    initializeMotorControlSystem();
    for ( ; ; )
    { 
      moveForward( 1 );
      while( RoverInMotionFlag == True );
      
      moveReverse( 1 );
      while( RoverInMotionFlag == True );
      
      rotate( 1 );
      while( RoverInMotionFlag == True );
      
      rotate( -1 );
      while( RoverInMotionFlag == True );
    }
    return 0;
}
