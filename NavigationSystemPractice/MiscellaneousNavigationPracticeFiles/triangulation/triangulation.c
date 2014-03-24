#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int16_t inches_t;

typedef struct
{
   inches_t x;
   inches_t y;
} coordinates_t;

typedef coordinates_t beacon_t;

static const inches_t COARSE_RESOLUTION = 18;

static const beacon_t beacon1 = { 0, 720 };
static const beacon_t beacon2 = { 0, 0 };
static const beacon_t beacon3 = { 450, 0 };

static inches_t distanceToBeacon1 = 0x7FFF;
static inches_t distanceToBeacon2 = 0x7FFF;
static inches_t distanceToBeacon3 = 0x7FFF;

static inches_t SquareRoot( int32_t operand )
{
   int32_t guess, lastGuess;
   guess = 40000;       
   for ( ; ; ) 
   {  
      lastGuess = guess;
      guess = guess - ( ( guess * guess ) - operand ) / ( 2 * guess );
      if ( lastGuess == guess ) 
      {
         break;
      }
   }   
   return ( inches_t ) guess;
}

static inches_t Distance( coordinates_t A, beacon_t B )
{
   int32_t Ax, Bx, Ay, By;
   Ax = A.x;
   Bx = B.x;
   Ay = A.y;
   By = B.y;
   return SquareRoot( ( Ax - Bx ) * ( Ax - Bx ) + ( Ay - By ) * ( Ay - By ) );
}

static coordinates_t triangulate()
{
   inches_t x, y, minTotalError, totalError;
   coordinates_t tempCoords, closestCoords;
   minTotalError = 0x7FFF;
   closestCoords = ( coordinates_t ) { 0, 0 };

   for ( y = COARSE_RESOLUTION; y <= 720 - COARSE_RESOLUTION; y += COARSE_RESOLUTION )
   {
      for ( x = COARSE_RESOLUTION; x <= 450 - COARSE_RESOLUTION; x += COARSE_RESOLUTION )
      {
         tempCoords = ( coordinates_t ) { x, y }; 
         totalError = abs( Distance( tempCoords, beacon1 ) - distanceToBeacon1 );
         totalError = totalError + abs( Distance( tempCoords, beacon2 ) - distanceToBeacon2 );
         totalError = totalError + abs( Distance( tempCoords, beacon3 ) - distanceToBeacon3 );
           
         if ( totalError < minTotalError )
         {
            minTotalError = totalError;
            closestCoords = ( coordinates_t ) { x, y };
         }
      }
   }
   for ( y = closestCoords.y - COARSE_RESOLUTION; y <= closestCoords.y + COARSE_RESOLUTION; y ++ )
   {
      for ( x = closestCoords.x - COARSE_RESOLUTION; x <= closestCoords.x + COARSE_RESOLUTION; x ++ )
      {
         tempCoords = ( coordinates_t ) { x, y }; 
         totalError = abs( Distance( tempCoords, beacon1 ) - distanceToBeacon1 );
         totalError = totalError + abs( Distance( tempCoords, beacon2 ) - distanceToBeacon2 );
         totalError = totalError + abs( Distance( tempCoords, beacon3 ) - distanceToBeacon3 );
           
         if ( totalError < minTotalError )
         {
            minTotalError = totalError;
            closestCoords = ( coordinates_t ) { x, y };
         } 
      }
   }
   return closestCoords;
}
 
int main( int argc, char** argv )
{
   coordinates_t coordinates;

   distanceToBeacon1 = atoi( argv[ 1 ] );
   distanceToBeacon2 = atoi( argv[ 2 ] );
   distanceToBeacon3 = atoi( argv[ 3 ] );

   coordinates = triangulate();
   printf( "( %d, %d )\n", coordinates.x, coordinates.y );
   return 0;
}
