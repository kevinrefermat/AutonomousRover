#include "wikiI2C.h"
#include "compass.h"

static const Byte SettingsCRA = 0x70;
static const Byte SettingsCRB = 0xA0;
static const Byte SettingsMode = 0x00;

static const Byte AddressCRA = 0x00;
static const Byte AddressCRB = 0x01;
static const Byte AddressMode = 0x02;

static const Byte SlaveWrite = 0x3C;
static const Byte SlaveRead = 0x3D;

static const Byte MaxNumberOfFailedAttempts = 10;

static sWord rawXData;
static sWord rawYData;
static sWord rawZData;


boolean_t writeToCompass( boolean_t sendStart, boolean_t sendStop, Byte data )
{
   Byte numberOfFailedAttempts = 0;
   while ( I2CWriteByte( sendStart, sendStop, data ) == 0 )
   {  
      numberOfFailedAttempts++;
      if ( numberOfFailedAttempts == MaxNumberOfFailedAttempts )
      {
         TurnOnErrorLight();
         return False;
      }
   } 
   return True;
}

boolean_t InitializeCompass()
{
   if ( !writeToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeToCompass( 0, 0, AddressCRA ) ) return False;
   if ( !writeToCompass( 0, 1, SettingsCRA ) ) return False;
   
   if ( !writeToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeToCompass( 0, 0, AddressCRB ) ) return False;
   if ( !writeToCompass( 0, 1, SettingsCRB ) ) return False;
   
   if ( !writeToCompass( 1, 0, SlaveWrite ) ) return False;
   if ( !writeToCompass( 0, 0, AddressMode ) ) return False;
   if ( !writeToCompass( 0, 1, SettingsMode ) ) return False;
   return True;
}

boolean_t readFromCompass()
{
   
}
