#ifndef MBED_HallSensor_H
#define MBED_HallSensor_H

#include "mbed.h"

#define PI 3.141592


class HallSensor
{
public:
    HallSensor(PinName a);
    
    int getPinState ();
    float getCurrentSpeed_rps();
    float getFilteredSpeed_rps();
    void filteringSpeed_rps(float alpha);

protected:
    void risingCallback();
    void getUpdateSpeed_rps();
    
    InterruptIn m_hallSensor;
    Timer m_timer_sec;
    float m_period_sec;
    float m_current_speed_rps;
    float m_previous_speed_rps;
    float m_filtered_speed_rps;
};

#endif