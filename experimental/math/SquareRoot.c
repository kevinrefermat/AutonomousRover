#include <stdio.h>
#include <stdint.h>
#include <assert.h>

static uint16_t SquareRoot( uint32_t operand )
{
   uint16_t bit, mask, root;
   uint32_t acc;
            
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

int main( int argc, char** argv )
{
   int i;
   uint32_t testInput[] = { 492932, 23892439, 329389, 2343, 432586, 234, 9384923, 0 };
   uint32_t testOutput[] = { 702, 4887, 573, 48, 657, 15, 3063, 0 };

   assert( SquareRoot( testInput[0] ) == testOutput[0] );
   assert( SquareRoot( testInput[1] ) == testOutput[1] );
   assert( SquareRoot( testInput[2] ) == testOutput[2] );
   assert( SquareRoot( testInput[3] ) == testOutput[3] );
   assert( SquareRoot( testInput[4] ) == testOutput[4] );
   assert( SquareRoot( testInput[5] ) == testOutput[5] );
   assert( SquareRoot( testInput[6] ) == testOutput[6] );
   assert( SquareRoot( testInput[7] ) == testOutput[7] );
   
   printf( "All tests passed.\n" );
}
