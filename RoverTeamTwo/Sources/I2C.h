#ifndef _I2C_H_
#define _I2C_H_

#include "Rover.h"

// This was taken from the Wikipedia page for I2C protocol. It was
// modified by Kevin Refermat for a Senior Design capstone project
// at Loyola Marymount University in 2014.

// Hardware-specific support functions that MUST be customized:

#define I2CSPEED 4
#define DDR_SCL DDRB_BIT0
#define SCL PORTB_BIT0
#define DDR_SDA DDRB_BIT1
#define SDA PORTB_BIT1

void arbitrationLost( void );
void i2CDelay( void );
boolean_t readSCL( void );   // Set SCL as input and return current level of line, 0 or 1
boolean_t readSDA();   // Set SDA as input and return current level of line, 0 or 1
void clearSCL();       // Actively drive SCL signal low
void clearSDA();       // Actively drive SDA signal low
void i2cStartCond();
void i2cStopCond();
void i2cWriteBit( boolean_t bit );
boolean_t i2cReadBit();

boolean_t I2CWriteByte( boolean_t sendStart, boolean_t sendStop, Byte byte );
Byte I2CReadByte( boolean_t nack, boolean_t sendStop );

#endif
