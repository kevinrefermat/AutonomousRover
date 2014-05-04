#include <hidef.h>
#include "derivative.h"
#include <libdefs.h>

#include "Rover.h"
#include "main.h"
#include "MotorControlSystem.h"
#include "NavigationSystem.h"
#include "ObstacleAvoidanceSystem.h"
#include "PositioningSystem.h"
#include "Math.h"
#include "Triangulation.h"


#include "Compass.h"
#include "I2C.h"

static inches_t distance0, distance1, distance3;
static coordinates_t myCoordinates;
static beaconGroup_t beaconGroup;
static nodeNumber_t closestNode;

static nodeNumber_t targetNodes[] = { 0, 1, 2 };

void main( void )
{  
   degree_t i;
   InitializeTimers();
   InitializePositioningSystem();
   InitializeObstacleAvoidanceSystem();
   InitializeNavigationSystem( &targetNodes );
   
   SetRoversPosition( 120, 120 );               
   closestNode = GetClosestNodeForTarget( 0 );
   myCoordinates = *GetNodeCoordinates( closestNode );
   closestNode = GetClosestNodeForTarget( 1 );   
   myCoordinates = *GetNodeCoordinates( closestNode );
   closestNode = GetClosestNodeForTarget( 2 ); 
   myCoordinates = *GetNodeCoordinates( closestNode );
   
   distance0 = Dijkstra( GetRoversNodeId(), 2 );
   
   for(;;);
}
