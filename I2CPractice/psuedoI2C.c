//set PWM clock to 100kHz 50% duty cycle

// Load Data Into Registers For Quick Access

PWM = 100; // SCL high
Data = input; // SDA floating pulled high

/**** START SIGNAL ****/

Data = output;
Data = 0; // SDA pulled low

/**** SEND BYTE ****/

PWM = 50; // SCL oscillates

while ( PWM is high ); // Detect falling edge
DataOut = Bit0FromRegister; // SDA transition

while ( PWM is high );
DataOut = Bit1FromRegister;

while ( PWM is high );
DataOut = Bit2FromRegister;

while ( PWM is high );
DataOut = Bit3FromRegister;

while ( PWM is high );
DataOut = Bit4FromRegister;

while ( PWM is high );
DataOut = Bit5FromRegister;

while ( PWM is high );
DataOut = Bit6FromRegister;

while ( PWM is high );
DataOut = Bit7FromRegister;

/**** WAIT FOR ACK ****/

Data = input;

void StartSignal()
{
   
}
