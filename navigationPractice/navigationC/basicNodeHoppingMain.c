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

   graph = CreateGraphEELab();
   start = clock();
   UpdateNodeVisibilityAndDistances( graph );
   end = clock();
/*
   segment_t mySegment;
   mySegment.leftPoint.x = 36;
   mySegment.leftPoint.y = 36;
   mySegment.rightPoint.x = 345;
   mySegment.rightPoint.y = atoi( argv[ 3 ] );
   printf( "mySegment intersected something = %d\n", intersectWithObstacle( graph, &mySegment ) );
  */ 
   Dijkstra( graph, source, target ); 

   printf( "It took %d clock cycles.\n", end - start );
   DestroyGraph( graph ); 

   return 0;
}
