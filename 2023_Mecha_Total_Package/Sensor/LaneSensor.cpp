#include "LaneSensor.h"

LaneSensor::LaneSensor(PinName lll, PinName ll, PinName l, PinName m, PinName r, PinName rr, PinName rrr)
    : sensorsIn_(lll, ll, l, m, r, rr, rrr)
{
    error_ = 0.0;
    //prevError_ =0.0; 이건 아직...??
    //sensorsIn_.mode(PullNode);
}
float LaneSensor::getError()
{
    float errorSum = 0;
    int errorCount = 0;
    float errorConstant = 20.0;

    int sensorIn = sensorsIn_ & sensorsIn_.mask();  // read the bus and mask out bits not being used
    if ((sensorIn | 0b1111110) == 0b1111110) {
        // lll
        errorSum += errorConstant * 3.0;
        errorCount++;
    }
    if ((sensorIn | 0b1111101) == 0b1111101) {
        // ll
        errorSum += errorConstant * 2.0;
        errorCount++;
    }
    if ((sensorIn | 0b1111011) == 0b1111011) {
        // l
        errorSum += errorConstant * 1.0;
        errorCount++;
    }
    if ((sensorIn | 0b1110111) == 0b1110111) {
        // m
        errorSum += errorConstant * 0.0;
        errorCount++;
    }
    if ((sensorIn | 0b1101111) == 0b1101111) {
        // r
        errorSum += errorConstant * (-1.0);
        errorCount++;
    }
    if ((sensorIn | 0b1011111) == 0b1011111) {
        // rr
        errorSum += errorConstant * (-2.0);
        errorCount++;
    }
    if ((sensorIn | 0b0111111) == 0b0111111) {
        // rrr
        errorSum += errorConstant * (-3.0);
        errorCount++;
    }
    // TODO: Write ll, l, m, r, rr, rrr parts

    if (errorCount == 0) {
        error_ = prevError_;
    } 
    else 
    {
        error_ = errorSum / ((float)errorCount);
    }
    prevError_ = error_;

    return error_;
}

int LaneSensor::getData() // 데이터 가져오는 함수?
{

    return sensorsIn_ & sensorsIn_.mask();;
}