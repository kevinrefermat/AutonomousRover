#include "wikiI2C.h"

// This was taken from the Wikipedia page for I2C protocol. It was
// modified by Kevin Refermat for a Senior Design capstone project
// at Loyola Marymount University in 2014.

// Hardware-specific support functions that MUST be customized:

static boolean_t started = False;

void arbitrationLost()
{
   TurnOnErrorLight();
}

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
   DDR_SCL = 0;
   return SCL;
}

boolean_t readSDA() // Set SDA as input and return current level of line, 0 or 1
{
   DDR_SDA = 0;
   return SDA;
}

void clearSCL() // Actively drive SCL signal low
{
   DDR_SCL = 1;
   SCL = 0;
}
void clearSDA() // Actively drive SDA signal low
{
   DDR_SDA = 1;
   SCL = 0;
}
 
void i2cStartCond()
{
   if ( started ) // if started, do a restart cond
   {
      readSDA(); // floating SDA means SDA is high
      i2cDelay();
      while ( readSCL() == 0 )
      {
         TurnOnErrorLight();
         // You should add timeout to this loop
      }
      // Repeated start setup time, minimum 4.7us
      i2cDelay();
   }
   if ( readSDA() == 0 )
   {
      arbitrationLost();
   }
  
   // SCL is high, set SDA from 1 to 0.
   clearSDA();
   i2cDelay();
   clearSCL();
   started = True;
}
 
void i2cStopCond()
{
   clearSDA();
   i2cDelay();
  
   while ( readSCL() == 0 )
   {
      TurnOnErrorLight();
      // add timeout to this loop.
   }
   // Stop bit setup time, minimum 4us
   i2cDelay();
   // SCL is high, set SDA from 0 to 1
   if ( readSDA() == 0 )
   {
      arbitrationLost();
   }
   i2cDelay();
   started = False;
}
 
void i2cWriteBit( boolean_t bit )
{
   if ( bit )
   {
      readSDA();
   }
   else
   {
      clearSDA();
   }
   i2cDelay();
   while ( readSCL() == 0 )
   {
      TurnOnErrorLight();
      // You should add timeout to this loop
   }
   // SCL is high, now data is valid
   // If SDA is high, check that nobody else is driving SDA
   if ( bit && readSDA() == 0 )
   {
      arbitrationLost();
   }
   i2cDelay();
   clearSCL();
}
 
boolean_t i2cReadBit()
{
   boolean_t bit;
   // Let the slave drive data
   readSDA();
   i2cDelay();
   while ( readSCL() == 0 )
   { 
      TurnOnErrorLight();
      // You should add timeout to this loop
   }
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
   i2cWriteBit( nack );
   if ( sendStop )
   {
      i2cStopCond();
   }
   return byte;
}
