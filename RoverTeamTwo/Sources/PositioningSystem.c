#include "Rover.h"
#include "PositioningSystem.h"
#include "Compass.h"
#include "Triangulation.h"
#include "MC9S12C128.h"


/*** CONSTANTS ***/

#define NUMBER_OF_MEASUREMENTS 5
static const inches_t MAX_DISTANCE_FROM_BEACON = 900;
static const inches_t MIN_DISTANCE_FROM_BEACON = CEILING_HEIGHT - 6;

static const TimeOutTime = MAX_UNSIGNED_16_BIT_VALUE;
static const pulseCount_t TransmittingOverhead = 8800;
static const inches_t WAITING_FOR_SIGNAL_TIMED_OUT = -1;
static const inches_t TRANSCEIVER_NOT_ACKOWLEDGING_TRANSMIT_REQUEST = -2;
static const inches_t UNINITIALIZED_DISTANCE = -3;
static const inches_t DISTANCE_OUT_OF_BOUNDS = -4;
static const inches_t TriangulationErrorThreshhold = 50;


/*** BEACON GROUP LUT ***/
#define NUMBER_OF_BEACON_GROUPS 16
static const Byte NoCoverageIndex = NUMBER_OF_BEACON_GROUPS - 1;

static const beaconGroup_t coverageZoneBeaconGroups[] =
{             
   { 0, 2, 3 }, //0
   { 0, 2, 1 }, //1
   { 0, 3, 4 }, //2
   { 0, 2, 4 }, //3
   { 1, 2, 4 }, //4
   { 2, 3, 4 }, //5
   { 0, 2, 4 }, //6
   { 1, 2, 3 }, //7
   { 0, 2, 3 }, //8
   { 0, 2, 4 }, //9
   { 1, 3, 4 }, //10
   { 1, 2, 4 }, //11
   { 0, 1, 3 }, //12
   { 1, 2, 3 }, //13
   { 1, 3, 4 }, //14
   { -1, -1, -1 } //No coverage
};

/*** STATIC VARIABLES ***/

static inches_t distanceMeasurements[ NUMBER_OF_MEASUREMENTS ];
static inches_t distanceToBeacon[ NUMBER_OF_BEACONS ];

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
   
}

inches_t GetAccurateLineOfSightDistanceToBeacon( beaconId_t beaconId )
{
   sByte i, j, k; 
   Byte timeOutCount;
   inches_t distance, temp;
   static const TimeOutThreshhold = 15;
   
   // reset distances
   
   for ( i = 0; i < NUMBER_OF_MEASUREMENTS; i++ )
   {
      distanceMeasurements[ i ] = UNINITIALIZED_DISTANCE;  
   }
   for ( i = 0, timeOutCount = 0; i < NUMBER_OF_MEASUREMENTS && timeOutCount < TimeOutThreshhold; timeOutCount++ )
   {
      //distanceMeasurements[ i ] = GetLineOfSightDistanceToBeacon( beaconId );
      distance = GetLineOfSightDistanceToBeacon( beaconId );
      if ( distance < 0 || distance < MIN_DISTANCE_FROM_BEACON || distance > MAX_DISTANCE_FROM_BEACON )
      {
         continue;
      }
      else
      {  
         distanceMeasurements[ i ] = distance;          
         i++;
      }
   }

   // find the median by sorting
   for ( j = 1; j < i; j++ )
   {
      for ( k = j; k > 0 && distanceMeasurements[ k ] < distanceMeasurements[ k - 1]; k-- )
      {
         temp = distanceMeasurements[ k ];
         distanceMeasurements[ k ] = distanceMeasurements[ k - 1 ];
         distanceMeasurements[ k - 1 ] = temp;    
      }
   }
   
   distance = distanceMeasurements[ NUMBER_OF_MEASUREMENTS / 2 ];
   
   return distance;
}

