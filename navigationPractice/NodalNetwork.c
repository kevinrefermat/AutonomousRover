#include "nodalNetworks.h"
#include "Rover.h"

node * createNode( gridUnit_t x, gridUnit_t y )
{
   position.m_X = x;   
   position.m_Y = y;   
   distanceFromSource = 0xFF ^ sizeof( gridUnit_t ) - 1;
   pointerToRoot = NULL;
   hasBeenVisited = False;
   neighborNodes = malloc( sizeof( node * ) * ARBITRARY_ARRAY_SIZE );
   distanceToNeighborNodes = malloc( sizeof( gridUnit_t ) * ARBITRARY_ARRAY_SIZE );
   numberOfNeighbors = 0;
}
