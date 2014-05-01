#include "Math.h"
#include "Rover.h"

static const Word TangentLut[] = 
{
   0, 1, 3, 5, 6, 8, 10, 12, 14,
   15, 17, 19, 21, 23, 24, 26, 28, 30,
   32, 34, 36, 38, 40, 42, 44, 46, 48,
   50, 53, 55, 57, 60, 62, 64, 67, 70,
   72, 75, 78, 80, 83, 86, 90, 93, 96,
   99, 103, 107, 111, 115, 119, 123, 127, 132,
   137, 142, 148, 153, 160, 166, 173, 180, 188,
   196, 205, 214, 224, 235, 247, 260, 274, 290,
   307, 327, 348, 373, 401, 433, 470, 514, 567,
   631, 711, 814, 951, 1142, 1430, 1908, 2863, 5728
};

degree_t arcTangent( sWord y, sWord x )
{
   Word uNumerator, uDenominator, uRatio;
   degree_t low, high;

   uNumerator = ( y > 0 ? y : -y );
   uDenominator = ( x > 0 ? x : -x );

   if ( uDenominator == 0 ) return ( 180 * ( y > 0 ) - 90 );
   
   uRatio = 100 * ( uNumerator / uDenominator );
   uNumerator = 10 * ( uNumerator % uDenominator );
   uRatio += 10 * ( uNumerator / uDenominator );
   uNumerator = 10 * ( uNumerator % uDenominator );
   uRatio += uNumerator / uDenominator;
   
   low = 0;
   high = 91;
   
   while ( high - low > 1 )
   {
      if ( uRatio > TangentLut[ ( low + high ) / 2 ] ) low = ( low + high ) / 2;
      else high = ( low + high ) / 2;
   }

   if ( y >= 0 )
   {
      if ( x > 0 ) return low;         // Quadrant I
      else return 180 - low;           // Quadrant II
   }
   else
   {
      if ( x < 0 ) return low - 180;   // Quadrant III
      else return ~low + 1;            // Quadrant IV
   }
}

static Word SquareRoot( LWord operand )
{
   Word bit, mask, root;
   LWord acc;
   
   bit = 16; 
   mask = 0x8000;
   root = 0x0000;
   do  
   {   
      acc = root | mask;
      if (acc * acc <= operand )
      {   
         root |= mask;
      }   
      mask >>= 1;
   } while ( --bit );
   return root;
}

inches_t Distance( coordinates_t A, coordinates_t B )
{
   sLWord Ax, Bx, Ay, By;
   Ax = A.x;
   Bx = B.x;
   Ay = A.y;
   By = B.y;
   return SquareRoot( ( Ax - Bx ) * ( Ax - Bx ) + ( Ay - By ) * ( Ay - By ) );
}

/*
inches_t SquareRoot( sLWord operand )
{
   sLWord guess, lastGuess;
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
*/

Word abs16( sWord operand )
{
   return operand < 0 ? -operand : operand;
}