// Must disable interrupts and ensure that beacon is not transmitting from previous function call
// At thirty feet this function takes 90ms and times out around 170ms
inches_t GetLineOfSightDistanceToBeacon( beaconId_t beaconId )
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
   
   // After acknowledge disable                                                                                                              
   BEACON_TRANSMITTER_ENABLE = 0;
   
   startTimerCount = TCNT;
   success = waitForAndDetectReceivedSonarPulse();
   endTimerCount = TCNT;
   
   if ( success == FALSE )
   {
      return WAITING_FOR_SIGNAL_TIMED_OUT;
   }
   lengthOfSoundInTimerClockCycles = endTimerCount - startTimerCount - TransmittingOverhead;
   
   distance = lengthOfSoundInTimerClockCycles;
   //integer multiplication, distance = distance * 54/1000, avoiding overflow
   distance /= 10;
   distance *= 9;
   distance /= 10;
   distance *= 6;
   distance /= 10;
   
   if ( distance < MIN_DISTANCE_FROM_BEACON || distance > MAX_DISTANCE_FROM_BEACON )
   {
      return DISTANCE_OUT_OF_BOUNDS;
   }
   return distance;
}

// function blocks until it detect and returns TRUE if successful and FALSE if timed out
static boolean_t waitForAndDetectReceivedSonarPulse()
{
   Byte i, minNoiseLevel;
   Word timeOutCount;
   volatile Byte ATDReading;
   
   static pulseCount_t time;
   
   const Byte NumberOfSamplesAfterFirstDetection = 30;
   const Byte NumberOfNoSignalSamples = 2000;  // 1500 samples takes 2.6ms
   const Byte SignalThreshhold = 10;      // 10 = 200mV difference than noise
   const Word TimeOutThreshhold = 6000;  // 30 feet away takes 2500 iterations of the loop
   
   minNoiseLevel = 0xFF;
   
   Delay( 30 ); // sample noise right before the first possible signal could arrive
   
   // get noise threshold
   for ( i = 0; i < NumberOfNoSignalSamples; i++ )
   {
      ATDReading = ( volatile ) ATDDR0L;
      minNoiseLevel = minNoiseLevel > ATDReading ? ATDReading : minNoiseLevel;
   }
   minNoiseLevel -= SignalThreshhold;
   
   // Signal detection starts after the delay from Delay() and the noise sampling
   // so that Rover isn't looking for a signal that couldn't possible have gotten to
   // it yet. Each Beacon has approximately a 35ms turnaround time from receiving RF
   // to transmitting sonar.
   for ( timeOutCount = 0; timeOutCount < TimeOutThreshhold; timeOutCount++ )
   {
      if ( ATDDR0L < minNoiseLevel )
      {
         for ( i = 0; i < NumberOfSamplesAfterFirstDetection; i++ )
         {              
            if ( ATDDR0L < minNoiseLevel )
            {
               return TRUE;
            }
         }
      }
   }
   return FALSE;
   // repeatedly loop and check for signal that is significantly more powerful than
   // the noise and of some significant duration
}

void SetDistanceToBeacon( beaconId_t beaconId, inches_t distance )
{
   distanceToBeacon[ beaconId ] = distance;   
}

inches_t GetDistanceToBeacon( beaconId_t beaconId )
{
   return distanceToBeacon[ beaconId ];  
}


