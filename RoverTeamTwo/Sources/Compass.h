#include "I2C.h"
#include "Rover.h"


boolean_t InitializeCompass( void );
boolean_t GetDataFromCompass( void );

boolean_t writeByteToCompass( Byte register, Byte data );
Byte readByteFromCompass( boolean_t sendStop );

typedef union
{
   sWord value;
   struct compassRegisters
   {
      Byte upper;
      Byte lower;
   } bytes;
} fullCompassRegister_t;