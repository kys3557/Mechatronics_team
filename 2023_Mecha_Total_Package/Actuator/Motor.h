#ifndef MBED_MOTOR_H
#define MBED_MOTOR_H

#include "mbed.h"

#define FORWARD 'f'
#define BACKWARD 'b'
#define BRAKE 's'

class Motor
{
private:
    PwmOut IN1_, IN2_;
    DigitalOut INH1_, INH2_;

public:
    Motor(PinName IN1, PinName IN2,PinName INH1, PinName INH2);

    void setSpeed_percent(float percent, char direction);
};

#endif
