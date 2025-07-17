#ifndef MBED_LANESENSOR_H
#define MBED_LANESENSOR_H

#include "mbed.h"




class LaneSensor
{
public:
    LaneSensor(PinName lll,PinName ll,PinName l,PinName m,PinName r,PinName rr,PinName rrr);
    float getError();
    int getData();

protected:
    float error_,prevError_;
    BusIn  sensorsIn_;
};

#endif //MBED_DISTANCESENSOR_H
