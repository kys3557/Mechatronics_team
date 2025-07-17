#ifndef MBED_SPEEDCONTROLLER_H
#define MBED_SPEEDCONTROLLER_H

#include "mbed.h"


class PIDController
{
private:
    float kp;
    float ki;
    float kd;
    float max_windup;
    float umin;
    float umax;

    float m_last_timestamp;
    float m_set_point;
    float m_start_time;
    float m_error_sum;
    float m_last_error;
    float m_last_d;

public:
    PIDController(float kp, float ki, float kd, float max_windup,
                             float start_time,float umin, float umax);
    //void reset();
    float update(float measured_value, float timestamp);
    void setTarget(float target);
    void setKp(float kp);
    void setKi(float ki);
    void setKd(float kd);
    void setMaxWindup(float max_windup);
    float getLastTimeStamp();
};

#endif
