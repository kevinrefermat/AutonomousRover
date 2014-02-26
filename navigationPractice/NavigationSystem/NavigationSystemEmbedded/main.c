#include "NavigationSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char** argv )
{
   nodeNumber_t source, target;
   clock_t start, end;
   
   if ( argc != 3 )
   {
      printf( "Syntax: <executable> <source node ID> <target node ID>\n" );
      exit( 1 );
   }

   source = atoi( argv[ 2 ] );
   target = atoi( argv[ 1 ] );

   start = clock();
   InitializeNavigationSystem();
   SetRoverPosition(  84, 120 );
   // implement change such that when you change the rover's position nodes are auto updated but only rover's connections avoid redundancy
   
   printf( "%d inches of travel\n", Dijkstra( source, target ) ); 

   end = clock();
   printf( "It took %d clock cycles.\n", end - start );
   
   printRamSize();

   return 0;
}
