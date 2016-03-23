#include "Rover.h"
#include "Celebratory.h"
#include "MotorControlSystem.h"
#include "MC9S12C128.h"

void InitializeCelebratory()
{ 
   DDRA_BIT6 = INPUT;
   PORTA_BIT6 = 1;
   DDRA_BIT6 = OUTPUT;
}

void Celebrate()
{
   PORTA_BIT6 = 0;   
 
   Delay(250);
   MoveForward( 12 );
   while( GetRoverInMotionFlag() );
   Delay( 100 );
   CelebrateRotate( 50 );
   while( GetRoverInMotionFlag() );
   Delay( 750 );
   MoveForward( 12 );
   while( GetRoverInMotionFlag() );
   Delay(100);
   CelebrateRotate( 50 );
   while( GetRoverInMotionFlag() );
   Delay( 750 );
   MoveForward( 12 );
   while( GetRoverInMotionFlag() );
   Delay( 100 );
   CelebrateRotate( 50 );
   while( GetRoverInMotionFlag() );
   Delay( 750 );
   MoveForward( 12 );
   while( GetRoverInMotionFlag() );
   Delay( 100 );
   CelebrateRotate( 50 );
   while( GetRoverInMotionFlag() );
   Delay(750);
   CelebrateRotate(200);
   while( GetRoverInMotionFlag() );
}