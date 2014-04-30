#include "Rover.h"
#include "PositioningSystem.h"
#include "MC9S12C128.h"

#define NUMBER_OF_MEASUREMENTS 5
static const inches_t MAX_DISTANCE_FROM_BEACON = 900;
static const inches_t MIN_DISTANCE_FROM_BEACON = 72;

static const TimeOutTime = MAX_16_BIT_VALUE;
static const pulseCount_t TransmittingOverhead = 8848;
static const inches_t WAITING_FOR_SIGNAL_TIMED_OUT = -1;
static const inches_t TRANSCEIVER_NOT_ACKOWLEDGING_TRANSMIT_REQUEST = -2;
static const inches_t UNINITIALIZED_DISTANCE = -3;

static distanceMeasurements[ NUMBER_OF_MEASUREMENTS ];

// Could implement check at initialization to ensure that the beacons are correctly
// indexed
void InitializePositioningSystem()
{  
   Byte i;
      // Setup ATD for Sonar detection
   ATDCTL2 = 0xC0; // fast flag clear
   ATDCTL3 = 0x0A;
   ATDCTL4 = 0x80; // speed/accuracy of conversion
   ATDCTL5 = 0xA0; // 10100000
                               
   BEACON_TRANSMITTER_ENABLE_DDR = OUTPUT;
   BEACON_TRANSMITTER_SIGNAL_0_DDR = OUTPUT;
   BEACON_TRANSMITTER_SIGNAL_1_DDR = OUTPUT;
   BEACON_TRANSMITTER_SIGNAL_2_DDR = OUTPUT;
   BEACON_TRANSMITTER_ACKNOWLEDGE_PIN_DDR = INPUT;
   
   BEACON_TRANSMITTER_ENABLE = 0;
   
   for ( i = 0; i < NUMBER_OF_MEASUREMENTS; i++ )
   {
      distanceMeasurements[ i ] = UNINITIALIZED_DISTANCE;
   }
}

inches_t GetAccurateDistanceToBeacon( beaconId_t beaconId )
{
   sByte i, k; 
   Byte timeOutCount;
   inches_t distance, temp;
   static const TimeOutThreshhold = 15;
   for ( i = 0, timeOutCount = 0; i < NUMBER_OF_MEASUREMENTS && timeOutCount < TimeOutThreshhold; timeOutCount++ )
   {
      distanceMeasurements[ i ] = GetDistanceToBeacon( beaconId );
      if ( distanceMeasurements[ i ] < 0 || distanceMeasurements[ i ] < MIN_DISTANCE_FROM_BEACON || distanceMeasurements[ i ] > MAX_DISTANCE_FROM_BEACON )
      {
         continue;
      }
      i++;
   }

   // find the median by sorting
   for ( i = 1; i < NUMBER_OF_MEASUREMENTS; i++ )
   {
      for ( k = i; k > 0 && distanceMeasurements[ k ] < distanceMeasurements[ k - 1]; k-- )
      {
         temp = distanceMeasurements[ k ];
         distanceMeasurements[ k ] = distanceMeasurements[ k - 1 ];
         distanceMeasurements[ k - 1 ] = temp;    
      }
   }
   
   distance = distanceMeasurements[ NUMBER_OF_MEASUREMENTS / 2 ];
   for ( i = 0; i < NUMBER_OF_MEASUREMENTS; i++ )
   {
      distanceMeasurements[ i ] = UNINITIALIZED_DISTANCE;  
   }
   return distance;
}

