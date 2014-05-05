#include "Rover.h"
#include "NavigationSystem.h"
#include "PositioningSystem.h"
#include "Compass.h"
#include "Math.h"


/*** CONSTANTS ***/

#define BOTTOM_OF_ROOM 0
#define TOP_OF_ROOM 696
#define LEFT_OF_ROOM 0
#define RIGHT_OF_ROOM 450

static const inches_t NO_CONNECTION = -1;

#define MAX_RAM_NUMBER_OF_NODES 37
#define ROM_NUMBER_OF_NODES 10
#define MAX_TOTAL_NUMBER_OF_NODES ( MAX_RAM_NUMBER_OF_NODES + ROM_NUMBER_OF_NODES )

#define MAX_RAM_NUMBER_OF_OBSTACLES 7
#define ROM_NUMBER_OF_OBSTACLES 10
#define MAX_TOTAL_NUMBER_OF_OBSTACLES ( MAX_RAM_NUMBER_OF_OBSTACLES + ROM_NUMBER_OF_OBSTACLES )

#define MAX_NODE_SEQUENCE_SIZE 10

static const nodeNumber_t MaxRamNumberOfNodes = MAX_RAM_NUMBER_OF_NODES;
static const nodeNumber_t RomNumberOfNodes = ROM_NUMBER_OF_NODES;
static const nodeNumber_t MaxTotalNumberOfNodes = MAX_TOTAL_NUMBER_OF_NODES;

static const obstacleNumber_t MaxRamNumberOfObstacles = MAX_RAM_NUMBER_OF_OBSTACLES;
static const obstacleNumber_t RomNumberOfObstacles = ROM_NUMBER_OF_OBSTACLES;
static const obstacleNumber_t MaxTotalNumberOfObstacles = MAX_TOTAL_NUMBER_OF_OBSTACLES;

static const nodeNumber_t ROVERS_NODE_ID = ROM_NUMBER_OF_NODES;

static const inches_t TARGET_UNREACHABLE_ERROR = -1;

#define NUMBER_OF_TARGETS 3
static const nodeNumber_t NumberOfTargets = NUMBER_OF_TARGETS;

#define TARGET_TO_NODE_DISTANCE 16 //14 inches from the edge of target plus 2 inches to the center of the post

/*** TYPEDEFS ***/
                   
typedef struct
{
   sWord size;
   sWord index;
   turnByTurnElement_t queue[ MAX_TOTAL_NUMBER_OF_NODES * 2 ]; 
} turnByTurnQueue_t;


typedef struct
{
   nodeNumber_t nodeSequence[ MAX_NODE_SEQUENCE_SIZE ];
   Byte index;
   Byte size;
} nodeSequence_t;


/*** STATIC VARIABLES ***/

static turnByTurnQueue_t RamTurnByTurnQueue;
static nodeSequence_t RamNodeSequence;

// 1 for rover's current position
static nodeNumber_t RamNumberOfNodes = 1;
static obstacleNumber_t RamNumberOfObstacles = 0;

// +1 for rover's current position
static nodeNumber_t NumberOfNodes = ROM_NUMBER_OF_NODES + 1;
static obstacleNumber_t NumberOfObstacles = ROM_NUMBER_OF_OBSTACLES;

static coordinates_t RamNodeCoordinateList[ MAX_RAM_NUMBER_OF_NODES ]; 
static inches_t RamAdjacencyMatrixData[ MAX_TOTAL_NUMBER_OF_NODES * ( MAX_TOTAL_NUMBER_OF_NODES - 1 ) / 2 ];
static tetragon_t RamObstacleList[ MAX_RAM_NUMBER_OF_OBSTACLES ];

static boolean_t beenVisited[ MAX_TOTAL_NUMBER_OF_NODES ];
static nodeNumber_t previousNode[ MAX_TOTAL_NUMBER_OF_NODES ];
static inches_t distanceFromTarget[ MAX_TOTAL_NUMBER_OF_NODES ];

static degree_t RoversBearing;

static runtimeTarget_t runtimeTarget[ NUMBER_OF_TARGETS ];
 
static boolean_t visitedTarget[] = { FALSE, FALSE, FALSE };


/*** LOOKUP TABLES ***/

