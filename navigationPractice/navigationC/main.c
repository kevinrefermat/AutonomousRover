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
      printf( "Syntax: <executable> <source node ID number> <target node ID number>\n" );
      exit( 1 );
   }

   source = atoi( argv[ 1 ] );
   target = atoi( argv[ 2 ] );
    
   graph = CreateGraph( 12 );

   AddEdge( graph, 0, 1, 23);
   AddEdge( graph, 0, 3, 5);
   AddEdge( graph, 0, 2, 8);
   AddEdge( graph, 1, 4, 17);
   AddEdge( graph, 1, 5, 16);
   AddEdge( graph, 2, 3, 19);
   AddEdge( graph, 2, 6, 15);
   AddEdge( graph, 3, 7, 11);
   AddEdge( graph, 4, 5, 15);
   AddEdge( graph, 4, 8, 20);
   AddEdge( graph, 4, 7, 24);
   AddEdge( graph, 5, 8, 14);
   AddEdge( graph, 5, 11, 21);
   AddEdge( graph, 6, 7, 25);
   AddEdge( graph, 6, 9, 15);
   AddEdge( graph, 7, 9, 9);
   AddEdge( graph, 7, 10, 23);
   AddEdge( graph, 8, 10, 22);
   AddEdge( graph, 9, 10, 6);
   AddEdge( graph, 10, 11, 5);
   
   start = clock();
   Dijkstra( graph, source, target ); 
   end = clock();

   printf( "It took %d clock cycles.\n", end - start );

   DestroyGraph( graph ); 

   return 0;
}
