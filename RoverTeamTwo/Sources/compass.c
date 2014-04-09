#include "I2C.h"
#include "Rover.h"
#include "Compass.h"

static const Byte SettingsCRA = 0x70; // 0x10 default
static const Byte SettingsCRB = 0x00; // 0x20 default
static const Byte SettingsMode = 0x00; // 0x00 default

static const Byte AddressCRA = 0x00;
static const Byte AddressCRB = 0x01;
static const Byte AddressMode = 0x02;

static const Byte SlaveWrite = 0x3C;
static const Byte SlaveRead = 0x3D;

static const boolean_t NACK = 1;

static const Byte MaxNumberOfFailedAttempts = 10;

static fullCompassRegister_t rawXData;
static fullCompassRegister_t rawYData;
static fullCompassRegister_t rawZData;


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
   if ( writeByteToCompass( 1, 0, SlaveWrite ) == NACK ) return False;
   if ( writeByteToCompass( 0, 0, AddressCRA ) == NACK ) return False;
   if ( writeByteToCompass( 0, 1, SettingsCRA ) == NACK ) return False;
   
   if ( writeByteToCompass( 1, 0, SlaveWrite ) == NACK ) return False;
   if ( writeByteToCompass( 0, 0, AddressCRB ) == NACK ) return False;
   if ( writeByteToCompass( 0, 1, SettingsCRB ) == NACK ) return False;
   
   if ( writeByteToCompass( 1, 0, SlaveWrite ) == NACK ) return False;
   if ( writeByteToCompass( 0, 0, AddressMode ) == NACK ) return False;
   if ( writeByteToCompass( 0, 1, SettingsMode ) == NACK ) return False;
   
   return True;
}

Byte readByteFromCompass( boolean_t sendStop )
{
   return I2CReadByte( 0, sendStop ); // send nack = 0 as the response byte
}

// returns 1 on success and 0 on failure
boolean_t GetDataFromCompass()
{
   if ( writeByteToCompass( 1, 0, SlaveRead ) == NACK ) return False;
   
   rawXData.bytes.upper = readByteFromCompass( 0 );
   rawXData.bytes.lower = readByteFromCompass( 0 );
   rawZData.bytes.upper = readByteFromCompass( 0 );
   rawZData.bytes.lower = readByteFromCompass( 0 );
   rawYData.bytes.upper = readByteFromCompass( 0 );
   rawYData.bytes.lower = readByteFromCompass( 1 );
   
   // Make sure register pointer is at first register by writing nothing to it
   if ( writeByteToCompass( 1, 0, SlaveWrite ) == NACK ) return False;
   if ( writeByteToCompass( 0, 1, 0x03 ) == NACK ) return False;
   
   return True;
}
