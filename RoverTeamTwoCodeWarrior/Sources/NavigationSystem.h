#ifndef _NAVIGATION_SYSTEM_H_
#define _NAVIGATION_SYSTEM_H_

#include "main.h"

void navigate( rotation_t * rotations, distance_t * distances, length_t size );

void determinePosition( gridUnits_t * x, gridUnits_t * y );

#endif