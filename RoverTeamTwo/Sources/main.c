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
      moveForward( 4 );
      while( RoverInMotionFlag == True );
      
      moveReverse( 8 );
      while( RoverInMotionFlag == True );
    }
    return 0;
}
