#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Graph.h"

static const inches_t NO_CONNECTION = -1;

graph_t* CreateGraph( graphSize_t numberOfNodes )
{
   graphSize_t x, y;
   graph_t* graph;
   graph = malloc( sizeof( graph_t ) );
   graph->m_NumberOfNodes = numberOfNodes;

   graph->m_AdjacencyMatrix = malloc( sizeof( inches_t* ) * graph->m_NumberOfNodes );
   for ( x = 0; x < graph->m_NumberOfNodes; x++ )
   {
      graph->m_AdjacencyMatrix[ x ] = malloc( sizeof( inches_t ) * graph->m_NumberOfNodes );
      for ( y = 0; y < graph->m_NumberOfNodes; y++ )
         graph->m_AdjacencyMatrix[ x ][ y ] = NO_CONNECTION;
   }
   return graph;
}

void DestroyGraph( graph_t* graph )
{
   graphSize_t x;
   for ( x = 0; x < graph->m_NumberOfNodes; x++ )
      free( graph->m_AdjacencyMatrix[ x ] );
   free( graph->m_AdjacencyMatrix );
   free( graph );
}

void AddEdge( graph_t* graph, graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance )
{
   if ( firstNodeId >= graph->m_NumberOfNodes || secondNodeId >= graph->m_NumberOfNodes )
   {
      fprintf( stderr, "Error in AddEdge() from Graph.c: nodeId was larger than the number of nodes.\n" );
      exit( 1 );
   }
   graph->m_AdjacencyMatrix[ firstNodeId ][ secondNodeId ] = distance;
   graph->m_AdjacencyMatrix[ secondNodeId ][ firstNodeId ] = distance;
}

bool Dijkstra( graph_t* graph, graphSize_t sourceNodeId, graphSize_t targetNodeId )
{ 
   graphSize_t i, j, neighborNodeId, currentNodeId;
   inches_t closestDistanceFromSource;
   bool beenVisited[ graph->m_NumberOfNodes ];
   graphSize_t previousNode[ graph->m_NumberOfNodes ];
   inches_t distanceFromSource[ graph->m_NumberOfNodes ];
   
   if ( sourceNodeId >= graph->m_NumberOfNodes || targetNodeId >= graph->m_NumberOfNodes )
   {
      fprintf( stderr, "Error in Dijsktra() from Graph.c: nodeId was larger than the number of nodes.\n" );
      exit( 1 );
   }
   
   for ( i = 0; i < graph->m_NumberOfNodes; i++ )
   {
      beenVisited[ i ] = false;
      distanceFromSource[ i ] = 0x7FFF;
   }
   
   distanceFromSource[ sourceNodeId ] = 0;
   currentNodeId = sourceNodeId;
   
   for ( i = 0; i < graph->m_NumberOfNodes; i++ )
   {
      closestDistanceFromSource = 0x7FFF;
      for ( neighborNodeId = 0; neighborNodeId < graph->m_NumberOfNodes; neighborNodeId++ )
      {
         if ( ( graph->m_AdjacencyMatrix[ currentNodeId ][ neighborNodeId ] != NO_CONNECTION ) &&
            ( !beenVisited[ neighborNodeId ] ) &&
            ( distanceFromSource[ neighborNodeId ] > distanceFromSource[ currentNodeId ] + graph->m_AdjacencyMatrix[ currentNodeId ][ neighborNodeId ] ) )
         {
            previousNode[ neighborNodeId ] = currentNodeId;
            distanceFromSource[ neighborNodeId ] = distanceFromSource[ currentNodeId ] + graph->m_AdjacencyMatrix[ currentNodeId ][ neighborNodeId ]; 
         }
      }
      
      if ( currentNodeId == targetNodeId ) break;
      beenVisited[ currentNodeId ] = true;

      for ( j = 0; j < graph->m_NumberOfNodes; j++ )
      {
         if ( !beenVisited[ j ] &&
            distanceFromSource[ j ] < closestDistanceFromSource )
         {
            closestDistanceFromSource = distanceFromSource[ j ];
            currentNodeId = j;
         }
      }

      if ( closestDistanceFromSource == 0x7FFF )
      {
         return false;
      }
   }

   printf( "%d\n", targetNodeId );
   while ( currentNodeId != sourceNodeId )
   {
      printf( "%d\n", previousNode[ currentNodeId ] );
      currentNodeId = previousNode[ currentNodeId ];
   }
   return true;
}
