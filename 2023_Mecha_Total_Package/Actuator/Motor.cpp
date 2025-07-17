#include "Motor.h"


Motor::Motor(PinName IN1, PinName IN2, PinName INH1, PinName INH2)
    : IN1_(IN1), IN2_(IN2), INH1_(INH1), INH2_(INH2)
{
    INH1_=1;
    INH2_=1;
    IN1_.period_us(100);
    IN2_.period_us(100);
}

void Motor::setSpeed_percent(float percent, char direction)
{
    float duty = percent/100.0;
    if(duty>1.0) {
        duty=1.0;
    } else if(duty<0.0) {
        duty=0.0;
    }
    
    if(direction==FORWARD) 
    {
        IN1_=0;
        IN2_=duty;
    } 
    else if (direction == BACKWARD){
        IN1_=duty;
        IN2_=0;
    }
    else if (direction == BRAKE)
    {
        IN1_=1;
        IN2_=1;
    }
    else 
    {
        IN1_=0;
        IN2_=0;
    }   
}