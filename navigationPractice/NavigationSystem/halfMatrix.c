#include <stdio.h>
#include <stdint.h>

#define MAX_TOTAL_NUMBER_OF_NODES 5

typedef int8_t nodeNumber_t;
typedef int16_t inches_t;

static int8_t AdjacencyMatrixData[ MAX_TOTAL_NUMBER_OF_NODES * ( MAX_TOTAL_NUMBER_OF_NODES - 1 ) / 2 ];

void SetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column, inches_t value )
{
   if ( row == column ) return;
   if ( row < column )
   {
      // swap because row must be greater than column
      row ^= column;
      column ^= row;
      row ^= column;
   }
   AdjacencyMatrixData[ ( ( row - 1 ) * row ) / 2 + column ] = value;
}

inches_t GetAdjacencyMatrixValue( nodeNumber_t row, nodeNumber_t column )
{
   if ( row == column ) return 0;
   if ( row < column )
   {
      // swap because row must be greater than column
      row ^= column;
      column ^= row;
      row ^= column;
   }
   return AdjacencyMatrixData[ ( ( row - 1 ) * row ) / 2 + column ];
}

int main()
{
   SetAdjacencyMatrix( 0, 1, 10 );
   SetAdjacencyMatrix( 2, 0, 20 );
   SetAdjacencyMatrix( 1, 2, 21 );
   SetAdjacencyMatrix( 0, 3, 30 );
   SetAdjacencyMatrix( 1, 3, 31 );
   SetAdjacencyMatrix( 3, 2, 32 );
   SetAdjacencyMatrix( 4, 0, 40 );
   SetAdjacencyMatrix( 4, 1, 41 );
   SetAdjacencyMatrix( 2, 4, 42 );
   SetAdjacencyMatrix( 3, 4, 43 );


   printf( "%d\n", GetAdjacencyMatrix( 0, 1 ) );
   printf( "%d\n", GetAdjacencyMatrix( 2, 0 ) );
   printf( "%d\n", GetAdjacencyMatrix( 1, 2 ) );
   printf( "%d\n", GetAdjacencyMatrix( 0, 3 ) );
   printf( "%d\n", GetAdjacencyMatrix( 1, 3 ) );
   printf( "%d\n", GetAdjacencyMatrix( 3, 2 ) );
   printf( "%d\n", GetAdjacencyMatrix( 4, 0 ) );
   printf( "%d\n", GetAdjacencyMatrix( 4, 1 ) );
   printf( "%d\n", GetAdjacencyMatrix( 2, 4 ) );
   printf( "%d\n", GetAdjacencyMatrix( 3, 4 ) );
   printf( "size of is %d\n", sizeof( AdjacencyMatrixData ) );
}
