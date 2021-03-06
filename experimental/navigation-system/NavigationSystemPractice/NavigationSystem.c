#include <stdio.h>

#include "MockRover.h"

#include "NavigationSystem.h"

#define BOTTOM_OF_ROOM 0
#define TOP_OF_ROOM 696
#define LEFT_OF_ROOM 0
#define RIGHT_OF_ROOM 450

static const inches_t NO_CONNECTION = -1;

#define MAX_RAM_NUMBER_OF_NODES 16
#define ROM_NUMBER_OF_NODES 21
#define MAX_TOTAL_NUMBER_OF_NODES ( MAX_RAM_NUMBER_OF_NODES + ROM_NUMBER_OF_NODES )

#define MAX_RAM_NUMBER_OF_OBSTACLES 7
#define ROM_NUMBER_OF_OBSTACLES 10
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
static nodeNumber_t RamNodeSequenceSize = 0;

// +1 for rover's current position
static nodeNumber_t NumberOfNodes = ROM_NUMBER_OF_NODES + 1;
static obstacleNumber_t NumberOfObstacles = ROM_NUMBER_OF_OBSTACLES;

static coordinates_t RamNodeCoordinateList[ MAX_RAM_NUMBER_OF_NODES ]; 
static inches_t RamAdjacencyMatrixData[ MAX_TOTAL_NUMBER_OF_NODES * ( MAX_TOTAL_NUMBER_OF_NODES - 1 ) / 2 ];
static tetragon_t RamObstacleList[ MAX_RAM_NUMBER_OF_OBSTACLES ];  
static nodeNumber_t RamNodeSequence[ MAX_TOTAL_NUMBER_OF_NODES ];
static degree_t RamDegreesSequence[ MAX_TOTAL_NUMBER_OF_NODES ];
static inches_t RamDistanceSequence[ MAX_TOTAL_NUMBER_OF_NODES ];

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
   { 408, 558 },
   { 54, 510 },
   { 234, 510 }
};

static const tetragon_t RomObstacleList[] =
{
   { 96, 450, 36, BOTTOM_OF_ROOM },          // x axis shit collecting wall
   { 420, 450, 336, BOTTOM_OF_ROOM },        // right side low cabinets
   { 420, 450, 534, 444 },                   // right side high cabinets
   { 174, 330, 156, 78 },                    // right low desk
   { 84, 156, 438, 246 },                    // left center desk
   { 276, 348, 408, 246 },                   // right center desk
   { 246, 330, 516, 444 },                   // right high desk
   { 66, 222, TOP_OF_ROOM, 522 },            // desk area
   { 222, 390, TOP_OF_ROOM, 570 },           // RF room
   { 390, RIGHT_OF_ROOM, TOP_OF_ROOM, 612 }  // high right corner
};


static boolean_t beenVisited[ MAX_TOTAL_NUMBER_OF_NODES ];
static nodeNumber_t previousNode[ MAX_TOTAL_NUMBER_OF_NODES ];
static inches_t distanceFromTarget[ MAX_TOTAL_NUMBER_OF_NODES ];

void InitializeNavigationSystem()
{
   UpdateAllNodeConnections();
}

void Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId )
{ 
   nodeNumber_t i, j, neighborNodeId, currentNodeId;
   inches_t shortestDistanceFromTarget;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      beenVisited[ i ] = False;
      distanceFromTarget[ i ] = 0x7FFF;
   }
   
   distanceFromTarget[ targetNodeId ] = 0;
   currentNodeId = targetNodeId;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      shortestDistanceFromTarget = 0x7FFF;
      for ( neighborNodeId = 0; neighborNodeId < NumberOfNodes; neighborNodeId++ )
         if ( ( GetAdjacencyMatrixValue( currentNodeId, neighborNodeId ) != NO_CONNECTION ) &&
            ( !beenVisited[ neighborNodeId ] ) &&
            ( distanceFromTarget[ neighborNodeId ] > distanceFromTarget[ currentNodeId ] + GetAdjacencyMatrixValue( currentNodeId, neighborNodeId ) ) )
         {
            previousNode[ neighborNodeId ] = currentNodeId;
            distanceFromTarget[ neighborNodeId ] = distanceFromTarget[ currentNodeId ] + GetAdjacencyMatrixValue( currentNodeId, neighborNodeId );
         }
      
      if ( currentNodeId == sourceNodeId ) break;
      beenVisited[ currentNodeId ] = True;

      for ( j = 0; j < NumberOfNodes; j++ )
         if ( !beenVisited[ j ] &&
            distanceFromTarget[ j ] < shortestDistanceFromTarget )
         {
            shortestDistanceFromTarget = distanceFromTarget[ j ];
            currentNodeId = j;
         }
      //********** Implement a catch for UNREACHABLE_TARGET;
   }
   RamNodeSequence[ 0 ] = currentNodeId;
   RamNodeSequenceSize = 1;
   while ( currentNodeId != targetNodeId )
   {
      currentNodeId = previousNode[ currentNodeId ];
      RamNodeSequence[ RamNodeSequenceSize++ ] = currentNodeId;
   }
   UpdateDegreesAndDistancesFromNodeSequence();
}

void AddObstacle( inches_t left, inches_t right, inches_t top, inches_t bottom )
{ 
   RamObstacleList[ RamNumberOfObstacles ].left = left;
   RamObstacleList[ RamNumberOfObstacles ].right = right;
   RamObstacleList[ RamNumberOfObstacles ].top = top;
   RamObstacleList[ RamNumberOfObstacles ].bottom = bottom;

   RamNumberOfObstacles++;
   NumberOfObstacles++;

   UpdateAllNodeConnections();
}

void AddNode( inches_t x, inches_t y )
{
   RamNodeCoordinateList[ RamNumberOfNodes ].x = x;
   RamNodeCoordinateList[ RamNumberOfNodes ].y = y;

   RamNumberOfNodes++;
   NumberOfNodes++;

   UpdateSingleNodeConnections( NumberOfNodes - 1 ); 
}

void SetRoverPosition( inches_t x, inches_t y )
{
   // KILL MAGIC NUMBER 0 -- DEAL WITH ROVER IDENTIFICATION BETWEEN ROM AND RAM LISTS
   RamNodeCoordinateList[ 0 ].x = x;
   RamNodeCoordinateList[ 0 ].y = y;

   // RomNumberOfNodes == roverId
   UpdateSingleNodeConnections( RomNumberOfNodes );
}

void UpdateSingleNodeConnections( nodeNumber_t node )
{
   nodeNumber_t otherNode;
   coordinates_t nodeCoords, otherNodeCoords;
   
   for ( otherNode = 0; otherNode < NumberOfNodes; otherNode++ )
   {
      if ( node != otherNode && NodesAreVisibleToEachOther( node, otherNode ) )
      {
         nodeCoords = node < RomNumberOfNodes ? RomNodeCoordinateList[ node ] : RamNodeCoordinateList[ node - RomNumberOfNodes ];
         otherNodeCoords = otherNode < RomNumberOfNodes ? RomNodeCoordinateList[ otherNode ] : RamNodeCoordinateList[ otherNode - RomNumberOfNodes ];
         SetAdjacencyMatrixValue( node, otherNode, Distance( nodeCoords, otherNodeCoords ) );
      }
      else
         SetAdjacencyMatrixValue( node, otherNode, NO_CONNECTION );
   }
}

void UpdateAllNodeConnections()
{
   nodeNumber_t currentNode, otherNode;
   coordinates_t nodeCoords, otherNodeCoords;
   
   for ( currentNode = 0; currentNode < NumberOfNodes; currentNode++ )
      for ( otherNode = currentNode + 1; otherNode < NumberOfNodes; otherNode++ )
      {
         if ( NodesAreVisibleToEachOther( currentNode, otherNode ) )
         {
            nodeCoords = currentNode < RomNumberOfNodes ? RomNodeCoordinateList[ currentNode ] : RamNodeCoordinateList[ currentNode - RomNumberOfNodes ];
            otherNodeCoords = otherNode < RomNumberOfNodes ? RomNodeCoordinateList[ otherNode ] : RamNodeCoordinateList[ otherNode - RomNumberOfNodes ];
            SetAdjacencyMatrixValue( currentNode, otherNode, Distance( nodeCoords, otherNodeCoords ) );
         }
         else
            SetAdjacencyMatrixValue( currentNode, otherNode, NO_CONNECTION );
   }
}

