#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "intersectionTest.h"

inline int32_t isAbove( segment* segment, coordinates* point )
{
   return ( segment->rightPoint.x - segment->leftPoint.x ) * 
          ( point->y - segment->leftPoint.y ) - 
          ( segment->rightPoint.y - segment->leftPoint.y ) *
          ( point->x - segment->leftPoint.x );
}

inline bool intersectWithSegment( segment* segment0, segment* segment1 )
{
   return ( isAbove( segment0, &( segment1->leftPoint ) ) * isAbove( segment0, &( segment1->rightPoint ) ) < 0 ) &&
          ( isAbove( segment1, &( segment0->leftPoint ) ) * isAbove( segment1, &( segment0->rightPoint ) ) < 0 );
}

bool intersectWithShape( tetragon* tetragon, segment* segment )
{
   return intersectWithSegment( &( tetragon->forwardDiagonal ), segment ) ||
          intersectWithSegment( &( tetragon->backDiagonal ), segment );
}  