static const target_t TargetLookupTable[] =
{
   { { 48, 360 }, { { 48 - TARGET_TO_NODE_DISTANCE, 360 }, { 48, 360 + TARGET_TO_NODE_DISTANCE }, { 48, 360 - TARGET_TO_NODE_DISTANCE }, { -1, -1 } }, 3 },
   { { 120, 480 }, { { 120 - TARGET_TO_NODE_DISTANCE, 480 }, { 120 + TARGET_TO_NODE_DISTANCE, 480 }, { 120, 480 + TARGET_TO_NODE_DISTANCE }, { -1, -1 } }, 3 },
   { { 216, 324 }, { { 216 - TARGET_TO_NODE_DISTANCE, 324 }, { 216 + TARGET_TO_NODE_DISTANCE, 324 }, { 216, 324 - TARGET_TO_NODE_DISTANCE }, { 216, 324 + TARGET_TO_NODE_DISTANCE } }, 4 },
   { { 384, 120 }, { { 384 + TARGET_TO_NODE_DISTANCE, 120 }, { 384, 120 + TARGET_TO_NODE_DISTANCE }, { -1, -1 }, { -1, -1 } }, 2 },
   { { 396, 396 }, { { 396 - TARGET_TO_NODE_DISTANCE, 396 }, { 396 + TARGET_TO_NODE_DISTANCE, 396 }, { 396, 396 + TARGET_TO_NODE_DISTANCE }, { -1, -1 } }, 3 },
   { { 384, 540 }, { { 384 + TARGET_TO_NODE_DISTANCE, 540 }, { 384 - TARGET_TO_NODE_DISTANCE, 540 }, { 384, 540 - TARGET_TO_NODE_DISTANCE }, { -1, -1 } }, 3 }
};

static const coordinates_t RomNodeCoordinateList[] = 
{
   { 36, 36 },
   { 180, 204 },
   { 381, 199 },
   { 33, 211 },
   { 33, 495 },
   { 177, 489 },
   { 249, 199 },
   { 393, 427 },
   { 225, 415 },
   { 33, 679 }
};

static const tetragon_t RomObstacleList[] =
{
   { 96, 450, 36, BOTTOM_OF_ROOM },          // x axis shit collecting wall
   { 420, 450, 336, BOTTOM_OF_ROOM },        // right side low cabinets
   { 420, 450, 534, 444 },                   // right side high cabinets
   { 174, 354, 156, BOTTOM_OF_ROOM },        // right low desk  EXTENDED to block treachorous south wall corridor
   { 84, 156, 438, 246 },                    // left center desk
   { 276, 348, 408, 246 },                   // right center desk
   { 246, 330, 570, 444 },                   // right high desk   EXTENDED to block the space between this and RF room
   { 66, 222, TOP_OF_ROOM, 522 },            // desk area
   { 222, 390, TOP_OF_ROOM, 570 },           // RF room
   { 390, RIGHT_OF_ROOM, TOP_OF_ROOM, 612 }  // high right corner
};

void InitializeNavigationSystem( nodeNumber_t *targets )
{
   Byte i, j;
   coordinates_t tempCoords;
   for ( i = 0; i < NumberOfTargets; i++ )
   {
      runtimeTarget[ i ].startingAccessPointNodeNumber = RomNumberOfNodes + RamNumberOfNodes;
      runtimeTarget[ i ].numberOfAccessPoints = TargetLookupTable[ targets[ i ] ].numberOfAccessPoints;
      for ( j = 0; j < TargetLookupTable[ targets[ i ] ].numberOfAccessPoints; j++ )
      {
         tempCoords = TargetLookupTable[ targets[ i ] ]. accessPoint[ j ];
         AddNode( tempCoords.x, tempCoords.y );
      }
   }
   
   SetRoversPosition( 36, 36 );
   SetRoversBearing( 0 );
   UpdateAllNodeConnections();
}

