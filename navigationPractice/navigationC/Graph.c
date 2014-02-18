#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Graph.h"

static const inches_t NO_CONNECTION = -1;
static const obstacleNumber_t MAX_NUMBER_OF_OBSTACLES = 30;
static const inches_t BOTTOM_OF_ROOM = 0;
static const inches_t TOP_OF_ROOM = 696;
static const inches_t LEFT_OF_ROOM = 0;
static const inches_t RIGHT_OF_ROOM = 450;
static const graphSize_t CURRENT_LOCATION = 0;
static const graphSize_t TARGET_LOCATION = 1;

graph_t* CreateGraph( graphSize_t numberOfNodes )
{
   graphSize_t x, y;
   
   graph_t* graph;
   graph = malloc( sizeof( graph_t ) );
   
   graph->m_NumberOfNodes = numberOfNodes + 2; //one extra for target and one extra for current location
   graph->m_NumberOfObstacles = 0;

   graph->m_AdjacencyMatrix = malloc( sizeof( inches_t* ) * graph->m_NumberOfNodes );
   for ( x = 0; x < graph->m_NumberOfNodes; x++ )
   {
      graph->m_AdjacencyMatrix[ x ] = malloc( sizeof( inches_t ) * graph->m_NumberOfNodes );
      for ( y = 0; y < graph->m_NumberOfNodes; y++ )
         graph->m_AdjacencyMatrix[ x ][ y ] = NO_CONNECTION;
   }

   graph->m_NodeCoordinateList = malloc( graph->m_NumberOfNodes * sizeof( coordinates_t ) );

   graph->m_ObstacleList = malloc( MAX_NUMBER_OF_OBSTACLES * sizeof( tetragon_t ) );
   return graph;
}

graph_t* CreateGraphEELab()
{
   graph_t* graph = CreateGraph( 18 );
   
   AddObstacle( graph, 96, 450, BOTTOM_OF_ROOM, 36 );          // x axis shit collecting wall
   AddObstacle( graph, 420, 450, BOTTOM_OF_ROOM, 336 );        // right side low cabinets
   AddObstacle( graph, 420, 450, 444, 534 );                   // right side high cabinets
   AddObstacle( graph, 174, 330, 78, 156 );                    // right low desk
   AddObstacle( graph, 84, 156, 246, 438 );                    // left center desk
   AddObstacle( graph, 276, 348, 246, 408 );                   // right center desk
   AddObstacle( graph, 246, 330, 444, 516 );                   // right high desk
   AddObstacle( graph, 66, 222, 516, TOP_OF_ROOM );            // desk area
   AddObstacle( graph, 222, 390, 570, TOP_OF_ROOM );           // RF room
   AddObstacle( graph, 612, RIGHT_OF_ROOM, 612, TOP_OF_ROOM ); // high right corner
  
   AddEdge( graph, 0, 3, 218 );
   AddEdge( graph, 0, 9, 444 );
   AddEdge( graph, 0, 17, 634 );
   AddEdge( graph, 1, 2, 84 );
   AddEdge( graph, 1, 4, 83 );
   AddEdge( graph, 1, 5, 99 );
   AddEdge( graph, 2, 6, 99 );
   AddEdge( graph, 2, 8, 101 );
   AddEdge( graph, 3, 17, 422 );
   AddEdge( graph, 3, 9, 228 );
   AddEdge( graph, 3, 4, 102 );
   AddEdge( graph, 4, 5, 59 );
   AddEdge( graph, 4, 7, 98 );
   AddEdge( graph, 4, 10, 228 );
   AddEdge( graph, 4, 11, 240 );
   AddEdge( graph, 5, 6, 117 );
   AddEdge( graph, 5, 7, 38) ;
   AddEdge( graph, 5, 10, 208 );
   AddEdge( graph, 5, 11, 198);
   AddEdge( graph, 6, 7, 82 );
   AddEdge( graph, 6, 8, 22 );
   AddEdge( graph, 7, 8, 99 );
   AddEdge( graph, 7, 10, 209 );
   AddEdge( graph, 7, 11, 183 );
   AddEdge( graph, 8, 12, 183 );
   AddEdge( graph, 9, 10, 102 );
   AddEdge( graph, 9, 11, 192 );
   AddEdge( graph, 9, 12, 291 );
   AddEdge( graph, 9, 13, 107 );
   AddEdge( graph, 9, 14, 281 );
   AddEdge( graph, 9, 15, 182 );
   AddEdge( graph, 9, 17, 195 );
   AddEdge( graph, 10, 11, 90 );
   AddEdge( graph, 10, 12, 189 );
   AddEdge( graph, 10, 13, 67 );
   AddEdge( graph, 10, 14, 183 );
   AddEdge( graph, 10, 17, 236 );
   AddEdge( graph, 11, 12, 99 );
   AddEdge( graph, 11, 13, 125 );
   AddEdge( graph, 11, 14, 108 );
   AddEdge( graph, 12, 13, 213 );
   AddEdge( graph, 12, 14, 74 );
   AddEdge( graph, 12, 16, 169 );
   AddEdge( graph, 13, 14, 179 );
   AddEdge( graph, 13, 15, 96 );
   AddEdge( graph, 13, 17, 178 );
   AddEdge( graph, 14, 16, 96 );
   AddEdge( graph, 15, 16, 179 );
   AddEdge( graph, 15, 17, 130 );
   AddEdge( graph, 16, 17, 310 );
   return graph;
}

