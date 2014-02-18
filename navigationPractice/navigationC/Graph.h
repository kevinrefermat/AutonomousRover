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

void AddEdge( graph_t* graph, graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance );
bool Dijkstra( graph_t* graph, graphSize_t sourceNodeId, graphSize_t targetNodeId );

bool intersectWithShape( tetragon_t* tetragon, segment_t* segment );

void AddObstacle( graph_t* graph, inches_t leftX, inches_t rightX, inches_t lowY, inches_t highY );
bool intersectWithObstacle( graph_t* graph, segment_t* segment );
void printVisibleNodes( coordinates_t* location );

#endif
