#include "Rover.h"

void InitializePeriodicObjectDetection( void );

inches_t DetectClosestObstacle( void );

static void OutputPulseToPing( void );

static timerCount_t MeasureReturnPulseFromPing( void );

void SetPingRotationalPosition( degree_t degrees );          

static registerValue_t DegreesToClockCycles( degree_t degrees );

void CheckForObstacles( void );

static void UpdatePingDelay( void );

interrupt VectorNumber_Vtimch1 void PeriodicCheckForObstacles( void );
