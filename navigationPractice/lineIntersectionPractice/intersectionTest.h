#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef int16_t inches_t;

typedef struct
{
   inches_t x;
   inches_t y;
} coordinates;

typedef struct
{
   coordinates leftPoint;
   coordinates rightPoint;
} segment;

// could be optimized to take advantage of common x and y values
typedef struct
{
   segment backDiagonal;
   segment forwardDiagonal;
} tetragon;


bool intersectWithShape( tetragon* tetragon, segment* segment );

bool intersectWithSegment( segment* segment0, segment* segment1 ); 
