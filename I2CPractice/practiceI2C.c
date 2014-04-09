#define DDR_SCL DDRB_BIT0
#define SCL PORTB_BIT0
#define DDR_SDA DDRB_BIT1
#define SDA PORTB_BIT1

static const Byte SlaveReadAddress = 0x3D;
static const Byte SlaveWriteAddress = 0x3C;

// After StartSequence assume functions leave SCL high
void StartSequence()
{
   DDR_SCL = 1;
   SCL = 1;

   DDR_SDA = 0; // SDA is high
   SDA = 1;
   SDA = 0;
}

void StopSequence()
{
   SDA = 0;
   SDA = 1;
}

boolean_t SendByte( sByte data )
{
   int i;
   boolean_t ack;
   for ( i = 0; i < 8; i++ )
   {
      SCL = 0;
      SDA = ( data < 0 ); // SDA = most sig bit of data
      SCL = 1;
      data <<= 1; // bit shift data left to get next sig bit
   }
   SCL = 0;
   DDR_SDA = 0;
   SCL = 1;
   ack = SDA
   return ack;
}

sByte ReadByte()
{
         
}

