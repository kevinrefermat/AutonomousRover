#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "NavigationSystem.h"

#define BOTTOM_OF_ROOM 0
#define TOP_OF_ROOM 696
#define LEFT_OF_ROOM 0
#define RIGHT_OF_ROOM 450

static const inches_t NO_CONNECTION = -1;

#define MAX_RAM_NUMBER_OF_NODES 16
#define ROM_NUMBER_OF_NODES 19
#define MAX_TOTAL_NUMBER_OF_NODES ( MAX_RAM_NUMBER_OF_NODES + ROM_NUMBER_OF_NODES )

#define MAX_RAM_NUMBER_OF_OBSTACLES 7
#define ROM_NUMBER_OF_OBSTACLES 8
#define MAX_TOTAL_NUMBER_OF_OBSTACLES ( MAX_RAM_NUMBER_OF_OBSTACLES + ROM_NUMBER_OF_OBSTACLES )

static const nodeNumber_t MaxRamNumberOfNodes = MAX_RAM_NUMBER_OF_NODES;
static const nodeNumber_t RomNumberOfNodes = ROM_NUMBER_OF_NODES;
static const nodeNumber_t MaxTotalNumberOfNodes = MAX_TOTAL_NUMBER_OF_NODES;

static const obstacleNumber_t MaxRamNumberOfObstacles = MAX_RAM_NUMBER_OF_OBSTACLES;
static const obstacleNumber_t RomNumberOfObstacles = ROM_NUMBER_OF_OBSTACLES;
static const obstacleNumber_t MaxTotalNumberOfObstacles = MAX_TOTAL_NUMBER_OF_OBSTACLES;

// 1 for rover's current position
static nodeNumber_t RamNumberOfNodes = 1;
static obstacleNumber_t RamNumberOfObstacles = 0;

// +1 for rover's current position
static nodeNumber_t NumberOfNodes = ROM_NUMBER_OF_NODES + 1;
static obstacleNumber_t NumberOfObstacles = ROM_NUMBER_OF_OBSTACLES;

static coordinates_t RamNodeCoordinateList[ MAX_RAM_NUMBER_OF_NODES ]; 
static inches_t AdjacencyMatrix[ MAX_TOTAL_NUMBER_OF_NODES ][ MAX_TOTAL_NUMBER_OF_NODES ];
static tetragon_t RamObstacleList[ MAX_RAM_NUMBER_OF_OBSTACLES ];  

static const coordinates_t RomNodeCoordinateList[] = 
{
   { 30, 670 },
   { 234, 528 },
   { 345, 528 },
   { 72, 456 },
   { 174, 456 },
   { 234, 429 },
   { 345, 429 },
   { 264, 417 },
   { 363, 417 },
   { 72, 228 },
   { 174, 228 },
   { 264, 234 },
   { 363, 234 },
   { 162, 162 },
   { 345, 162 },
   { 162, 66 },
   { 345, 66 },
   { 36, 36 },
   { 408, 558 }
};

static const tetragon_t RomObstacleList[] =
{
   { 96, 450, BOTTOM_OF_ROOM, 36 },          // x axis shit collecting wall
   { 420, 450, BOTTOM_OF_ROOM, 336 },        // right side low cabinets
   { 420, 450, 444, 534 },                   // right side high cabinets
   { 174, 330, 78, 156 },                    // right low desk
   { 84, 156, 246, 438 },                    // left center desk
   { 276, 348, 246, 408 },                   // right center desk
   { 246, 330, 444, 516 },                   // right high desk
   { 66, 222, 522, TOP_OF_ROOM },            // desk area
   { 222, 390, 570, TOP_OF_ROOM },           // RF room
   { 612, RIGHT_OF_ROOM, 612, TOP_OF_ROOM }  // high right corner
};


static bool beenVisited[ MAX_TOTAL_NUMBER_OF_NODES ];
static nodeNumber_t previousNode[ MAX_TOTAL_NUMBER_OF_NODES ];
static inches_t distanceFromSource[ MAX_TOTAL_NUMBER_OF_NODES ];

