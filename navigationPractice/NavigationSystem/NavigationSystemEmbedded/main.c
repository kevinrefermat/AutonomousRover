#include "NavigationSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char** argv )
{
   nodeNumber_t source, target;
   clock_t t1, t2, t3, t4;
   
   if ( argc != 3 )
   {
      printf( "Syntax: <executable> <source node ID> <target node ID>\n" );
      exit( 1 );
   }

   source = atoi( argv[ 1 ] );
   target = atoi( argv[ 2 ] );

   t1 = clock();
   InitializeNavigationSystem();
   t2 = clock();
   SetRoverPosition(  84, 120 );
   // implement change such that when you change the rover's position nodes are auto updated but only rover's connections avoid redundancy
   
   t3 = clock();
   Dijkstra( source, target ); 
   t4 = clock();
   printNodeSequence();
   
   printRamSize();

   printf( "InitializeNavigationSystem().....%d clock cycles\n", t2 - t1 );
   printf( "SetRoverPosition()...............%d clock cycles\n", t3 - t2 );
   printf( "Dijkstra().......................%d clock cycles\n", t4 - t3 );

   printEnvironment();

   return 0;
}
