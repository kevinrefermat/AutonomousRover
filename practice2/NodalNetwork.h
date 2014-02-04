#ifndef _NODAL_NETWORK_H_
#define _NODAL_NETWORK_H_

#include "Rover.h"

static unsigned char ARBITRARY_ARRAY_SIZE = 30;

typedef struct node
{
   coordinates position;
   gridUnit_t distanceFromSource;
   node* pointerToRoot;
   boolean_t hasBeenVisited;
   node** neighborNodes;
   gridUnit_t* distanceToNeighborNodes;
   unsigned char numberOfNeighbors;   
};

node * createNode( gridUnit_t x, gridUnit_t y );

add
#endif
