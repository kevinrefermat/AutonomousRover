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
static const graphSize_t ROVER_ID = 0;
static const graphSize_t TARGET_1_ID = 1;
static const graphSize_t TARGET_2_ID = 2;
static const graphSize_t TARGET_3_ID = 3;
static const graphSize_t TARGET_4_ID = 4;
static const graphSize_t MAX_NUMBER_OF_NODES = 35;
static const inches_t INDEX_OUT_OF_BOUNDS = -1;
static const inches_t UNREACHABLE_TARGET = -2;


graph_t* CreateGraph( graphSize_t numberOfNodes )
{
   graphSize_t x, y;
   
   graph_t* graph;
   graph = malloc( sizeof( graph_t ) );
   
   graph->m_NumberOfNodes = numberOfNodes;
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
   graph_t* graph = CreateGraph( MAX_NUMBER_OF_NODES );
   
   AddObstacle( graph, 96, 450, BOTTOM_OF_ROOM, 36 );          // x axis shit collecting wall
   AddObstacle( graph, 420, 450, BOTTOM_OF_ROOM, 336 );        // right side low cabinets
   AddObstacle( graph, 420, 450, 444, 534 );                   // right side high cabinets
   AddObstacle( graph, 174, 330, 78, 156 );                    // right low desk
   AddObstacle( graph, 84, 156, 246, 438 );                    // left center desk
   AddObstacle( graph, 276, 348, 246, 408 );                   // right center desk
   AddObstacle( graph, 246, 330, 444, 516 );                   // right high desk
   AddObstacle( graph, 66, 222, 522, TOP_OF_ROOM );            // desk area
   AddObstacle( graph, 222, 390, 570, TOP_OF_ROOM );           // RF room
   AddObstacle( graph, 612, RIGHT_OF_ROOM, 612, TOP_OF_ROOM ); // high right corner
  
   SetNodeCoordinates( graph, 2, 30, 670 );
   SetNodeCoordinates( graph, 3, 234, 528 );
   SetNodeCoordinates( graph, 4, 345, 528 );
   SetNodeCoordinates( graph, 5, 72, 456 );
   SetNodeCoordinates( graph, 6, 174, 456 );
   SetNodeCoordinates( graph, 7, 234, 429 );
   SetNodeCoordinates( graph, 8, 345, 429 );
   SetNodeCoordinates( graph, 9, 264, 417 );
   SetNodeCoordinates( graph, 10, 363, 417 );
   SetNodeCoordinates( graph, 11, 72, 228 );
   SetNodeCoordinates( graph, 12, 174, 228 );
   SetNodeCoordinates( graph, 13, 264, 234 );
   SetNodeCoordinates( graph, 14, 363, 234 );
   SetNodeCoordinates( graph, 15, 162, 162 );
   SetNodeCoordinates( graph, 16, 345, 162 );
   SetNodeCoordinates( graph, 17, 162, 66 );
   SetNodeCoordinates( graph, 18, 345, 66 );
   SetNodeCoordinates( graph, 19, 36, 36 );
   SetNodeCoordinates( graph, 20, 408, 558 );

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

inches_t Dijkstra( graph_t* graph, graphSize_t sourceNodeId, graphSize_t targetNodeId )
{ 
   graphSize_t i, j, neighborNodeId, currentNodeId, numberOfNodes;
   inches_t shortestDistanceFromSource;
   inches_t** adjacencyMatrix;

   bool beenVisited[ graph->m_NumberOfNodes ];
   graphSize_t previousNode[ graph->m_NumberOfNodes ];
   inches_t distanceFromSource[ graph->m_NumberOfNodes ];
   numberOfNodes = graph->m_NumberOfNodes;
   adjacencyMatrix = graph->m_AdjacencyMatrix;
   
   if ( sourceNodeId >= numberOfNodes || targetNodeId >= numberOfNodes ) return INDEX_OUT_OF_BOUNDS;
   
   for ( i = 0; i < numberOfNodes; i++ )
   {
      beenVisited[ i ] = false;
      distanceFromSource[ i ] = 0x7FFF;
   }
   
   distanceFromSource[ sourceNodeId ] = 0;
   currentNodeId = sourceNodeId;
   
   for ( i = 0; i < numberOfNodes; i++ )
   {
      shortestDistanceFromSource = 0x7FFF;
      for ( neighborNodeId = 0; neighborNodeId < numberOfNodes; neighborNodeId++ )
         if ( ( adjacencyMatrix[ currentNodeId ][ neighborNodeId ] != NO_CONNECTION ) &&
            ( !beenVisited[ neighborNodeId ] ) &&
            ( distanceFromSource[ neighborNodeId ] > distanceFromSource[ currentNodeId ] + adjacencyMatrix[ currentNodeId ][ neighborNodeId ] ) )
         {
            previousNode[ neighborNodeId ] = currentNodeId;
            distanceFromSource[ neighborNodeId ] = distanceFromSource[ currentNodeId ] + adjacencyMatrix[ currentNodeId ][ neighborNodeId ]; 
         }
      
      if ( currentNodeId == targetNodeId ) break;
      beenVisited[ currentNodeId ] = true;

      for ( j = 0; j < numberOfNodes; j++ )
         if ( !beenVisited[ j ] &&
            distanceFromSource[ j ] < shortestDistanceFromSource )
         {
            shortestDistanceFromSource = distanceFromSource[ j ];
            currentNodeId = j;
         }
      //********** Implement a catch for UNREACHABLE_TARGET;
   }

   printf( "%d\n", targetNodeId );
   while ( currentNodeId != sourceNodeId )
   {
      printf( "%d\n", previousNode[ currentNodeId ] );
      currentNodeId = previousNode[ currentNodeId ];
   }
   return distanceFromSource[ targetNodeId ];
}

void AddObstacle( graph_t* graph, inches_t leftX, inches_t rightX, inches_t lowY, inches_t highY )
{ 
   graphSize_t i = graph->m_NumberOfObstacles;
 
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

void SetNodeCoordinates( graph_t* graph, graphSize_t nodeId, inches_t x, inches_t y )
{
   graph->m_NodeCoordinateList[ nodeId ].x = x;
   graph->m_NodeCoordinateList[ nodeId ].y = y;
}

void SetRoverPosition( graph_t* graph, inches_t x, inches_t y )
{
   SetNodeCoordinates( graph, ROVER_ID, x, y ); 
}

void UpdateNodeVisibilityAndDistances( graph_t* graph )
{
   graphSize_t currentNode, otherNode, numberOfNodes, x, y;
   inches_t** adjacencyMatrix;
   coordinates_t* nodeCoordinateList;
   adjacencyMatrix = graph->m_AdjacencyMatrix;
   numberOfNodes = graph->m_NumberOfNodes;
   nodeCoordinateList = graph->m_NodeCoordinateList;;
   
   for ( x = 0; x < graph->m_NumberOfNodes; x++ )
      for ( y = 0; y < graph->m_NumberOfNodes; y++ )
         adjacencyMatrix[ x ][ y ] = -1;

   for ( currentNode = 0; currentNode < numberOfNodes; currentNode++ )
      for ( otherNode = currentNode + 1; otherNode < numberOfNodes; otherNode++ )
      {
         if ( NodesAreVisibleToEachOther( graph, currentNode, otherNode ) )
         {
            adjacencyMatrix[ currentNode ][ otherNode ] = Distance( nodeCoordinateList[ currentNode ], nodeCoordinateList[ otherNode ] );
            adjacencyMatrix[ otherNode ][ currentNode ] = adjacencyMatrix[ currentNode ][ otherNode ];
         }
      }
}

inline inches_t Distance( coordinates_t A, coordinates_t B )
{
   int32_t Ax, Bx, Ay, By;
   Ax = A.x;
   Bx = B.x;
   Ay = A.y;
   By = B.y;
   return SquareRoot( ( Ax - Bx ) * ( Ax - Bx ) + ( Ay - By ) * ( Ay - By ) );
}

inline inches_t SquareRoot( int32_t operand )
{
   int32_t guess, lastGuess;
   if ( operand < 0 )
   {
      fprintf( stderr, "Error in SquareRoot() in Graph.c: operand was negative\n" );
      exit( 1 );
   }
   guess = 40000;       
   for ( ; ; ) 
   {  
      lastGuess = guess;
      guess = guess - ( ( guess * guess ) - operand ) / ( 2 * guess );
      if ( lastGuess == guess ) break;
   }   
   return ( inches_t ) guess;
}


bool NodesAreVisibleToEachOther( graph_t* graph, graphSize_t nodeId1, graphSize_t nodeId2 )
{
   segment_t segment;
   segment.leftPoint.x = graph->m_NodeCoordinateList[ nodeId1 ].x;
   segment.leftPoint.y = graph->m_NodeCoordinateList[ nodeId1 ].y;
   segment.rightPoint.x = graph->m_NodeCoordinateList[ nodeId2 ].x;
   segment.rightPoint.y = graph->m_NodeCoordinateList[ nodeId2 ].y;
   return !IntersectWithObstacle( graph, &segment );
}

inline bool IntersectWithObstacle( graph_t* graph, segment_t* segment )
{
   obstacleNumber_t i;
   for ( i = 0; i < graph->m_NumberOfObstacles; i++ )
      if ( IntersectWithShape( &( graph->m_ObstacleList[ i ] ), segment ) ) 
         return true;
   return false;
}

inline bool IntersectWithShape( tetragon_t* tetragon, segment_t* segment )
{
   return IntersectWithSegment( &( tetragon->forwardDiagonal ), segment ) ||
          IntersectWithSegment( &( tetragon->backDiagonal ), segment );
}

inline bool IntersectWithSegment( segment_t* segment0, segment_t* segment1 )
{
   return ( IsAbove( segment0, &( segment1->leftPoint ) ) + IsAbove( segment0, &( segment1->rightPoint ) ) == 1 ) &&
          ( IsAbove( segment1, &( segment0->leftPoint ) ) + IsAbove( segment1, &( segment0->rightPoint ) ) == 1 );
}

inline bool IsAbove( segment_t* segment, coordinates_t* point )
{
   int32_t BxAx, PyAy, ByAy, PxAx;
   BxAx = segment->rightPoint.x - segment->leftPoint.x;
   PyAy = point->y - segment->leftPoint.y;
   ByAy = segment->rightPoint.y - segment->leftPoint.y;
   PxAx = point->x - segment->leftPoint.x;
   return ( BxAx * PyAy - ByAy * PxAx ) > 0;
}
