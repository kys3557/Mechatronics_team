#ifndef MBED_PLOTTING_H
#define MBED_PLOTTING_H

#include "mbed.h"

class Plotting
{
private:
    float m_rgfData[32];
    unsigned int m_uiDataCount;
public:
    Plotting();
    
    void reset();
    void put(float data, int index);
    void put(int data, int index);
    void put(unsigned int data, int index);
    void send(Serial *port);
};

#endif
