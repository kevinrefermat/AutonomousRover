#ifndef _GRAPH_ADJACENCY_H_
#define _GRAPH_ADJACENCY_H_

#include <stdint.h>

#include "Graph.h"

typedef int16_t matrixValue_t;


class Graph_Adjacency: public Graph
{
   private:
      matrixValue_t**  m_AdjacencyMatrix;

   public:
      Graph_Adjacency( graphSize_t numberOfNodes );
      ~Graph_Adjacency();
      
      void AddEdge( graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance );
      void PrintGraph();
      
      void Dijkstra( graphSize_t sourceNodeId, graphSize_t targetNodeId );      
};

#endif
