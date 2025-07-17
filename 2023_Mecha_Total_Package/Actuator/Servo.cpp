#include "Servo.h"

Servo::Servo(PinName IN): m_PWM(IN)
{
    m_degree=0;
    m_width= MID+(0.0005/90.0)*m_degree;
    m_period_ticker.attach(callback(this, &Servo::setPeriod),0.02);
    m_width_timeout.attach(callback(this, &Servo::setWidth),m_width);
    m_PWM=1;
}

void Servo::setWidth()
{
    m_PWM=0;
}

void Servo::setPeriod()
{
    m_width_timeout.attach(callback(this, &Servo::setWidth),m_width);
    m_PWM=1;
}

float Servo::getDegree()
{
    return m_degree;
}

void Servo::update(float degree)
{
    if(degree>MAX) {
        degree=MAX;
    }
    if(degree<-1*MAX) {
        degree=MAX*-1;
    }

    m_degree=degree;
    m_width= MID+(0.0005/90.0)*degree;
}