#include "Graph.h"
#include <stdio.h>

int main()
{
   graph_t* graph = CreateGraph( 12 );

   AddEdge( graph, 0, 1, 14);
   AddEdge( graph, 0, 2, 9);
   AddEdge( graph, 0, 3, 7);
   AddEdge( graph, 1, 2, 2);
   AddEdge( graph, 1, 4, 9);
   AddEdge( graph, 2, 3, 10);
   AddEdge( graph, 2, 5, 11);
   AddEdge( graph, 3, 5, 15);
   AddEdge( graph, 4, 5, 6);
   
   Dijkstra( graph, 0, 4 ); 
   
   DestroyGraph( graph ); 
   return 0;
}
