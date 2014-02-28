#ifndef _GRID_H_
#define _GRID_H_

#include "main.h"

struct gridCell
{
   unsigned char m_IsOccupied  : 1;
   unsigned char m_Beacon1Mark : 1;
   unsigned char m_Beacon2Mark : 1;
   unsigned char m_Beacon3Mark : 1;
   unsigned char               : 4;
};

struct coordinates
{
   gridUnit_t m_X;
   gridUnit_t m_Y;
};

struct grid
{
   struct coordinates m_Size;
   struct coordinates m_Beacon1Coordinates;
   struct coordinates m_Beacon2Coordinates;
   struct coordinates m_Beacon3Coordinates;
   struct coordinates m_Target1Coordinates;
   struct coordinates m_Target2Coordinates;
   struct coordinates m_Target3Coordinates;
   struct coordinates m_RoverCoordinates;
   struct gridCell** m_Map;
};

struct grid* createEnvironment( gridUnit_t xSize, gridUnit_t ySize );

void destroyEnvironment( struct grid* environment );

void printGrid( struct grid* environment );

void addRectangle( struct grid* environment, gridUnit_t top, gridUnit_t bottom, gridUnit_t left, gridUnit_t right );

#endif
