#include <hidef.h>
#include "derivative.h"

#include "main.h"

int main()
{
    initializeMotorControlSystem();
    move( FORWARD_MOTION, 10 );
    EnableInterrupts;
    return 0;
}