inches_t Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId )
{ 
   nodeNumber_t i, j, neighborNodeId, currentNodeId;
   inches_t shortestDistanceFromTarget;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      beenVisited[ i ] = FALSE;
      distanceFromTarget[ i ] = MAX_SIGNED_16_BIT_VALUE;
   }
   
   distanceFromTarget[ targetNodeId ] = 0;
   currentNodeId = targetNodeId;
   
   for ( i = 0; i < NumberOfNodes; i++ )
   {
      shortestDistanceFromTarget = MAX_SIGNED_16_BIT_VALUE;
      for ( neighborNodeId = 0; neighborNodeId < NumberOfNodes; neighborNodeId++ )
      {
        
         if ( ( GetAdjacencyMatrixValue( currentNodeId, neighborNodeId ) != NO_CONNECTION ) &&
            ( !beenVisited[ neighborNodeId ] ) &&
            ( distanceFromTarget[ neighborNodeId ] > distanceFromTarget[ currentNodeId ] + GetAdjacencyMatrixValue( currentNodeId, neighborNodeId ) ) )
         {
            previousNode[ neighborNodeId ] = currentNodeId;
            distanceFromTarget[ neighborNodeId ] = distanceFromTarget[ currentNodeId ] + GetAdjacencyMatrixValue( currentNodeId, neighborNodeId );
         }
      }
      if ( currentNodeId == sourceNodeId ) break;
      beenVisited[ currentNodeId ] = TRUE;

      for ( j = 0; j < NumberOfNodes; j++ )
      {
         if ( !beenVisited[ j ] &&
            distanceFromTarget[ j ] < shortestDistanceFromTarget )
         {
            shortestDistanceFromTarget = distanceFromTarget[ j ];
            currentNodeId = j;
         }
      }
   }
   
   if ( distanceFromTarget[ sourceNodeId ] >= MAX_SIGNED_16_BIT_VALUE )
   {
      RamNodeSequence.nodeSequence[ 0 ] = -1;
      RamNodeSequence.size = -1;
      RamNodeSequence.index = -1;
      return TARGET_UNREACHABLE_ERROR;
   }
   
   RamNodeSequence.size = 1;
   RamNodeSequence.nodeSequence[ 0 ] = GetRoversNodeId();
   
   while ( currentNodeId != targetNodeId )
   {
      currentNodeId = previousNode[ currentNodeId ];
      RamNodeSequence.nodeSequence[ RamNodeSequence.size ] = currentNodeId;
      RamNodeSequence.size++;
   }
   return distanceFromTarget[ sourceNodeId ];
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

void SetRoversPosition( inches_t x, inches_t y )
{
   // DEAL WITH ROVER IDENTIFICATION BETWEEN ROM AND RAM LISTS
  
   RamNodeCoordinateList[ 0 ].x = x;
   RamNodeCoordinateList[ 0 ].y = y;

   // RomNumberOfNodes == roverId
   UpdateSingleNodeConnections( RomNumberOfNodes );
}

coordinates_t GetRoversPosition()
{
   coordinates_t roversCoordinates = *GetNodeCoordinates( ROVERS_NODE_ID );
   return roversCoordinates;
}

void SetRoversBearing( degree_t degree )
{
   RoversBearing = degree;
}

degree_t GetRoversBearing()
{
   return RoversBearing;
}

nodeNumber_t GetRoversNodeId()
{
   return ROVERS_NODE_ID;
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
      else SetAdjacencyMatrixValue( node, otherNode, NO_CONNECTION );
   }
}

