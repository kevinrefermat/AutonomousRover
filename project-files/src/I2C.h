#ifndef _I2C_H_
#define _I2C_H_

#include "Rover.h"

// This was taken from the Wikipedia page for I2C protocol. It was
// modified by Kevin Refermat for a Senior Design capstone project
// at Loyola Marymount University in 2014.

// Hardware-specific support functions that MUST be customized:

#define I2CSPEED 10

boolean_t I2CWriteByte( boolean_t sendStart, boolean_t sendStop, Byte byte );
Byte I2CReadByte( boolean_t nack, boolean_t sendStop );

void i2CDelay( void );
boolean_t readSCL( void );          // Set SCL as input and return current level of line, 0 or 1
boolean_t readSDA( void );          // Set SDA as input and return current level of line, 0 or 1
void clearSCL( void );              // Actively drive SCL signal low
void clearSDA( void );              // Actively drive SDA signal low
void i2cStartCond( void );
void i2cStopCond( void );
void i2cWriteBit( boolean_t bit );
boolean_t i2cReadBit( void );

#endif
