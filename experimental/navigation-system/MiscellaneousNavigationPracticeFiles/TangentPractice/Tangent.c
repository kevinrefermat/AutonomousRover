#include <stdio.h>
#include <math.h>

int main()
{
   int i;
   double pi, theta;
   pi = 3.14159;
   theta = 0;
   for ( i = 0; i < 90; i++ )
   {
      printf( "tan( %1.3f ) = %.4f\n", theta, tan( theta * pi / 180.0 ) );
      theta = theta + 1.0;
   }
   return 0;
}