// Must disable interrupts and ensure that beacon is not transmitting from previous function call
// At thirty feet this function takes 90ms and times out around 170ms
inches_t GetDistanceToBeacon( beaconId_t beaconId )
{
   boolean_t success;
   timerCount_t startTimerCount, endTimerCount, lengthOfSoundInTimerClockCycles;
   inches_t distance;
   Word timeOutCount;
   const Word TimeOutThreshhold = 7000; // My only test said that it normally takes 3700 counts

   timeOutCount = 0;

   switch ( beaconId )
   {
      case 0:
        BEACON_TRANSMITTER_SIGNAL_0 = 0;
        BEACON_TRANSMITTER_SIGNAL_1 = 0;
        BEACON_TRANSMITTER_SIGNAL_2 = 0;
        break;
      case 1:
        BEACON_TRANSMITTER_SIGNAL_0 = 1;
        BEACON_TRANSMITTER_SIGNAL_1 = 0;
        BEACON_TRANSMITTER_SIGNAL_2 = 0;
        break;
      case 2:
        BEACON_TRANSMITTER_SIGNAL_0 = 0;
        BEACON_TRANSMITTER_SIGNAL_1 = 1;
        BEACON_TRANSMITTER_SIGNAL_2 = 0;
        break;
      case 3:
        BEACON_TRANSMITTER_SIGNAL_0 = 1;
        BEACON_TRANSMITTER_SIGNAL_1 = 1;
        BEACON_TRANSMITTER_SIGNAL_2 = 0;
        break;
      case 4:
        BEACON_TRANSMITTER_SIGNAL_0 = 0;
        BEACON_TRANSMITTER_SIGNAL_1 = 0;
        BEACON_TRANSMITTER_SIGNAL_2 = 1;
        break;
   }
   
   // Beacon executes start signal on rising edge of its enable pin
   // BS2 based transmitter has ~4000 to ~12000 instructions per seconds. Must have
   // delay to guarantee it sees the rising edge. Implement feedback from transmitter
   // so freescale can accurately start the timer

   BEACON_TRANSMITTER_ENABLE = 1;

   // ADD TIMEOUT 
   while ( BEACON_TRANSMITTER_ACKNOWLEDGE_PIN == 0 )
   {
      timeOutCount++;
      if ( timeOutCount > TimeOutThreshhold )
      {
         BEACON_TRANSMITTER_ENABLE = 0;
         return TRANSCEIVER_NOT_ACKOWLEDGING_TRANSMIT_REQUEST;
      }
   }
   startTimerCount = TCNT;
   
   // After acknowledge disable
   BEACON_TRANSMITTER_ENABLE = 0;

   success = waitForAndDetectReceivedSonarPulse();
   endTimerCount = TCNT;
   if ( success == FALSE )
   {
      return WAITING_FOR_SIGNAL_TIMED_OUT;
   }
   lengthOfSoundInTimerClockCycles = ( endTimerCount - startTimerCount - TransmittingOverhead );
   distance = lengthOfSoundInTimerClockCycles;
   //integer multiplication, distance = distance * 54/1000, avoiding overflow
   distance /= 10;
   distance *= 9;
   distance /= 10;
   distance *= 6;
   distance /= 10;

   return distance;
}

// function blocks until it detect and returns TRUE if successful and FALSE if timed out
static boolean_t waitForAndDetectReceivedSonarPulse()
{
   Word noSignalLevel16;
   Byte i, noSignalLevel8, maxNoiseLevel, minNoiseLevel, ATDReading;
   Word timeOutCount;
   
   static pulseCount_t time;
   
   const Byte NumberOfNoSignalSamples = 1000;  // 1000 samples takes 4.5ms
   const Byte SignalThreshhold = 10;      // 10 = 200mV difference than noise
   const Word TimeOutThreshhold = 6000;  // 30 feet away takes 2500 iterations of the loop
   
   noSignalLevel16 = 0;
   maxNoiseLevel = 0x00;
   minNoiseLevel = 0xFF;

   Delay( 30 ); // sample noise right before the first possible signal could arrive
   // get noise threshold
   for ( i = 0; i < NumberOfNoSignalSamples; i++ )
   {
      //while ( ATDSTAT1_CCF0 == 0 );
      maxNoiseLevel = maxNoiseLevel < ATDDR0L ? ATDDR0L : maxNoiseLevel;
      minNoiseLevel = minNoiseLevel > ATDDR0L ? ATDDR0L : minNoiseLevel;
   }
   maxNoiseLevel += SignalThreshhold;
   minNoiseLevel -= SignalThreshhold;
   
   // Signal detection starts after the delay from Delay() and the noise sampling
   // so that Rover isn't looking for a signal that couldn't possible have gotten to
   // it yet. Each Beacon has approximately a 35ms turnaround time from receiving RF
   // to transmitting sonar.
   for ( timeOutCount = 0; timeOutCount < TimeOutThreshhold; timeOutCount++ )
   {
      //ATDReading = ATDDR0L;
      if ( ATDDR0L > maxNoiseLevel )
      {
         return TRUE;
      }
   }
   return FALSE;
   // repeatedly loop and check for signal that is significantly more powerful than
   // the noise and of some significant duration
}

