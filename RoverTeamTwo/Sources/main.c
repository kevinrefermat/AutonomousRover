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
   InitializeTimers();
   InitializeMotorControlSystem();
   MoveForward( 10000 );
   for ( ; ; );


   TurnOnErrorLight();
   Delay ( 5000 );
   TurnOffErrorLight();

   for (;;);

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

}