/*** DEAL WITH COMPASS PART OF THIS ***/
void DetermineRoversPosition( coordinates_t approximateCoordinates )
{        
   inches_t error;
   beaconId_t firstBeacon, secondBeacon, thirdBeacon;
   coordinates_t triangulationCoordinates;
   beaconGroup_t *pBeaconGroup;
   degree_t bearing;
   
   pBeaconGroup = GetBeaconGroup( approximateCoordinates );
   firstBeacon = pBeaconGroup->firstBeacon;
   secondBeacon = pBeaconGroup->secondBeacon;
   thirdBeacon = pBeaconGroup->thirdBeacon;
   
   distanceToBeacon[ firstBeacon ] = GetFloorDistance( GetAccurateLineOfSightDistanceToBeacon( firstBeacon ) );
   distanceToBeacon[ secondBeacon ] = GetFloorDistance( GetAccurateLineOfSightDistanceToBeacon( secondBeacon ) );
   distanceToBeacon[ thirdBeacon ] = GetFloorDistance( GetAccurateLineOfSightDistanceToBeacon( thirdBeacon ) );
   triangulationCoordinates = Triangulate( firstBeacon, secondBeacon, thirdBeacon );
   error = GetTriangulationError();
   
   if ( error > TriangulationErrorThreshhold )
   {
      /* guess using PACNT to determine rough position if
       * not at a node. If near a node then use the LUT
       */
      SetRoversPosition( approximateCoordinates );
   }
   else
   {
      SetRoversPosition( triangulationCoordinates );
   }
   bearing = GetAnAccurateCompassReading();
   
   if ( bearing >= 0 )
   {        
      SetRoversBearing( GetAnAccurateCompassReading() );
   }
   else
   {
      // calculate theoretical angle based on last angle
   }
}

beaconGroup_t * GetBeaconGroup( coordinates_t approximateCoordinates )
{
   Byte beaconGroupIndex; 
   inches_t x, y;
   beaconGroupIndex = NoCoverageIndex; //Default 
   x = approximateCoordinates.x;
   y = approximateCoordinates.y;
   
   // zone 0
   if ( x <= 138 && y <= 78 )
   {
      beaconGroupIndex = 0;     
   }
   // zone 1
   else if ( x <= 138 && y <= 150 && y >= 78 )
   {
      beaconGroupIndex = 1;       
   }
   // zone 2
   else if ( x <= 96 && y <= 246 && y >= 150 )
   {
      beaconGroupIndex = 2;
   }
   // zone 3
   else if ( x <= 60 && y >= 246 )
   {
      beaconGroupIndex = 3;  
   }
   // zone 4
   else if ( ( x <= 174 && x >= 120 && y <= 522 && y >= 456 ) ||
             ( x <= 120 && x >= 60 && y >= 174 && y <= 522 ) )
   {
      beaconGroupIndex = 4;  
   }
   // zone 5
   else if ( x <= 210 && x >= 174 && y >= 456 && y <= 522 )
   {
      beaconGroupIndex = 5;  
   }
   // zone 6
   else if ( x <= 246 && x >= 210 && y >= 456 && y <= 570 )
   {
      beaconGroupIndex = 6;  
   }
   // zone 7
   else if ( x <= 192 && x >= 156 && y >= 312 && y <= 456 )
   {
      beaconGroupIndex = 7;  
   }
   // zone 8
   else if ( ( x <= 276 && x >= 150 && y >= 210 && y <= 312 ) ||
             ( x <= 318 && x >= 150 && y >= 150 && y <= 210 ) ||
             ( x <= 318 && x >= 276 && y >= 210 && y <= ( -x + 528 ) ) )
   {
      beaconGroupIndex = 8;  
   }
   // zone 9
   else if ( x <= 288 && x >= 192 && y >= 312 && y <= 456 )
   {
      beaconGroupIndex = 9;  
   }
   // zone 10
   else if ( x >= 342 && y >= 516 )
   {
      beaconGroupIndex = 10;  
   } 
   // zone 11
   else if ( x <= 330 && x >= 288 && y >= 414 && y <= 432 )
   {
      beaconGroupIndex = 11;  
   }
   // zone 12
   else if ( x >= 318 && y <= 264 && y >= ( -x + 528 ) )
   {
      beaconGroupIndex = 12;  
   }
   // zone 13
   else if ( x >= 318 && y <= ( -x + 528 ) && y >= ( -x + 456 ) )
   {
      beaconGroupIndex = 13;  
   }
   // zone 14
   else if ( x >= 348 && y <= 468 && y >= ( -x + 756 ) )
   {
      beaconGroupIndex = 14;  
   }
   return &( coverageZoneBeaconGroups[ beaconGroupIndex ] );
}