void InitializeNavigationSystem()
{
   nodeNumber_t x, y;
   
   for ( x = 0; x < MaxTotalNumberOfNodes; x++ )
      for ( y = 0; y < MaxTotalNumberOfNodes; y++ )
         AdjacencyMatrix[ x ][ y ] = NO_CONNECTION;
}

inches_t Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId )
{ 
   nodeNumber_t i, j, neighborNodeId, currentNodeId;
   inches_t shortestDistanceFromSource;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      beenVisited[ i ] = false;
      distanceFromSource[ i ] = 0x7FFF;
   }
   
   distanceFromSource[ sourceNodeId ] = 0;
   currentNodeId = sourceNodeId;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      shortestDistanceFromSource = 0x7FFF;
      for ( neighborNodeId = 0; neighborNodeId < NumberOfNodes; neighborNodeId++ )
         if ( ( AdjacencyMatrix[ currentNodeId ][ neighborNodeId ] != NO_CONNECTION ) &&
            ( !beenVisited[ neighborNodeId ] ) &&
            ( distanceFromSource[ neighborNodeId ] > distanceFromSource[ currentNodeId ] + AdjacencyMatrix[ currentNodeId ][ neighborNodeId ] ) )
         {
            previousNode[ neighborNodeId ] = currentNodeId;
            distanceFromSource[ neighborNodeId ] = distanceFromSource[ currentNodeId ] + AdjacencyMatrix[ currentNodeId ][ neighborNodeId ]; 
         }
      
      if ( currentNodeId == targetNodeId ) break;
      beenVisited[ currentNodeId ] = true;

      for ( j = 0; j < NumberOfNodes; j++ )
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

void AddObstacle( inches_t left, inches_t right, inches_t top, inches_t bottom )
{ 
   RamObstacleList[ RamNumberOfObstacles ].left = left;
   RamObstacleList[ RamNumberOfObstacles ].right = right;
   RamObstacleList[ RamNumberOfObstacles ].top = top;
   RamObstacleList[ RamNumberOfObstacles ].bottom = bottom;
 
   RamNumberOfObstacles++;
   NumberOfObstacles++;
}

void AddNode( inches_t x, inches_t y )
{
   RamNodeCoordinateList[ RamNumberOfNodes ].x = x;
   RamNodeCoordinateList[ RamNumberOfNodes ].y = y;
   
   RamNumberOfNodes++;
   NumberOfNodes++;
}

void SetRoverPosition( inches_t x, inches_t y )
{
   // KILL MAGIC NUMBER 0 -- DEAL WITH ROVER IDENTIFICATION BETWEEN ROM AND RAM LISTS
   RamNodeCoordinateList[ 0 ].x = x;
   RamNodeCoordinateList[ 0 ].y = y;
}

void UpdateNodeVisibilityAndDistances()
{
   nodeNumber_t currentNode, otherNode, x, y;
   coordinates_t currentNodeCoordinates, otherNodeCoordinates;

   for ( x = 0; x < NumberOfNodes; x++ )
      for ( y = 0; y < NumberOfNodes; y++ )
         AdjacencyMatrix[ x ][ y ] = NO_CONNECTION;
   
   for ( currentNode = 0; currentNode < NumberOfNodes; currentNode++ )
      for ( otherNode = currentNode + 1; otherNode < NumberOfNodes; otherNode++ )
         if ( NodesAreVisibleToEachOther( currentNode, otherNode ) )
         {
            currentNodeCoordinates = currentNode < RomNumberOfNodes ? RomNodeCoordinateList[ currentNode ] : RamNodeCoordinateList[ currentNode - RomNumberOfNodes ];
            otherNodeCoordinates = otherNode < RomNumberOfNodes ? RomNodeCoordinateList[ otherNode ] : RamNodeCoordinateList[ otherNode - RomNumberOfNodes ];
            AdjacencyMatrix[ currentNode ][ otherNode ] = Distance( currentNodeCoordinates, otherNodeCoordinates );
            AdjacencyMatrix[ otherNode ][ currentNode ] = AdjacencyMatrix[ currentNode ][ otherNode ];
         }
}

