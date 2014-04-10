#ifndef _NAVIGATION_SYSTEM_H_
#define _NAVIGATION_SYSTEM_H_

#include "Rover.h"

/*** TYPE DEFINITIONS ***/

typedef struct
{
   coordinates_t leftPoint;
   coordinates_t rightPoint;
} segment_t;

typedef struct
{
   inches_t left;
   inches_t right;
   inches_t top;
   inches_t bottom;
} tetragon_t;

typedef struct
{
   direction_t typeOfMotion;
   sWord value;
} turnByTurnElement_t;


/*** FUNCTIONS ***/

void InitializeNavigationSystem( void );

void AddNode( inches_t x, inches_t y );
void AddObstacle( inches_t left, inches_t right, inches_t top, inches_t bottom );
void SetRoverPosition( inches_t x, inches_t y );

void Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId );

void UpdateSingleNodeConnections( nodeNumber_t node );
void UpdateAllNodeConnections( void );
static inches_t Distance( coordinates_t A, coordinates_t B );
static inches_t SquareRoot( sLWord operand );

boolean_t NodesAreVisibleToEachOther( nodeNumber_t nodeId1, nodeNumber_t nodeId2 );
static boolean_t IntersectWithObstacle( segment_t* segment );
static boolean_t IntersectWithShape( tetragon_t* tetragon, segment_t* segment );
static boolean_t IntersectWithSegment( segment_t* segment0, segment_t* segment1 );
static boolean_t IsAbove( segment_t* segment, coordinates_t* point );

static void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value );
static inches_t GetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column );

void UpdateTurnByTurnQueueFromNodeSequence( void );

coordinates_t* GetNodeCoordinates( nodeNumber_t nodeId );

turnByTurnElement_t* GetNextTurnByTurnElement( void );
boolean_t HasNextTurnByTurnElement( void );

#endif
