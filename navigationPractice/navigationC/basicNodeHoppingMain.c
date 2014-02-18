#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char** argv )
{
   graphSize_t source, target;
   graph_t* graph;
   clock_t start, end;

   if ( argc != 4 )
   {
      printf( "Syntax: <executable> <source node ID number> <target node ID number> <y>\n" );
      exit( 1 );
   }

   source = atoi( argv[ 2 ] );
   target = atoi( argv[ 1 ] );
   graph = CreateGraphEELab();

   segment_t mySegment;
   mySegment.leftPoint.x = 36;
   mySegment.leftPoint.y = 36;
   mySegment.rightPoint.x = 345;
   mySegment.rightPoint.y = atoi( argv[ 3 ] );
   printf( "mySegment intersected something = %d\n", intersectWithObstacle( graph, &mySegment ) );
   
   start = clock();
   Dijkstra( graph, source, target ); 
   end = clock();

   printf( "It took %d clock cycles.\n", end - start );

   DestroyGraph( graph ); 

   return 0;
}
