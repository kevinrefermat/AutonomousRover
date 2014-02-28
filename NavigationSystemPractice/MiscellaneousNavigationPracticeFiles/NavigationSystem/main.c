#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char** argv )
{
   graphSize_t source, target;
   graph_t* graph;
   clock_t start, end;
   
   if ( argc != 3 )
   {
      printf( "Syntax: <executable> <source node ID> <target node ID>\n" );
      exit( 1 );
   }
   source = atoi( argv[ 2 ] );
   target = atoi( argv[ 1 ] );

   start = clock();
   graph = CreateGraphEELab();
   SetRoverPosition( graph, 84, 120 );
   UpdateNodeVisibilityAndDistances( graph );
   
   printf( "%d inches of travel\n", Dijkstra( graph, source, target ) ); 

   end = clock();
   printf( "It took %d clock cycles.\n", end - start );
   DestroyGraph( graph ); 

   return 0;
}
