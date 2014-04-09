#include "I2C.h"
#include "Rover.h"


boolean_t InitializeCompass( void );
boolean_t GetDataFromCompass( void );

boolean_t writeByteToCompass( Byte register, Byte data );
Byte readByteFromCompass( boolean_t sendStop );


