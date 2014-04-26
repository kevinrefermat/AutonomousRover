#include "Rover.h"
#include "PositioningSystem.h"
#include "MC9S12C128.h"


static const TimeOutTime = MAX_16_BIT_VALUE;
static const pulseCount_t TransmittingOverhead = 0;
static const inches_t NO_SIGNAL_DETECTED = -1;
static const inches_t TRANSCEIVER_NOT_ACKOWLEDGING_TRANSMIT_REQUEST = -2;

// Could implement check at initialization to ensure that the beacons are correctly
// indexed
void InitializePositioningSystem()
{  
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
      return NO_SIGNAL_DETECTED;
   }
   lengthOfSoundInTimerClockCycles = ( endTimerCount - startTimerCount - TransmittingOverhead );
   distance = lengthOfSoundInTimerClockCycles;

   return distance;
}

// function blocks until it detect and returns TRUE if successful and FALSE if timed out
static boolean_t waitForAndDetectReceivedSonarPulse()
{
   Word noSignalLevel16;
   Byte i, noSignalLevel8, maxNoiseLevel, minNoiseLevel, ATDReading;
   Word timeOutCount;
   
   const Byte NumberOfNoSignalSamples = 100;
   const Byte SignalThreshhold = 5;
   const Word TimeOutThreshhold = 6000;  // 30 feet away takes 2500 iterations of the loop
   
   noSignalLevel16 = 0;
   maxNoiseLevel = 0xFF;
   minNoiseLevel = 0x00;

   // CHECK InitializePositioningSystem() for ATD settings
   
   // get noise threshold
   for ( i = 0; i < NumberOfNoSignalSamples; i++ )
   {
      ATDReading = ATDDR0L;
      while ( ATDSTAT1_CCF0 == 0 );
      maxNoiseLevel = maxNoiseLevel < ATDReading ? ATDReading : maxNoiseLevel;
      minNoiseLevel = minNoiseLevel > ATDReading ? ATDReading : minNoiseLevel;
   }
   maxNoiseLevel += SignalThreshhold;
   minNoiseLevel -= SignalThreshhold;

   //noSignalLevel16 /= NumberOfNoSignalSamples;
   //noSignalLevel8 = noSignalLevel16;
   
   for ( timeOutCount = 0; timeOutCount < TimeOutThreshhold; timeOutCount++; )
   {
      if ( ATDReading > maxNoiseLevel || ATDReading < minNoiseLevel )
      {
         return TRUE;
      }
   }
   return FALSE;
   // repeatedly loop and check for signal that is significantly more powerful than
   // the noise and of some significant duration
}

