#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAGIC_NUMBER 2000

int32_t main( int32_t argc, char** argv )
{
   int32_t guess, numberOfIterations, operand, answer, i;
   if ( argc != 3 )
   {
      printf( "Syntax: <executable> <operand> <number of iterations>\n" );
      exit( 1 );
   }
   numberOfIterations = atoi( argv[ 2 ] );
   operand = atoi( argv[ 1 ] );
   guess = operand > MAGIC_NUMBER ? 300 : 25;

   printf( "guess = %d\n", guess );
   for ( i = 0; i < numberOfIterations; i++ )
   {
      guess = guess - ( ( guess * guess ) - operand ) / ( 2 * guess );
      printf( "guess = %d\n", guess );
   }
   return 0; 
}