static inches_t Distance( coordinates_t A, coordinates_t B )
{
   int32_t Ax, Bx, Ay, By;
   Ax = A.x;
   Bx = B.x;
   Ay = A.y;
   By = B.y;
   return SquareRoot( ( Ax - Bx ) * ( Ax - Bx ) + ( Ay - By ) * ( Ay - By ) );
}

static inches_t SquareRoot( int32_t operand )
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


bool NodesAreVisibleToEachOther( nodeNumber_t nodeId1, nodeNumber_t nodeId2 )
{
   segment_t segment;
   coordinates_t* nodeId1Coordinates, * nodeId2Coordinates;

   nodeId1Coordinates = nodeId1 < RomNumberOfNodes ? ( coordinates_t* )&RomNodeCoordinateList[ nodeId1 ] : &RamNodeCoordinateList[ nodeId1 - RomNumberOfNodes ];
   nodeId2Coordinates = nodeId2 < RomNumberOfNodes ? ( coordinates_t* )&RomNodeCoordinateList[ nodeId2 ] : &RamNodeCoordinateList[ nodeId2 - RomNumberOfNodes ];

// DOES IT MATTER THAT I AM BLINDLY ASSIGNING LEFT POINT???
   segment.leftPoint.x = nodeId1Coordinates->x;
   segment.leftPoint.y = nodeId1Coordinates->y;
   segment.rightPoint.x = nodeId2Coordinates->x;
   segment.rightPoint.y = nodeId2Coordinates->y;
   return !IntersectWithObstacle( &segment );
}

static bool IntersectWithObstacle( segment_t* segment )
{
   obstacleNumber_t i;
   for ( i = 0; i < RomNumberOfObstacles; i++ )
      if ( IntersectWithShape( ( tetragon_t* )&( RomObstacleList[ i ] ), segment ) ) 
         return true;
   for ( i = 0; i < RamNumberOfObstacles; i++ )
      if ( IntersectWithShape( &( RamObstacleList[ i ] ), segment ) ) 
         return true;
   return false;
}

static bool IntersectWithShape( tetragon_t* tetragon, segment_t* segment )
{
   segment_t forwardDiagonal, backDiagonal;
   forwardDiagonal.leftPoint.x = tetragon->left;
   forwardDiagonal.leftPoint.y = tetragon->bottom;
   forwardDiagonal.rightPoint.x = tetragon->right;
   forwardDiagonal.rightPoint.y = tetragon->top;
   backDiagonal.leftPoint.x = tetragon->left;
   backDiagonal.leftPoint.y = tetragon->top;
   backDiagonal.rightPoint.x = tetragon->right;
   backDiagonal.rightPoint.y = tetragon->bottom;
                             
   return IntersectWithSegment( &forwardDiagonal, segment ) ||
          IntersectWithSegment( &backDiagonal, segment );
}

static bool IntersectWithSegment( segment_t* segment0, segment_t* segment1 )
{
   return ( IsAbove( segment0, &( segment1->leftPoint ) ) + IsAbove( segment0, &( segment1->rightPoint ) ) == 1 ) &&
          ( IsAbove( segment1, &( segment0->leftPoint ) ) + IsAbove( segment1, &( segment0->rightPoint ) ) == 1 );
}

static bool IsAbove( segment_t* segment, coordinates_t* point )
{
   int32_t BxAx, PyAy, ByAy, PxAx;
   BxAx = segment->rightPoint.x - segment->leftPoint.x;
   PyAy = point->y - segment->leftPoint.y;
   ByAy = segment->rightPoint.y - segment->leftPoint.y;
   PxAx = point->x - segment->leftPoint.x;
   return ( BxAx * PyAy - ByAy * PxAx ) > 0;
}
