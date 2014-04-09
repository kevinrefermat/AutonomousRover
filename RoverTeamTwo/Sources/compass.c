#include "I2C.h"
#include "Rover.h"
#include "Compass.h"

static const Byte SettingsCRA = 0x70;
static const Byte SettingsCRB = 0xA0;
static const Byte SettingsMode = 0x00;

static const Byte AddressCRA = 0x00;
static const Byte AddressCRB = 0x01;
static const Byte AddressMode = 0x02;

static const Byte SlaveWrite = 0x3C;
static const Byte SlaveRead = 0x3D;

static const Byte MaxNumberOfFailedAttempts = 10;

static Byte rawXLData;
static Byte rawYLData;
static Byte rawZLData;
static Byte rawXUData;
static Byte rawYUData;
static Byte rawZUData;


boolean_t writeByteToCompass( boolean_t sendStart, boolean_t sendStop, Byte data )
{
   Byte numberOfFailedAttempts = 0;
   while ( I2CWriteByte( sendStart, sendStop, data ) == 1 ) // I2CWriteByte returned nack = 1
   {  
      numberOfFailedAttempts++;
      if ( numberOfFailedAttempts == MaxNumberOfFailedAttempts )
      {
         TurnOnErrorLight();
         return 1; // nack = 1
      }
   } 
   return 0; // nack = 0
}

boolean_t InitializeCompass()
{
   if ( !writeByteToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeByteToCompass( 0, 0, AddressCRA ) ) return False;
   if ( !writeByteToCompass( 0, 1, SettingsCRA ) ) return False;
   
   if ( !writeByteToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeByteToCompass( 0, 0, AddressCRB ) ) return False;
   if ( !writeByteToCompass( 0, 1, SettingsCRB ) ) return False;
   
   if ( !writeByteToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeByteToCompass( 0, 0, AddressMode ) ) return False;
   if ( !writeByteToCompass( 0, 1, SettingsMode ) ) return False;
   return True;
}

Byte readByteFromCompass( boolean_t sendStop )
{
   return I2CReadByte( 0, sendStop ); // send nack = 0 as the response byte
}

// returns 1 on success and 0 on failure
boolean_t GetDataFromCompass()
{
   // Make sure register pointer is at first register by writing nothing to it
   if ( writeByteToCompass( 1, 0, SlaveWrite ) == 1 ) return False;
   if ( writeByteToCompass( 0, 1, 0x03 ) == 1 ) return False;
   
   //
   if ( writeByteToCompass( 1, 0, SlaveRead ) == 1 ) return False;
   rawXUData = readByteFromCompass( 0 );
   rawXLData = readByteFromCompass( 0 );
   rawZUData = readByteFromCompass( 0 );
   rawZLData = readByteFromCompass( 0 );
   rawYUData = readByteFromCompass( 0 );
   rawYLData = readByteFromCompass( 1 );
   return True;
}
