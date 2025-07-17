#ifndef MBED_SERVO_H
#define MBED_SERVO_H

#include "mbed.h"

#define MAX 45.0/* Fill in the blank */
#define MID 0.00155811/* 값이 커지면 반시계 Fill in the blank 0.0017511*/

class Servo
{
private:
    DigitalOut m_PWM;
    Ticker m_period_ticker;
    Timeout m_width_timeout;
    float m_degree;
    float m_width;
    
    void setPeriod();
    void setWidth();
public:
    Servo(PinName IN);
    void update(float degree);
    float getDegree();
};

#endif