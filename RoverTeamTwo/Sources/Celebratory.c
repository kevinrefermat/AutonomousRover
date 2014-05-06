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
 
   Delay(750);
   MoveForward( 6 );
   while( GetRoverInMotionFlag() );
   CelebrateRotate(90);
   while( GetRoverInMotionFlag() );
   MoveReverse( 6 );
   while( GetRoverInMotionFlag() );
   Delay(750);
   CelebrateRotate(90);
   while( GetRoverInMotionFlag() );
   MoveForward( 6 );
   while( GetRoverInMotionFlag() );
   CelebrateRotate(90);
   while( GetRoverInMotionFlag() );
   MoveReverse( 6 );
   while( GetRoverInMotionFlag() );
   CelebrateRotate(90);
   while( GetRoverInMotionFlag() );
   Delay(750);
   CelebrateRotate(360);
   while( GetRoverInMotionFlag() );
}