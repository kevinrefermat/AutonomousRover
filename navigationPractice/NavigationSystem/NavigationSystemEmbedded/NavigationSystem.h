#ifndef _NAVIGATION_SYSTEM_H_
#define _NAVIGATION_SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>


/*** TYPE DEFINITIONS ***/

typedef int8_t nodeNumber_t;
typedef int8_t obstacleNumber_t;
typedef int16_t inches_t;

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

inches_t Dijkstra( nodeNumber_t sourceNodeId, nodeNumber_t targetNodeId );

void UpdateSingleNodeConnections( nodeNumber_t node );
void UpdateAllNodeConnections();
static inches_t Distance( coordinates_t A, coordinates_t B );
static inches_t SquareRoot( int32_t operand );

bool NodesAreVisibleToEachOther( nodeNumber_t nodeId1, nodeNumber_t nodeId2 );
static bool IntersectWithObstacle( segment_t* segment );
static bool IntersectWithShape( tetragon_t* tetragon, segment_t* segment );
static bool IntersectWithSegment( segment_t* segment0, segment_t* segment1 );
static bool IsAbove( segment_t* segment, coordinates_t* point );

static void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value );
static inches_t GetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column );

void printRamSize();

#endif