void UpdateAllNodeConnections()
{
   nodeNumber_t currentNode, otherNode;
   coordinates_t nodeCoords, otherNodeCoords;
   
   for ( currentNode = 0; currentNode < NumberOfNodes; currentNode++ )
   {
      for ( otherNode = currentNode + 1; otherNode < NumberOfNodes; otherNode++ )
      {
         if ( NodesAreVisibleToEachOther( currentNode, otherNode ) )
         {
            nodeCoords = currentNode < RomNumberOfNodes ? RomNodeCoordinateList[ currentNode ] : RamNodeCoordinateList[ currentNode - RomNumberOfNodes ];
            otherNodeCoords = otherNode < RomNumberOfNodes ? RomNodeCoordinateList[ otherNode ] : RamNodeCoordinateList[ otherNode - RomNumberOfNodes ];
            SetAdjacencyMatrixValue( currentNode, otherNode, Distance( nodeCoords, otherNodeCoords ) );
         }
         else
         {
            SetAdjacencyMatrixValue( currentNode, otherNode, NO_CONNECTION );
         }
      }
   }
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
   {
      if ( IntersectWithShape( ( tetragon_t* )&( RomObstacleList[ i ] ), segment ) ) 
      {
        return TRUE;
      }
   }
   for ( i = 0; i < RamNumberOfObstacles; i++ )
   {
      if ( IntersectWithShape( &( RamObstacleList[ i ] ), segment ) ) 
      {
        return TRUE;
      }
   }
   return FALSE;
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

static boolean_t IsAbove( segment_t* segment, coordinates_t* point )
{
   sWord BxAx, PyAy, ByAy, PxAx;
   sLWord BxAxPyAy, ByAyPxAx;
   BxAx = segment->rightPoint.x - segment->leftPoint.x;
   PyAy = point->y - segment->leftPoint.y;
   ByAy = segment->rightPoint.y - segment->leftPoint.y;
   PxAx = point->x - segment->leftPoint.x;
   _asm
   {
      LDD BxAx
      LDY PyAy
      EMULS
      LEAX BxAxPyAy
      STY 0,X
      STD 2,X
      LDD ByAy
      LDY PxAx
      EMULS
      LEAX ByAyPxAx
      STY 0,X
      STD 2,X
   }
   
   // FIGURE OUT CLEVER WAY TO COMPARE TWO 32 BIT NUMBERS!
   return BxAxPyAy > ByAyPxAx;
  
}

static void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value )
{
   if ( row == column )
   {
      return;
   }
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
   if ( row == column )
   {
      return 0;
   }
   if ( row < column )
   {   
      // swap because row must be greater than column
      row ^= column;
      column ^= row;
      row ^= column;
   }   
   return RamAdjacencyMatrixData[ ( ( row - 1 ) * row ) / 2 + column ];
}


/********************************************************************************/

void UpdateTurnByTurnQueueFromNodeSequence()
{
   nodeNumber_t i;
   coordinates_t* currentNode,* nextNode;
   static degree_t currentAngle = 90;
   RamTurnByTurnQueue.index = 0;
   RamTurnByTurnQueue.size = ( RamNodeSequence.size - 1 ) * 2;
   
   for ( i = 0; i < RamNodeSequence.size - 1; i++ )
   {
      currentNode = GetNodeCoordinates( RamNodeSequence.nodeSequence[ i ] );
      nextNode = GetNodeCoordinates( RamNodeSequence.nodeSequence[ i + 1 ] );
      currentAngle = currentAngle - arcTangent( nextNode->y - currentNode->y, nextNode->x - currentNode->x );
      if ( currentAngle < -180 ) currentAngle += 360;
      else if ( currentAngle > 180 ) currentAngle -= 360;
      RamTurnByTurnQueue.queue[ 2 * i ].typeOfMotion = ROTATE_MOTION;
      RamTurnByTurnQueue.queue[ 2 * i ].value = currentAngle; 
      
      RamTurnByTurnQueue.queue[ 2 * i + 1 ].typeOfMotion = FORWARD_MOTION;
      RamTurnByTurnQueue.queue[ 2 * i + 1 ].value = distanceFromTarget[ RamNodeSequence.nodeSequence[ i ] ] - distanceFromTarget[ RamNodeSequence.nodeSequence[ i + 1 ] ];
   }
}

coordinates_t* GetNodeCoordinates( nodeNumber_t nodeId )
{
   if ( nodeId < RomNumberOfNodes )
   {
      return &RomNodeCoordinateList[ nodeId ];
   }
   else if ( nodeId < RomNumberOfNodes + RamNumberOfNodes )
   {
      return &RamNodeCoordinateList[ nodeId - RomNumberOfNodes ];  
   }
   else
   {
      return 0x0000;
   }
}

turnByTurnElement_t * GetNextTurnByTurnElement()
{
   return &RamTurnByTurnQueue.queue[ RamTurnByTurnQueue.index++ ];
}

boolean_t HasNextTurnByTurnElement()
{
   return RamTurnByTurnQueue.index < RamTurnByTurnQueue.size;
}

