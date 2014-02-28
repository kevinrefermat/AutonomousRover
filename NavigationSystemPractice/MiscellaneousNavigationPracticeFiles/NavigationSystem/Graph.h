#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include <stdbool.h>


/*** TYPE DEFINITIONS ***/

typedef int16_t graphSize_t;
typedef int16_t inches_t;
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
   segment_t backDiagonal;
   segment_t forwardDiagonal;
} tetragon_t;

typedef struct
{
   graphSize_t m_NumberOfNodes;
   coordinates_t* m_NodeCoordinateList;
   inches_t** m_AdjacencyMatrix;
   obstacleNumber_t m_NumberOfObstacles;
   tetragon_t* m_ObstacleList;
} graph_t;


/*** FUNCTIONS ***/

graph_t* CreateGraph( graphSize_t numberOfNodes );
graph_t* CreateGraphEELab();
void DestroyGraph( graph_t* graph );

void SetNodeCoordinates( graph_t* graph, graphSize_t nodeId, inches_t x, inches_t y );
void AddObstacle( graph_t* graph, inches_t leftX, inches_t rightX, inches_t lowY, inches_t highY );
void SetRoverPosition( graph_t* graph, inches_t x, inches_t y );

inches_t Dijkstra( graph_t* graph, graphSize_t sourceNodeId, graphSize_t targetNodeId );

void UpdateNodeVisibilityAndDistances( graph_t* graph );
inline inches_t Distance( coordinates_t A, coordinates_t B );
inline inches_t SquareRoot( int32_t operand );

bool NodesAreVisibleToEachOther( graph_t* graph, graphSize_t nodeId1, graphSize_t nodeId2 );
inline bool IntersectWithObstacle( graph_t* graph, segment_t* segment );
inline bool IntersectWithShape( tetragon_t* tetragon, segment_t* segment );
inline bool IntersectWithSegment( segment_t* segment0, segment_t* segment1 );
inline bool IsAbove( segment_t* segment, coordinates_t* point );

#endif
