#ifndef _NAVIGATION_SYSTEM_H_
#define _NAVIGATION_SYSTEM_H_

#include "MockRover.h"

/*** TYPE DEFINITIONS ***/

typedef int8_t nodeNumber_t;
typedef int8_t obstacleNumber_t;

typedef struct
{
   inches_t x;
   inches_t y;
} coordinates_t;

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


/*** FUNCTIONS ***/

void InitializeNavigationSystem();

void AddNode( inches_t x, inches_t y );
void AddObstacle( inches_t left, inches_t right, inches_t top, inches_t bottom );
void SetRoverPosition( inches_t x, inches_t y );

void Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId );

void UpdateSingleNodeConnections( nodeNumber_t node );
void UpdateAllNodeConnections();
static inches_t Distance( coordinates_t A, coordinates_t B );
static inches_t SquareRoot( sLWord operand );

boolean_t NodesAreVisibleToEachOther( nodeNumber_t nodeId1, nodeNumber_t nodeId2 );
static boolean_t IntersectWithObstacle( segment_t* segment );
static boolean_t IntersectWithShape( tetragon_t* tetragon, segment_t* segment );
static boolean_t IntersectWithSegment( segment_t* segment0, segment_t* segment1 );
static boolean_t IsAbove( segment_t* segment, coordinates_t* point );

static void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value );
static inches_t GetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column );

static void UpdateDegreesAndDistancesFromNodeSequence();

void printRamSize();
void printNodeSequence();

#endif
