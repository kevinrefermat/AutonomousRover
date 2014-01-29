#include <hidef.h>
#include "derivative.h"

#include "Rover.h"
#include "MotorControlSystem.h"

int main()
{   
    gridUnit_t i;
    EnableInterrupts;
    initializeMotorControlSystem();
    
    i = 1;
    move( REVERSE_MOTION, 4 );
    while( RoverInMotionFlag == True );
    
    move( FORWARD_MOTION, 3 );
    while( RoverInMotionFlag == True );
    
    while( RoverInMotionFlag == False );
    
    
    return 0;
}
