#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "Rover.h"

typedef unsigned char coordinate_t;
typedef short connectionLogicalNumber_t;

typedef struct coordinatePair_t
{
    coordinate_t x;
    coordinate_t y;
};

typedef struct connection_t
{
   logicalValue_t nodeALogicalValue;
   logicalValue_t nodeBLogicalValue;
   logicalValue_t connectionLogicalValue;
   feet_t distanceFeet;
   boolean_t isCorrupted;
};

typedef struct connectionList_t
{
   connection_t * arrayOfConnections;
   connectionLogicalValue_t size;
};

typedef struct node_t
{
   connectionLogicalValue_t * listOfConnections;
   coordinatePair_t coordinates;
   boolean_t wasVisited;
};

feet_t 

#endif