static inches_t Distance( coordinates_t A, coordinates_t B )
{
   sLWord Ax, Bx, Ay, By;
   Ax = A.x;
   Bx = B.x;
   Ay = A.y;
   By = B.y;
   return SquareRoot( ( Ax - Bx ) * ( Ax - Bx ) + ( Ay - By ) * ( Ay - By ) );
}

static inches_t SquareRoot( sLWord operand )
{
   sLWord guess, lastGuess;
   if ( operand < 0 )
   {
      fprintf( stderr, "Error in SquareRoot() in Graph.c: operand was negative\n" );
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


boolean_t NodesAreVisibleToEachOther( nodeNumber_t nodeId1, nodeNumber_t nodeId2 )
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

static boolean_t IntersectWithObstacle( segment_t* segment )
{
   obstacleNumber_t i;
   for ( i = 0; i < RomNumberOfObstacles; i++ )
      if ( IntersectWithShape( ( tetragon_t* )&( RomObstacleList[ i ] ), segment ) ) 
         return True;
   for ( i = 0; i < RamNumberOfObstacles; i++ )
      if ( IntersectWithShape( &( RamObstacleList[ i ] ), segment ) ) 
         return True;
   return False;
}

static boolean_t IntersectWithShape( tetragon_t* tetragon, segment_t* segment )
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

static boolean_t IntersectWithSegment( segment_t* segment0, segment_t* segment1 )
{
   return ( IsAbove( segment0, &( segment1->leftPoint ) ) + IsAbove( segment0, &( segment1->rightPoint ) ) == 1 ) &&
          ( IsAbove( segment1, &( segment0->leftPoint ) ) + IsAbove( segment1, &( segment0->rightPoint ) ) == 1 );
}


/*** DIVIDING BY 6 ALLOWS BOTH SIDES OF INEQUALITY TO BE SCALED SO I CAN USE uin16_t instead of int32_t ***/
static boolean_t IsAbove( segment_t* segment, coordinates_t* point )
{
   sLWord BxAx, PyAy, ByAy, PxAx;
   BxAx = ( segment->rightPoint.x - segment->leftPoint.x );
   PyAy = point->y - segment->leftPoint.y;
   ByAy = ( segment->rightPoint.y - segment->leftPoint.y );
   PxAx = point->x - segment->leftPoint.x;
   return BxAx * PyAy > ByAy * PxAx;
}

static void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value )
{
   if ( row == column ) return;
   if ( row < column )
   {   
      // swap because row must be greater than column
      row ^= column;
      column ^= row;
      row ^= column;
   }   
   RamAdjacencyMatrixData[ ( ( row - 1 ) * row ) / 2 + column ] = value;
}

static inches_t GetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column )
{
   if ( row == column ) return 0;
   if ( row < column )
   {   
      // swap because row must be greater than column
      row ^= column;
      column ^= row;
      row ^= column;
   }   
   return RamAdjacencyMatrixData[ ( ( row - 1 ) * row ) / 2 + column ];
}

static void UpdateDegreesAndDistancesFromNodeSequence()
{
   nodeNumber_t i;
   for ( i = 0; i < RamNodeSequenceSize - 1; i++ )
      RamDistanceSequence[ i ] = distanceFromTarget[ RamNodeSequence[ i ] ] - distanceFromTarget[ RamNodeSequence[ i + 1 ] ];
}

void printRamSize()
{
   printf( "Ram used = %d Bytes\n", sizeof( RamObstacleList ) + sizeof( RamNumberOfObstacles ) + sizeof( RamNodeCoordinateList ) + sizeof( RamNumberOfNodes ) + sizeof( RamNodeSequence ) + sizeof( RamAdjacencyMatrixData ) + sizeof( RamNodeSequenceSize ) + sizeof( RamDistanceSequence ) + sizeof( RamDegreesSequence ) );
}

