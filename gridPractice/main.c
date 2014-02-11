#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "grid.h"

int main()
{  
   struct grid* environment;
   environment = createEnvironment( xSize, ySize );
   
   addRectangle( environment, 4, 2, 12, 16 );
   addRectangle( environment, 18, 13, 8, 15 );
   addRectangle( environment, 6, 2, 1, 5 );
   addRectangle( environment, 27, 23, 3, 18 );
   addRectangle( environment, 27, 5, 24, 28 );
   printGrid( environment );

   destroyEnvironment( environment );
   return 0;
}
