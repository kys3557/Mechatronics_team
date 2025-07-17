#include "SpeedController.h"


PIDController::PIDController(float kp, float ki, float kd, float max_windup,
                             float start_time, float umin, float umax)
{
//    The PID controller can be initalized with a specific kp value
//    ki value, and kd value
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->max_windup = max_windup;

    this->umin = umin;
    this->umax = umax;

    //Store relevant data
    this->m_last_timestamp = 0.0;
    this->m_set_point = 0.0;
    this->m_start_time = start_time;
    this->m_error_sum = 0.0;
    this->m_last_error = 0.0;
}



float PIDController::update(float measured_value, float timestamp)
{
    // TODO
    // Fill in the blank
    
    float delta_time = timestamp - m_last_timestamp;
    float error = m_set_point - measured_value;
    
    m_last_timestamp = timestamp;
    
    
    m_error_sum += delta_time*(error + m_last_error)/2;
    
    float delta_error = error - m_last_error;
    
    if(delta_error > 10.0) {
        delta_error = 0.0;
    }
    
    m_last_error = error;

    if(m_error_sum > max_windup) {
        m_error_sum = max_windup;
    } else if(m_error_sum < -1.0*max_windup) {
        m_error_sum = max_windup*-1.0;
    }

    float p = kp*error;
    float i = ki*m_error_sum;
    float d = kd*(delta_error/delta_time);

    float u=p+i+d;

    if(u > umax) {
        u = umax;
    } else if(u < umin) {
        u = umin;
    }

    return u;
}

void PIDController::setTarget(float target)
{
    this->m_set_point = target;
}

void PIDController::setKp(float kp)
{
    this->kp = kp;
}

void PIDController::setKi(float ki)
{
    this->ki = ki;
}

void PIDController::setKd(float kd)
{
    this->kd = kd;
}

void PIDController::setMaxWindup(float max_windup)
{
    this->max_windup = max_windup;
}

float PIDController::getLastTimeStamp()
{
    return this->m_last_timestamp;
}