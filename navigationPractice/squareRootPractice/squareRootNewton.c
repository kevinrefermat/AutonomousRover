#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAGIC_NUMBER 2000

typedef int16_t inches_t;

inline inches_t SquareRoot( int32_t operand )
{
   int32_t guess, lastGuess;
   guess = 40000;          
   for ( ; ; ) 
   {   
      printf( "%d\n", guess );  
      lastGuess = guess;
      guess = guess - ( ( guess * guess ) - operand ) / ( 2 * guess );
      if ( lastGuess == guess ) break;
   }   
   return ( inches_t ) guess;
}

int32_t main( int32_t argc, char** argv )
{
   int32_t guess, lastGuess, operand;
   if ( argc != 2 )
   {
      printf( "Syntax: <executable> <operand>\n" );
      exit( 1 );
   }
   operand = atoi( argv[ 1 ] );
   /*guess = 40000;

   printf( "guess = %d\n", guess );
   for ( ; ; )
   {
      lastGuess = guess;
      guess = guess - ( ( guess * guess ) - operand ) / ( 2 * guess );
      printf( "guess = %d\n", guess );
      if ( lastGuess == guess ) break;
   }
   */
   SquareRoot( operand );
   return 0; 
}
