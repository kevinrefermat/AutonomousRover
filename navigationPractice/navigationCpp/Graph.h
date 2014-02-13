#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>

typedef uint16_t graphSize_t;
typedef int16_t inches_t;

class Graph
{
   protected:
      uint16_t m_NumberOfNodes;

   public:
      Graph( uint16_t numberOfNodes );
      ~Graph();
      virtual void AddEdge( graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance ) = 0;
      virtual void PrintGraph() = 0;
      virtual void Dijkstra( graphSize_t sourceNodeId, graphSize_t targetNodeId ) = 0;
};

#endif
