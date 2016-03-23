#ifndef _OBSTACLE_AVOIDANCE_SYSTEM_
#define _OBSTACLE_AVOIDANCE_SYSTEM_

#include "Rover.h"

void InitializeObstacleAvoidanceSystem( void );

void EnablePeriodicObstacleDetection( milliseconds_t period );
void DisablePeriodicObstacleDetection( void );

inches_t DetectClosestObstacle( void );
static void OutputPulseToPing( void );
static timerCount_t MeasureReturnPulseFromPing( void );

void SetPingRotationalPosition( degree_t degrees );          

static void SetPingTimer( void );

coordinates_t GetLeftEdgeOfObstacle( void );

boolean_t GetObstacleDetectedFlag( void );
void ClearObstacleDetectedFlag( void );

nextState_t DetectAndPlaceObstacle( void );

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles( void );

#endif
