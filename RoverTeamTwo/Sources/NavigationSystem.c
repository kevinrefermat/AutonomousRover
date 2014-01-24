#include "NavigationSystem.h"

void navigate( signed char * rotations, unsigned char * distances, unsigned char size )
{
	for ( unsigned char i = 0; i < size; i++ )
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
