static const PULSES_PER_FOOT = 10;
static const FORWARD_MOTION = whoKnows;
static const BACKWARD_MOTION = whoCares;

interrupt /* vector number for PAOVI */ void motionCompleted();

void initializeMotorControlSystem();

void moveForward( unsigned char distance );

void moveBackward( unsigned char distance );

int distanceToPulses( unsigned char distance );
