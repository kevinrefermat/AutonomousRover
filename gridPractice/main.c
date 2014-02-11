#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "grid.h"

int main()
{  
   struct grid* environment;
   environment = createEnvironment( xSize, ySize );
   
   addRectangle( environment, 12, 7, 2, 10 );
   printGrid( environment );

   destroyEnvironment( environment );
   return 0;
}
