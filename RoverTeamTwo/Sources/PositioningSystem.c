#include "Rover.h"
#include "PositioningSystem.h"
#include "MC9S12C128.h"


static const TimeOutTime = MAX_16_BIT_VALUE;
static const TransmittingOverhead = 10000;

// Could implement check at initialization to ensure that the beacons are correctly
// indexed
void InitializePositioningSystem()
{
   BEACON_TRANSMITTER_ENABLE_DDR = OUTPUT;
   BEACON_TRANSMITTER_SIGNAL_0_DDR = OUTPUT;
   BEACON_TRANSMITTER_SIGNAL_1_DDR = OUTPUT;
   BEACON_TRANSMITTER_ACKNOWLEDGE_PIN_DDR = INPUT;
   
   BEACON_TRANSMITTER_ENABLE = 0;
}



// Must disable interrupts and ensure that beacon is not transmitting from previous function call
inches_t GetDistanceToBeacon( beaconId_t beaconId )
{
   boolean_t success;
   timerCount_t startTimerCount, endTimerCount, lengthOfSoundInTimerClockCycles;
   inches_t distance;

   switch ( beaconId )
   {
      case 0:
        BEACON_TRANSMITTER_SIGNAL_0 = 0;
        BEACON_TRANSMITTER_SIGNAL_1 = 0;
        break;
      case 1:
        BEACON_TRANSMITTER_SIGNAL_0 = 1;
        BEACON_TRANSMITTER_SIGNAL_1 = 0;
        break;
      case 2:
        BEACON_TRANSMITTER_SIGNAL_0 = 0;
        BEACON_TRANSMITTER_SIGNAL_1 = 1;
        break;
      case 3:
        BEACON_TRANSMITTER_SIGNAL_0 = 1;
        BEACON_TRANSMITTER_SIGNAL_1 = 1;
        break;
   }
   
   // Beacon executes start signal on rising edge of its enable pin
   // BS2 based transmitter has ~4000 to ~12000 instructions per seconds. Must have
   // delay to guarantee it sees the rising edge. Implement feedback from transmitter
   // so freescale can accurately start the timer

   BEACON_TRANSMITTER_ENABLE = 1;

   // ADD TIMEOUT 
   while ( BEACON_TRANSMITTER_ACKNOWLEDGE_PIN == 0 );
   startTimerCount = TCNT;
   
   // After acknowledge disable
   BEACON_TRANSMITTER_ENABLE = 0;

   success = waitForAndDetectReceivedSonarPulse();
   endTimerCount = TCNT;
   if ( success == FALSE )
   {
      return -1;
   }
   lengthOfSoundInTimerClockCycles = ( endTimerCount - startTimerCount - TransmittingOverhead );
   distance = lengthOfSoundInTimerClockCycles;

   return distance;
}

// function blocks until it detect and returns TRUE if successful and FALSE if timed out
static boolean_t waitForAndDetectReceivedSonarPulse()
{
   ATDCTL2 = 0xC0; // fast flag clear
   ATDCTL3 = 0x0A;
   ATDCTL4 = 0x80; // speed/accuracy of conversion
   ATDCTL5 = 0x20; // 00100000


   // get noise threshold
   // add time out structure
   // repeatedly loop and check for signal that is significantly more powerful than
   // the noise and of some significant duration
}

