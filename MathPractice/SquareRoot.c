#include <stdio.h>
#include <stdint.h>

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
   uint32_t testData[] = { 492932, 23892439, 329389, 2343, 432586, 234, 9384923, 0 };
   for ( i = 0; i < 7; i++ )
   {
      printf( "The square root is %d\n", SquareRoot( testData[ i ] ) );
   }
}
