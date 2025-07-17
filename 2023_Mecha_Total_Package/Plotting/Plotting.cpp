#include "Plotting.h"

Plotting::Plotting()
{
    for(int i=0; i<32; i++) {
        m_rgfData[i]=0;
    }
    m_uiDataCount=0;
}

void Plotting::reset()
{
    for(int i = 0; i < 32; i++) {
        m_rgfData[i] = 0;
    }
    m_uiDataCount = 0;
}

void Plotting::put(float data, int index)
{
    m_rgfData[index] = data;
    m_uiDataCount++;
}

void Plotting::put (int data, int index)
{
    m_rgfData[index] = (float)data;
    m_uiDataCount++;
}

void Plotting::put (unsigned int data, int index)
{
    m_rgfData[index] = (float)data;
    m_uiDataCount++;
}

void Plotting::send(Serial *port)
{
    port->putc (0xAA);
    port->putc (0xBB);
    port->putc (0xCC);
    port->putc ((char)(m_uiDataCount*4));
    for(int i=0; i < m_uiDataCount; i++) {
        char *bytePtr = (char *)&(m_rgfData[i]);
        port->putc(*bytePtr);      // reverse the order of these lines if you have endian issues
        port->putc(*(bytePtr+1));
        port->putc(*(bytePtr+2));
        port->putc(*(bytePtr+3));
    }
}