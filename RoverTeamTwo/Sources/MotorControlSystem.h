#ifndef _MOTOR_CONTROL_SYSTEM_H_
#define _MOTOR_CONTROL_SYSTEM_H_

#include "MC9S12C128.h"
#include "Rover.h"


/*** Function Prototypes ***/

void InitializeMotorControlSystem( void );

void CommenceTurnByTurnExecution( void );

void MoveForward( inches_t distance );
void MoveReverse( inches_t distance );
void Rotate( degree_t degrees );
void StopMotion( void );

boolean_t GetRoverInMotionFlag( void );


/*** Static Function Prototypes ***/

static void SetLeftTreadDrivePower( Byte power );
static void SetRightTreadDrivePower( Byte power );

static void EnableTreads( void );
static void DisableTreads( void );

static void LeftTreadForward( void );
static void LeftTreadReverse( void );

static void RightTreadForward( void );
static void RightTreadReverse( void );   

static void BrakeTreads( void );

void EnableTreadStabilization( void );
static void SetTreadStabilizationTimer( void );


static void InitializePulseAccumulator( pulseCount_t numberOfPulsesTillInterrupt );
static pulseCount_t DistanceToPulses( inches_t distance );
static pulseCount_t DegreesToPulses( degree_t degrees );

static boolean_t ExecuteNextTurnByTurnInstruction( void );

static void SetRoverInMotionFlag( void );
static void ClearRoverInMotionFlag( void );

static void AdjustLeftTreadDrivePower( sByte adjustment );
static void AdjustRightTreadDrivePower( sByte adjustment );


/*** Interrupt Service Routine ***/

interrupt VectorNumber_Vtimpaovf void MotionCompleted( void );

interrupt VectorNumber_Vtimch2 void PeriodicTreadStabilization( void );

#endif
