#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "grid.h"

struct grid* createEnvironment( gridUnit_t xSize, gridUnit_t ySize )
{
   struct grid* environment;
   environment = malloc( sizeof( struct grid ) );
   
   environment->m_Size.m_X = xSize;
   environment->m_Size.m_Y = ySize;
   
   environment->m_RoverCoordinates.m_X = 0;
   environment->m_RoverCoordinates.m_Y = 0;
   
   environment->m_Beacon1Coordinates.m_X = 0;
   environment->m_Beacon1Coordinates.m_Y = 0;
   
   environment->m_Beacon2Coordinates.m_X = 0;
   environment->m_Beacon2Coordinates.m_Y = 0;
  
   environment->m_Beacon3Coordinates.m_X = 0;
   environment->m_Beacon3Coordinates.m_Y = 0;
  
   environment->m_Map = malloc( sizeof( struct gridCell* ) * environment->m_Size.m_X );
   
   gridUnit_t x, y;
   for ( x = 0; x < environment->m_Size.m_X; x++ )
   {
      environment->m_Map[ x ] = malloc( environment->m_Size.m_Y * sizeof( struct gridCell ) );
   }
   for ( x = 0; x < environment->m_Size.m_X; x++ )
   {
      for ( y = 0; y < environment->m_Size.m_Y; y++ )
      {
         environment->m_Map[ x ][ y ].m_IsOccupied = false;
      }
   }
   return environment;  
}

void destroyEnvironment( struct grid* environment )
{  
   gridUnit_t x;
   for ( x = 0; x < environment->m_Size.m_X; x++ )
   {
      free( environment->m_Map[ x ] );
   }
   free( environment->m_Map );
   free( environment );
}

void printGrid( struct grid* environment )
{
   gridUnit_t x, y;
   printf( "\n" );
   for ( y = 0; y < environment->m_Size.m_Y; y++ )
   {
      printf( "|" );
      for ( x = 0; x < environment->m_Size.m_X; x++ )
      {
         if ( environment->m_Map[ environment->m_Size.m_X - x - 1][ environment->m_Size.m_Y - y - 1 ].m_IsOccupied == false )
            printf( ".." );
         else
            printf( "HH" );
      }
      printf( "|\n" );
   }
   printf( "\n" );
}

void addRectangle( struct grid* environment, gridUnit_t top, gridUnit_t bottom, gridUnit_t left, gridUnit_t right )
{
   gridUnit_t x, y;
   for ( y = bottom; y <= top; y++ )
   {
      for ( x = left; x <= right; x++ )
      {
         environment->m_Map[ x ][ y ].m_IsOccupied = true;  
      }
   }
}
