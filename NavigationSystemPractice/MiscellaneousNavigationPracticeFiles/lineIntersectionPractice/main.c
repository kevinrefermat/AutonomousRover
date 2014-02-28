#include <stdio.h>
#include <stdlib.h>
#include "intersectionTest.h"

int main()
{
   tetragon* rectangle = malloc( sizeof( tetragon ) );
   rectangle->forwardDiagonal.leftPoint.x = 0;
   rectangle->forwardDiagonal.leftPoint.y = 0;
   rectangle->forwardDiagonal.rightPoint.x = 4;
   rectangle->forwardDiagonal.rightPoint.y = 2;
   rectangle->backDiagonal.leftPoint.x = 0;
   rectangle->backDiagonal.leftPoint.y = 2;
   rectangle->backDiagonal.rightPoint.x = 4;
   rectangle->backDiagonal.rightPoint.y = 0;

   segment* segment = malloc( sizeof( segment ) );
   segment->leftPoint.x = 4;
   segment->leftPoint.y = 2;
   segment->rightPoint.x = 7;
   segment->rightPoint.y = 8;

   printf( "%d\n", intersectWithShape( rectangle, segment ) );
   return 0;
}
