#include "I2C.h"
#include "Rover.h"


// This was taken from the Wikipedia page for I2C protocol. It was
// modified by Kevin Refermat for a Senior Design capstone project
// at Loyola Marymount University in 2014.

// Hardware-specific support functions that MUST be customized:

static boolean_t started = 0;
static Byte timer = 0;
static Byte TimeOutTime = 100;
                              

void i2cDelay()
{ 
   volatile sWord v; 
   sWord i; 
   for ( i = 0; i < I2CSPEED / 2; i++ ) 
   {
      v;
   }
}

boolean_t readSCL() // Set SCL as input and return current level of line, 0 or 1
{
   boolean_t value;
   _asm
   {
      LDAA  DDRB
      ANDA  #$FE   // set DDR_SCL to input
      STAA  DDRB 
      LDAA  PORTB 
      ANDA  #$01
      STAA  value
   }
   return value;
}

boolean_t readSDA() // Set SDA as input and return current level of line, 0 or 1
{
   boolean_t value;
   _asm
   {
      LDAA  DDRB
      ANDA  #$FD  // set DDR_SDA to input
      STAA  DDRB 
      LDAA  PORTB 
      ANDA  #$2
      LSRA
      STAA  value
   }
   return value;
}

void clearSCL() // Actively drive SCL signal low
{
   DDR_SCL = 1;
   SCL = 0;
}
void clearSDA() // Actively drive SDA signal low
{
   DDR_SDA = 1;
   SDA = 0;
}

void readSCL_AllowClockStretching()
{
   timer = 0;
   while ( readSCL() == 0 )
   {
      timer++;
      if ( timer >= TimeOutTime )
      {
         TurnOnErrorLight();
         break;
      }
   }
}
 
// SCL goes HIGH, SDA goes HIGH, SDA goes LOW, SCL goes LOW
void i2cStartCond()
{
   if ( started ) // if started, do a restart cond
   {
      readSDA();
      i2cDelay();
      
      readSCL_AllowClockStretching();
     
      // Repeated start setup time, minimum 4.7us
      i2cDelay();
   }
   
   readSCL_AllowClockStretching();
   i2cDelay();
   readSDA();
   // SCL is high, set SDA from 1 to 0.
   clearSDA();
   i2cDelay();
   clearSCL();
   started = True;
}
 
// SDA goes LOW, SCL Goes HIGH, SDA GOES HIGH
void i2cStopCond()
{

   clearSDA();
   i2cDelay();
  
   readSCL_AllowClockStretching();
   // Stop bit setup time, minimum 4us
   i2cDelay();
   
   // SCL is high, set SDA from 0 to 1
   readSDA();
   
   i2cDelay();
   started = False;
}
 
// put data on SDA, SCL goes HIGH, SCL goes LOW
void i2cWriteBit( boolean_t bit )
{
   // Assuming SCL is low
   if ( bit )
   {
      readSDA();
   }
   else
   {
      clearSDA();
   }
   i2cDelay();
   readSCL_AllowClockStretching();
   // SCL is high, now data is valid
   i2cDelay();
   clearSCL();
}
 
// set SDA to read, SCL goes HIGH, SCL goes LOW
boolean_t i2cReadBit()
{
   boolean_t bit;
   // Let the slave drive data
   readSDA();
   i2cDelay();
   
   readSCL_AllowClockStretching();
   i2cDelay();
   // SCL is high, now data is valid
   bit = readSDA();
   i2cDelay();
   clearSCL();
   return bit;
}
 
boolean_t I2CWriteByte( boolean_t sendStart, boolean_t sendStop, Byte byte )
{
   Byte bit;
   boolean_t nack;
   if ( sendStart )
   {
      i2cStartCond();
   }
   for ( bit = 0; bit < 8; bit++ )
   {
      i2cWriteBit( ( byte & 0x80 ) != 0 );
      byte <<= 1;
   }
   nack = i2cReadBit();
   if ( sendStop )
   {
      i2cStopCond();
   }
   return nack;
}
 
Byte I2CReadByte( boolean_t nack, boolean_t sendStop )
{
   Byte byte, bit;
   byte = 0;

   for ( bit = 0; bit < 8; bit++ )
   {
      byte = ( byte << 1 ) | i2cReadBit();
   }
   
   if ( sendStop )
   {
      i2cStopCond();
   }
   else
   {
      i2cWriteBit( nack );
   }
   return byte;
}
