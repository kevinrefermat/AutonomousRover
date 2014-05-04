#ifndef _ROVER_H_
#define _ROVER_H_

#include "MC9S12C128.h"
#include <stdtypes.h>

/*** TYPE DECLARATIONS ***/

typedef Byte boolean_t;

typedef Word milliseconds_t;
typedef Word microseconds_t;
typedef Word timerCount_t;

typedef Word pulseCount_t;
typedef sWord degree_t;
typedef Byte direction_t;

typedef Word inches_t; 
typedef Byte feet_t;

typedef sByte beaconId_t;

typedef sByte nodeNumber_t;
typedef sByte obstacleNumber_t;

typedef struct
{
   inches_t x;
   inches_t y;
} coordinates_t;


/*** ROVER IO MAP ***/

// TIMER 0 = Ping sensor
// TIMER 1 = Periodic obstacle detection
// TIMER 2 = TreadStabilization left or right (not sure which)
// TIMER 3 = TreadStabilization right or left (not sure which)
// TIMER 4 = Positioning system sound measurement
// TIMER 7 = pulse accumulator to calculate rover's distance

// PWM0 = Ping sensor servo signal upper byte
// PWM1 = Ping sensor servo signal lower byte
// PWM2 = left tread motor driver enable
// PWM3 = right tread motor drive enable

// PORTA_BIT0 = Motor driver left input 0 
// PORTA_BIT1 = Motor driver left input 1
// PORTA_BIT2 = Motor driver right input 0
// PORTA_BIT3 = Motor driver right input 1


// PORTB_BIT0 = Compass module SCL line
// PORTB_BIT1 = Compass module SDA line
// PORTB_BIT2 = Beacon transmitter enable
// PORTB_BIT3 = Beacon acknowledge pin
// PORTB_BIT4 = Beacon signal pin 0
// PORTB_BIT5 = Beacon signal pin 1
// PORTB_BIT6 = Beacon signal pin 2
// PORTB_BIT7 = LED error light on microcontroller

#define MOTOR_DRIVE_LEFT_IN_0 PORTA_BIT0
#define MOTOR_DRIVE_LEFT_IN_0_DDR DDRA_BIT0
#define MOTOR_DRIVE_LEFT_IN_1 PORTA_BIT1
#define MOTOR_DRIVE_LEFT_IN_1_DDR DDRA_BIT1
#define MOTOR_DRIVE_RIGHT_IN_0 PORTA_BIT2
#define MOTOR_DRIVE_RIGHT_IN_0_DDR DDRA_BIT2
#define MOTOR_DRIVE_RIGHT_IN_1 PORTA_BIT3
#define MOTOR_DRIVE_RIGHT_IN_1_DDR DDRA_BIT3

#define MOTOR_DRIVE_LEFT_ENABLE PWME_PWME2  
#define MOTOR_DRIVE_LEFT_PERIOD PWMPER2
#define MOTOR_DRIVE_LEFT_DUTY PWMDTY2
#define MOTOR_DRIVE_RIGHT_ENABLE PWME_PWME3   
#define MOTOR_DRIVE_RIGHT_PERIOD PWMPER3
#define MOTOR_DRIVE_RIGHT_DUTY PWMDTY3
 
#define OBSTACLE_DETECTION_PIN PTT_PTT0
#define OBSTACLE_DETECTION_DDR DDRT_DDRT0 

#define BEACON_TRANSMITTER_ENABLE PORTB_BIT2
#define BEACON_TRANSMITTER_ENABLE_DDR DDRB_BIT2
#define BEACON_TRANSMITTER_ACKNOWLEDGE_PIN PORTB_BIT3
#define BEACON_TRANSMITTER_ACKNOWLEDGE_PIN_DDR DDRB_BIT3

#define BEACON_TRANSMITTER_SIGNAL_0 PORTB_BIT4
#define BEACON_TRANSMITTER_SIGNAL_0_DDR DDRB_BIT4
#define BEACON_TRANSMITTER_SIGNAL_1 PORTB_BIT5
#define BEACON_TRANSMITTER_SIGNAL_1_DDR DDRB_BIT5 
#define BEACON_TRANSMITTER_SIGNAL_2 PORTB_BIT6
#define BEACON_TRANSMITTER_SIGNAL_2_DDR DDRB_BIT6

#define DDR_SCL DDRB_BIT0
#define SCL PORTB_BIT0
#define DDR_SDA DDRB_BIT1
#define SDA PORTB_BIT1

/*** CONSTANTS ***/

#define TRUE 1
#define FALSE 0

#define INPUT 0
#define OUTPUT 1

#define CEILING_HEIGHT 72

#define NUMBER_OF_BEACONS 5

#define MAX_UNSIGNED_16_BIT_VALUE 0xFFFF
#define MAX_SIGNED_16_BIT_VALUE 0x7FFF

#define CLOCK_SPEED_HZ 2000000
#define TIMER_COUNTER_PRESCALE 8
#define TIMER_COUNTER_TICKS_PER_S ( CLOCK_SPEED_HZ / TIMER_COUNTER_PRESCALE )
#define TIMER_COUNTER_TICKS_PER_MS ( TIMER_COUNTER_TICKS_PER_S / 1000 )

#define MAX_PERIOD_OF_INTERRUPT_MS ( ( MAX_UNSIGNED_16_BIT_VALUE * 1000 ) / TIMER_COUNTER_TICKS_PER_S )

#define SPEED_OF_SOUND_INCH_PER_SEC 13333
#define CLOCK_TICKS_PER_INCH_OF_SOUND_TRAVEL ( CLOCK_SPEED_HZ / SPEED_OF_SOUND_INCH_PER_SEC )
#define TIMER_CLOCK_TICKS_PER_INCH_OF_SOUND_TRAVEL ( CLOCK_TICKS_PER_INCH_OF_SOUND_TRAVEL / TIMER_COUNTER_PRESCALE )

extern const direction_t FORWARD_MOTION;
extern const direction_t REVERSE_MOTION;
extern const direction_t STOP_MOTION;
extern const direction_t ROTATE_MOTION;

/*** GLOBAL FUNCTIONS ***/

void Delay( milliseconds_t time );

void TurnOnErrorLight( void );
void TurnOffErrorLight( void );
inches_t FeetToInches( Word feet );


/*** INITIALIZATION FUNCTIONS ***/

void InitializeTimers( void );

#endif