nextState_t NavigateToTarget( nodeNumber_t runtimeTargetNodeIndex )
{
   Byte nodeSequenceIndex;
   coordinates_t approximateCoordinates, currentNodeCoords, nextNodeCoords;
   inches_t totalDistance, partialDistance;
   degree_t netDegrees, newBearing;
   
   Dijkstra( GetRoversNodeId(), GetClosestNodeForTarget( GetRoversNodeId(), runtimeTargetNodeIndex ) );
   approximateCoordinates = GetRoversPosition();
   nodeSequenceIndex = 0;
   //for ( nodeSequenceIndex = 0; nodeSequenceIndex < RamNodeSequence.size - 1; nodeSequenceIndex++ ) // WHILE ROVER IN MOTION? 
   {      
      currentNodeCoords = GetRoversPosition(); 
      nextNodeCoords = *GetNodeCoordinates( RamNodeSequence.nodeSequence[ nodeSequenceIndex + 1 ] );
      
      //EXECUTE ROTATION
      newBearing = ConvertUnitCircleAngleToCompassBearing( arcTangent( nextNodeCoords.y - currentNodeCoords.y, nextNodeCoords.x - currentNodeCoords.x ) );
      netDegrees = newBearing - GetRoversBearing(); 
      Rotate( netDegrees );
      SetRoversBearing( newBearing ); 
      //IF OBSTACLE DETECTED is FALSE
      
      //EXECUTE FORWARD MOTION
      partialDistance = 0;
      totalDistance = distanceFromTarget[ RamNodeSequence.nodeSequence[ nodeSequenceIndex ] ] - distanceFromTarget[ RamNodeSequence.nodeSequence[ nodeSequenceIndex + 1 ] ];	
      MoveForward( totalDistance );
      EnablePeriodicObstacleDetection();
      while( GetRoverInMotionFlag() && !GetObstacleDetectedFlag() )
      {
         partialDistance = GetDistanceIntoCurrentRoute();
         approximateCoordinates.x = currentNodeCoords.x + ( ( nextNodeCoords.x - currentNodeCoords.x ) * ( partialDistance / 15 ) ) / ( totalDistance / 15 );
         approximateCoordinates.y = currentNodeCoords.y + ( ( nextNodeCoords.y - currentNodeCoords.y ) * ( partialDistance / 15 ) ) / ( totalDistance / 15 );
      }
      DisablePeriodicObstacleDetection();
      if ( GetObstacleDetectedFlag() )
      {
         ClearObstacleDetectedFlag();
         SetRoversPosition( approximateCoordinates.x, approximateCoordinates.y );
         return SenseAndPlaceObstacle;
      }
      DetermineRoversPosition( approximateCoordinates );
   }
   if ( Distance( GetRoversPosition(), *GetNodeCoordinates( RamNodeSequence.nodeSequence[ RamNodeSequence.size - 1 ] ) ) > 5 )
   {
      return PursueTarget;
   }
   else
   {
      visitedTarget[ runtimeTargetNodeIndex ] = TRUE;
      return FindClosestTarget;
   }
}

nextState_t FindNextTarget( nodeNumber_t *pTarget )
{
   inches_t distance, shortestDistance;
   Byte i, closestTargetIndex;
   
   shortestDistance = MAX_SIGNED_16_BIT_VALUE;
   
   for ( i = 0; i < NUMBER_OF_TARGETS; i++ )
   {
      if ( visitedTarget[ i ] == FALSE )
      {
         distance = Dijkstra( GetRoversNodeId(), runtimeTarget[ i ].startingAccessPointNodeNumber );
         if ( distance < shortestDistance )
         {
            shortestDistance = distance;
            closestTargetIndex = i;
         }
      }
   }
   if ( shortestDistance == MAX_SIGNED_16_BIT_VALUE )
   {
      return TimeToCelebrate;
   }
   else
   {  
      *pTarget = closestTargetIndex;
      return PursueTarget;
   }
} 

nodeNumber_t GetClosestNodeForTarget( nodeNumber_t sourceNodeId, nodeNumber_t runtimeTargetIndex )
{
   nodeNumber_t i;
   inches_t closestDistance, tempDistance;
   nodeNumber_t closestNodeId;
   runtimeTarget_t target;
   
   target = runtimeTarget[ runtimeTargetIndex ];
   
   closestDistance = MAX_SIGNED_16_BIT_VALUE;
   closestNodeId = target.startingAccessPointNodeNumber; 
   
   for ( i = target.startingAccessPointNodeNumber; i < target.numberOfAccessPoints + target.startingAccessPointNodeNumber; i++ )
   {
      tempDistance = Dijkstra( sourceNodeId, i );
      if ( tempDistance < closestDistance )
      {
         closestDistance = tempDistance;
         closestNodeId = i;
      }
   }
   return closestNodeId;
}