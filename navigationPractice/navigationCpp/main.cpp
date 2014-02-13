#include "Graph_Adjacency.h"
#include <iostream>

using namespace std;

int main()
{
   Graph_Adjacency * myGraph = new Graph_Adjacency( 6 );
   myGraph->AddEdge( 0, 1, 14);
   myGraph->AddEdge( 0, 2, 9);
   myGraph->AddEdge( 0, 3, 7);
   myGraph->AddEdge( 1, 2, 2);
   myGraph->AddEdge( 1, 4, 9);
   myGraph->AddEdge( 2, 3, 10);
   myGraph->AddEdge( 2, 5, 11);
   myGraph->AddEdge( 3, 5, 15);
   myGraph->AddEdge( 4, 5, 6);
   
   myGraph->Dijkstra( 0, 4 ); 
   
   myGraph->PrintGraph();

   delete myGraph;
   return 0;
}
