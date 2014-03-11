#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct
{
   int32_t ratioX1000;
   int16_t degree;
} node_t;

static node_t LUT[ 90 ];


int main()
{
   int i;
   double pi, theta;
   node_t currentNode;
   pi = 3.14159;
   theta = 0;
   for ( i = 0; i < 90; i++ )
   {
      currentNode.degree = i;
      currentNode.ratioX1000 = 100 * tan( theta * pi / 180.0 );
      theta = theta + 1.0;
      if ( i % 9 == 8 )
         printf( "%d,\n", currentNode.ratioX1000 );
      else
         printf( "%d, ", currentNode.ratioX1000 );
   }
   return 0;
}
