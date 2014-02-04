#include "nodalNetworks.h"
#include "Rover.h"

typedef struct node
{
   gridUnit_t distanceFromSource;
   node* pointerToRoot;
   boolean_t hasBeenVisited;
   node** neighborNodes;
   gridUnit_t* distanceToNeighborNodes;
   unsigned char numberOfNeighbors;   
};

node * createNode( void )
{
   distanceFromSource = 0xFF ^ sizeof( gridUnit_t ) - 1;
   pointerToRoot = NULL;
   hasBeenVisited = False;
   neighborNodes = malloc( sizeof( node * ) * ARBITRARY_ARRAY_SIZE );
   distanceToNeighborNodes = malloc( sizeof( gridUnit_t ) * ARBITRARY_ARRAY_SIZE );
   numberOfNeighbors = 0;
}


