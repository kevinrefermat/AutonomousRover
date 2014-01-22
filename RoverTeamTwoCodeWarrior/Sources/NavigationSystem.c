#include "NavigationSystem.h"

void navigate( rotation_t * rotations, distance_t * distances, length_t size )
{
   length_t i;
	for ( i = 0; i < size; i++ )
	{
		rotate( rotations[ i ] );
		moveForward( distances[ i ] );
		// wait until PAOVI which indicates that the rover has finished moving forward
		// and can then proceed to next direction
	}
}

void determinePosition( int x, int y )
{

}
