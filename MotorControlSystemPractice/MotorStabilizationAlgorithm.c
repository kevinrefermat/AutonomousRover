/*** USER DEFINED ***/

dt = 14286; // microseconds
DesiredRPM = 1000; // ???
ProportionalErrorGain = 1;
IntegralErrorGain = 1;
DerivativeErrorGain = 1;

/********************/


/*** ALGORITHM ***/

// declared when a motion has been initialized
IntegralError = 0;
PreviousError = 0;

// interrupt fired when pulse has been measured
void interruptServiceRoutine()
{
   CurrentRPM = GetCurrentRPM();
   Error = DesiredRPM - CurrentRPM;
   IntegralError += Error * dt;
   DerivativeError = ( Error - PreviousError ) / dt;
   Adjustment = ProportionalErrorGain * Error +
                IntegralErrorGain * IntegralError +
                DerivativeErrorGain * DerivativeError;
   PWMDutyCycle += Adjustment;
   ClearInterruptFlag();
}
