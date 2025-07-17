#include "DistanceSensor.h"

#define MAX_DISTANCE (100.0)

// Class Initialization 
DistanceSensor::DistanceSensor(PinName a): m_analogIn(a)
{
    m_distance_cm = MAX_DISTANCE;
    m_filtered_distance_cm = MAX_DISTANCE;
    m_previous_distance_cm = MAX_DISTANCE;
}

// Update raw distance sensor data (m_distance_cm)
void DistanceSensor::updateCurrentDistance_cm()
{
    /* Change to your code (Interpolation) */
    /////////////////////////////////////////
    float scaled_analog = 3.3*m_analogIn;
    if (scaled_analog >= 3.0){
        m_distance_cm = 5.0;
    }
    else if (scaled_analog < 3.0 && scaled_analog >= 2.9){
        m_distance_cm = 6.0;
    }
    else if (scaled_analog < 2.9 && scaled_analog >= 2.8){
        m_distance_cm = 7.0;
    }
    else if (scaled_analog < 2.8 && scaled_analog >= 2.7){
        m_distance_cm = 8.0;
    }
    else if (scaled_analog < 2.7 && scaled_analog >= 2.6){
        m_distance_cm = 9.0;
    }
    else if (scaled_analog < 2.6 && scaled_analog >= 2.5){
        m_distance_cm = 10.0;
    }
    else if (scaled_analog < 2.5 && scaled_analog >= 2.4){
        m_distance_cm = 11.0;
    }
    else if (scaled_analog < 2.4 && scaled_analog >= 2.3){
        m_distance_cm = 12.0;
    }
    else if (scaled_analog < 2.3 && scaled_analog >= 2.2){
        m_distance_cm = 13.0;
    }
    else if (scaled_analog < 2.2 && scaled_analog >= 2.1){
        m_distance_cm = 14.0;
    }
    else if (scaled_analog < 2.1 && scaled_analog >= 2.0){
        m_distance_cm = 15.0;
    }
    else if (scaled_analog < 2.0 && scaled_analog >= 1.9){
        m_distance_cm = 16.0;
    }
    else if (scaled_analog < 1.9 && scaled_analog >= 1.8){
        m_distance_cm = 17.0;
    }
    else if (scaled_analog < 1.8 && scaled_analog >= 1.7){
        m_distance_cm = 18.0;
    }
    else if (scaled_analog < 1.7 && scaled_analog >= 1.6){
        m_distance_cm = 19.0;
    }
    else if (scaled_analog < 1.6 && scaled_analog >= 1.5){
        m_distance_cm = 20.0;
    }
    else if (scaled_analog < 1.5 && scaled_analog >= 1.4){
        m_distance_cm = 21.0;
    }
    else if (scaled_analog < 1.4 && scaled_analog >= 1.3){
        m_distance_cm = 22.0;
    }
    else if (scaled_analog < 1.3 && scaled_analog >= 1.2){
        m_distance_cm = 23.0;
    }
    else if (scaled_analog < 1.2 && scaled_analog >= 1.1){
        m_distance_cm = 24.0;
    }
    else if (scaled_analog < 1.1 && scaled_analog >= 1.0){
        m_distance_cm = 25.0;
    }
    else if (scaled_analog < 1.0 && scaled_analog >= 0.9){
        m_distance_cm = 25.5;
    }
    else if (scaled_analog < 0.9 && scaled_analog >= 0.8){
        m_distance_cm = 26.0;
    }
    else if (scaled_analog < 0.8 && scaled_analog >= 0.7){
        m_distance_cm = 26.5;
    }
    else if (scaled_analog < 0.7 && scaled_analog >= 0.6){
        m_distance_cm = 27.0;
    }
    else if (scaled_analog < 0.6 && scaled_analog >= 0.5){
        m_distance_cm = 27.5;
    }
    else if (scaled_analog < 0.5 && scaled_analog >= 0.4){
        m_distance_cm = 28.0;
    }
    else if (scaled_analog < 0.4 && scaled_analog >= 0.3){
        m_distance_cm = 28.5;
    }
    else if (scaled_analog < 0.3 && scaled_analog >= 0.2){
        m_distance_cm = 29.0;
    }
    else if (scaled_analog < 0.2 && scaled_analog >= 0.1){
        m_distance_cm = 30.0;
    }
    else if (scaled_analog < 0.1 && scaled_analog >= 0.05){
        m_distance_cm = 31.0;
    }
    else{
        m_distance_cm = MAX_DISTANCE;
    }
}

// Low Pass Filter for raw distance value 
void DistanceSensor::filteringDistance_cm(float alpha)
{
    float filtered_distance_cm;
    
    // Update Current Distance (m_distance_cm)
    this->updateCurrentDistance_cm();
    
    // Filtering 
    filtered_distance_cm = alpha * m_distance_cm + (1-alpha) * m_previous_distance_cm;
    
    m_filtered_distance_cm = filtered_distance_cm;
    
    m_previous_distance_cm = filtered_distance_cm;
}

// Return raw distance value
float DistanceSensor::getCurrrendDistance_cm()
{
    return m_distance_cm;
}

float DistanceSensor::getFilteredDistance_cm()
{
    return m_filtered_distance_cm;
}