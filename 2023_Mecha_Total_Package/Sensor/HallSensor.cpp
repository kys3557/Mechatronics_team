#include "HallSensor.h"
#define REF 23400.0
#define MIN 60.0
#define SEC_PERIOD (1.0 / (2.0 * REF / MIN ))

// Initialize Class
HallSensor::HallSensor(PinName a) : m_hallSensor(a, PullUp)
{
    float fDummy_sec = 1000.0;
    m_hallSensor.rise (callback(this, &HallSensor::risingCallback));
    m_period_sec = fDummy_sec;
    m_timer_sec.start ();
    m_previous_speed_rps = 0;
    m_filtered_speed_rps = 0;
}

void HallSensor::getUpdateSpeed_rps()
{
    float fSpeed_rps;
    
    if (m_timer_sec.read () > 0.1 || m_period_sec > 0.1) {
        fSpeed_rps = 0.0f;
    } else {
        fSpeed_rps = 2.0f * PI / m_period_sec;
    }
    m_current_speed_rps = fSpeed_rps;
}

void HallSensor::filteringSpeed_rps(float alpha)
{
    float filtered_speed_rps;
    // Update Hall Sensor value (m_current_speed_rps)
    this->getUpdateSpeed_rps();
    
    filtered_speed_rps = alpha * m_current_speed_rps + (1-alpha) * m_previous_speed_rps;
    
    m_filtered_speed_rps = filtered_speed_rps;
    
    m_previous_speed_rps = filtered_speed_rps;

}

float HallSensor::getCurrentSpeed_rps()
{
    return m_current_speed_rps;        
}

float HallSensor::getFilteredSpeed_rps()
{
    return m_filtered_speed_rps;
}

void HallSensor::risingCallback()
{
    if(m_timer_sec.read() > SEC_PERIOD) {
        m_period_sec = m_timer_sec.read();
        m_timer_sec.reset();
    }
}

int HallSensor::getPinState ()
{
    return m_hallSensor;
}