void printNodeSequence()
{
   nodeNumber_t i;
   for ( i = 0; i < RamNodeSequenceSize; i++ )
      printf( "%d\n", RamNodeSequence[ i ] );
   for ( i = 0; i < RamNodeSequenceSize - 1; i++ )
      printf( "%d inches from %d to %d\n", RamDistanceSequence[ i ], RamNodeSequence[ i ], RamNodeSequence[ i + 1 ] );
}

#define SCALE_FACTOR 12

void printEnvironment()
{
   sWord width, height, row, column, i, j, k, left, right, top, bottom;
   width = RIGHT_OF_ROOM * 2 / SCALE_FACTOR;
   height = TOP_OF_ROOM / SCALE_FACTOR;
   char MatrixOfRoom[ height ][ width ];

   for ( row = 0; row < height; row++ )
   {
      MatrixOfRoom[ row ][ 0 ] = '|';
      MatrixOfRoom[ row ][ 1 ] = ' ';
      for ( column = 2; column < width - 2; column++ )
      {
         MatrixOfRoom[ row ][ column ] = ' ';
      }
      MatrixOfRoom[ row ][ width - 2 ] = ' ';
      MatrixOfRoom[ row ][ width - 1 ] = '|';
   }

   for ( i = 0; i < RomNumberOfObstacles; i++ )
   {
      left = RomObstacleList[ i ].left * 2 / SCALE_FACTOR;
      right = RomObstacleList[ i ].right * 2 / SCALE_FACTOR;
      top = RomObstacleList[ i ].top / SCALE_FACTOR;
      bottom = RomObstacleList[ i ].bottom / SCALE_FACTOR;
      
      printf( "left = %3d   right = %3d   top = %3d   bottom = %3d\n", left, right, top, bottom );

      for ( j = bottom; j < top; j++ )
      {
         for ( k = left; k < right; k++ ) 
         {
            MatrixOfRoom[ j ][ k ] = 'H'; 
         }
      }
   }
   
   for ( i = 0; i < RamNumberOfObstacles; i++ )
   {
      left = RamObstacleList[ i ].left * 2 / SCALE_FACTOR;
      right = RamObstacleList[ i ].right * 2 / SCALE_FACTOR;
      top = RamObstacleList[ i ].top / SCALE_FACTOR;
      bottom = RamObstacleList[ i ].bottom / SCALE_FACTOR;
      
      printf( "left = %3d   right = %3d   top = %3d   bottom = %3d\n", left, right, top, bottom );

      for ( j = bottom; j < top; j++ )
      {
         for ( k = left; k < right; k++ ) 
         {
            MatrixOfRoom[ j ][ k ] = 'H'; 
         }
      }
   }

   for ( i = 0; i < RomNumberOfNodes; i++ )
   {
      MatrixOfRoom[ RomNodeCoordinateList[ i ].y / SCALE_FACTOR ][ RomNodeCoordinateList[ i ].x * 2 / SCALE_FACTOR ] = i % 10 + 0x30;
      MatrixOfRoom[ RomNodeCoordinateList[ i ].y / SCALE_FACTOR ][ RomNodeCoordinateList[ i ].x * 2 / SCALE_FACTOR - 1 ] = i / 10 + 0x30;
   }
   
   for ( i = 0; i < RamNumberOfNodes; i++ )
   {
      MatrixOfRoom[ RamNodeCoordinateList[ i ].y / SCALE_FACTOR ][ RamNodeCoordinateList[ i ].x * 2 / SCALE_FACTOR ] = ( i + RomNumberOfNodes ) % 10 + 0x30;
      MatrixOfRoom[ RamNodeCoordinateList[ i ].y / SCALE_FACTOR ][ RamNodeCoordinateList[ i ].x * 2 / SCALE_FACTOR - 1 ] = ( i + RomNumberOfNodes ) / 10 + 0x30;
   }

   for ( row = 0; row < height; row++ )
   {
      for ( column = 0; column < width; column++ )
      {
         printf( "%c", MatrixOfRoom[ height - row - 1 ][ column ] );
      }
      printf( "\n" );
   }
}