void DestroyGraph( graph_t* graph )
{
   graphSize_t x;
   free( graph->m_ObstacleList );
   free( graph->m_NodeCoordinateList );
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

// change int32_t so that big numbers aren't used
inline bool isAbove( segment_t* segment, coordinates_t* point )
{
   int32_t BxAx, PyAy, ByAy, PxAx;
   BxAx = segment->rightPoint.x - segment->leftPoint.x;
   PyAy = point->y - segment->leftPoint.y;
   ByAy = segment->rightPoint.y - segment->leftPoint.y;
   PxAx = point->x - segment->leftPoint.x;
   return ( BxAx * PyAy - ByAy * PxAx ) > 0;
}

inline bool intersectWithSegment( segment_t* segment0, segment_t* segment1 )
{
   return ( isAbove( segment0, &( segment1->leftPoint ) ) + isAbove( segment0, &( segment1->rightPoint ) ) == 1 ) &&
          ( isAbove( segment1, &( segment0->leftPoint ) ) + isAbove( segment1, &( segment0->rightPoint ) ) == 1 );
}

bool intersectWithShape( tetragon_t* tetragon, segment_t* segment )
{
   return intersectWithSegment( &( tetragon->forwardDiagonal ), segment ) ||
          intersectWithSegment( &( tetragon->backDiagonal ), segment );
}

void AddObstacle( graph_t* graph, inches_t leftX, inches_t rightX, inches_t lowY, inches_t highY )
{
   obstacleNumber_t i = graph->m_NumberOfObstacles;
   graph->m_ObstacleList[ i ].backDiagonal.leftPoint.x = leftX;
   graph->m_ObstacleList[ i ].backDiagonal.leftPoint.y = highY;
   graph->m_ObstacleList[ i ].backDiagonal.rightPoint.x = rightX;
   graph->m_ObstacleList[ i ].backDiagonal.rightPoint.y = lowY;
   graph->m_ObstacleList[ i ].forwardDiagonal.leftPoint.x = leftX;
   graph->m_ObstacleList[ i ].forwardDiagonal.leftPoint.y = lowY;
   graph->m_ObstacleList[ i ].forwardDiagonal.rightPoint.x = rightX;
   graph->m_ObstacleList[ i ].forwardDiagonal.rightPoint.y = highY;
   graph->m_NumberOfObstacles++;
}

bool intersectWithObstacle( graph_t* graph, segment_t* segment )
{
   obstacleNumber_t i;
   for ( i = 0; i < graph->m_NumberOfObstacles; i++ )
   {
      if ( intersectWithShape( &( graph->m_ObstacleList[ i ] ), segment ) ) 
      {
         printf( "true at obstacle %d\n", i );
         return true;
      }
   }
   return false;
}

void printVisibleNodes( coordinates_t* location )
{
   
}
