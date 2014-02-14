#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include <stdbool.h>

typedef int16_t graphSize_t;
typedef int16_t inches_t;
typedef int8_t boolean_t;

typedef struct
{
   graphSize_t m_NumberOfNodes;
   inches_t** m_AdjacencyMatrix;
} graph_t;

graph_t* CreateGraph( graphSize_t numberOfNodes );
void DestroyGraph( graph_t* graph );

void AddEdge( graph_t* graph, graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance );
bool Dijkstra( graph_t* graph, graphSize_t sourceNodeId, graphSize_t targetNodeId );

#endif
