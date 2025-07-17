#ifndef MBED_DISTANCESENSOR_H
#define MBED_DISTANCESENSOR_H

#include "mbed.h"

class DistanceSensor
{
public:
    DistanceSensor(PinName a);
    float getCurrrendDistance_cm();
    float getFilteredDistance_cm();
    void filteringDistance_cm(float alpha);

protected:
    void updateCurrentDistance_cm();
    float m_distance_cm;
    AnalogIn  m_analogIn;
    float m_previous_distance_cm;
    float m_filtered_distance_cm;
};

#endif //MBED_DISTANCESENSOR_H