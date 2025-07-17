#include "mbed.h"
#include "Motor.h"
#include "Plotting.h"
#include "HallSensor.h"
#include "DistanceSensor.h"
#include "SpeedController.h"
#include "LaneSensor.h"
#include "Servo.h"

//To plot with usb, set as below.
Serial pc(USBTX,USBRX); // Tx, Rx Pin
Plotting plot;


//Set each gpio to see the output of the hall sensor as a led
DigitalOut myled1(LED1);

// Pin Mapping /Generate Sensor, Motor Object
LaneSensor laneSensor(p11,p12,p13,p14,p15,p16,p17);
DistanceSensor pir (p20);
HallSensor hall (p29);
Motor motor (p23, p24, p25, p26);
Servo servo (p10);

// Generate Controller Object
//PIDController speedController(0.04, 0.015, 0.0003, 100.0, 0.0, -1.0, 100.0);
//PIDController speedController(0.06, 0.015, 0.0005, 100.0, 0.0, -1.0, 100.0);
//PIDController speedController(0.05, 0.015, 0.0004, 100.0, 0.0, -1.0, 100.0);
 
PIDController speedController(0.05, 0.016, 0.0004, 100.0, 0.0, -1.0, 100.0);

//PIDController speedController(0.1, 0.3, 0.0004, 100.0, 0.0, -1.0, 100.0); THIS
//PIDController speedController(0.01, 0.08,0.0001*1/70, 100.0, 0, 30, 100.0);

//Interrupt is generated every 1ms and count is increased by 1
unsigned int uiFlag_1ms = 0;
unsigned int uiFlag_50ms = 0;

void counter_1ms ()
{
    uiFlag_1ms++;
    uiFlag_50ms++;
}

int main()
{
    wait(1);
    ////Set the 1ms ticker.
    Ticker ticker_1ms;
    ticker_1ms.attach(&counter_1ms, 0.001);
    
    // for calculating dt
    Timer time;
    time.start();
    
    // For PID Controller 
    float curr_speed = 0.0;
    float u_percent= 0.0;
    float setSpeed=0.0;
    float stopDistance = 0.0;
    float degree = 0.0;
    float stop1 = 10.0;
    float stop2 = 0.0;

    // Set low-pass filter alpha
    float hallSensorAlpha = 0.1;//motor 0.2->0.3 0.3not bad ->0.4
    float distanceSensorAlpha = 0.2;//psd
    
    while(1) {
        
        // Every 1 ms,
        if(uiFlag_1ms >= 1) {
            uiFlag_1ms = 0;
            
            //If A is 0, turn on the LED.
            myled1 = hall.getPinState();
            
            // Set Target Speed
            setSpeed = 400.0; //400이 best ... *12/14 400
            // 완주 목적으로는 650... 완주 하고 나서는 올려야함
            // 450으로 하면 내리막길에서 이탈함... 올라가는건 문제 없음?

            // Set Stop Distance+
            stopDistance = 30.0; //35는 맞는데 언덕을 인식해서 줄이기로 함 ㅋㅋ
            
            // Filtering Sensors
            pir.filteringDistance_cm(distanceSensorAlpha);
           hall.filteringSpeed_rps(hallSensorAlpha);

            // Put Target Speed (Reference) to Controller
            speedController.setTarget(setSpeed);
            
            // Get filtered hall sensor speed for control (without noise)
            curr_speed = hall.getFilteredSpeed_rps();
            
            // Using PID controller
            u_percent = speedController.update(curr_speed, time.read());
            /*
            if(pir.getFilteredDistance_cm() <= stopDistance )
            {
                setSpeed = 0.0;
                motor.setSpeed_percent(0,BRAKE);

            }
            */
           
            // 진짜 주행을 위한 거리 조절 코드입니다 하하하하하!!!!!!!!!!!!!!!!!!
           
           if ((pir.getFilteredDistance_cm()) <= stopDistance) {
                
                if ((pir.getFilteredDistance_cm()) <= 27.3) { // 20
                    motor.setSpeed_percent(10, FORWARD); // 12/08 8->5
                }
                else
                {   
                    setSpeed = 0.0; // 멈추게 하기
                    motor.setSpeed_percent(7,FORWARD); // 멈추게 하기
                }
                
                continue;
            }
            

            // Put Control Output to motor
            motor.setSpeed_percent(u_percent, BACKWARD);
            
            float error = laneSensor.getError();
            // TODO : Convert error to servo degree 밑에 있는 거 아님?

            degree = (error); // 일단 1대1
            ////////////////////////////////////////
            servo.update (degree);
            degree = servo.getDegree();
            /*
            if(degree = )  // 좌회전
            {
                setSpeed = 300;
            }
            else if(degree = ) // 좌회전
            {
                setSpeed = 300;
            }
            else if(degree = ) // 약간 좌회전
            {
                setSpeed = 320;
            }
            else if(degree = ) // 직진
            {
                setSpeed = 350;
            }
            else if(degree = ) // 약간 우회전
            {
                setSpeed = 320;
            }
            else if(degree = ) // 우회전
            {
                setSpeed = 300;
            }
            else if(degree = ) // 우회전
            {
                setSpeed = 300;
            }
            */
            int data = laneSensor.getData();


            if(curr_speed > setSpeed) // 내리막길에서 이탈함... cur_speed가 현재 속도
            {
                //motor.setSpeed_percent(15, FORWARD); // 이렇게 해도 이탈함
                setSpeed =0.0; // 그냥 속도를 0으로

            }
            // 커브에서 속도를 줄이고 싶음. 근데 센서가 두개 꺼지는 경우도 있는데 이것도 해야하나
            if((data == 127) ) //좌회전 두개(서보 값 이상해서 꺾이는 각도가 같음)
            {
             motor.setSpeed_percent(6.5, BACKWARD); //1111110

            }
            else if(data == 128) //all light
            {
                motor.setSpeed_percent(10, BACKWARD); // 14->11->15
            }
            else if(data == 126) //1111101
            {
                motor.setSpeed_percent(6.5, BACKWARD); // 14->11->15
            }
            else if(data == 123) //1111011
            {
                motor.setSpeed_percent(11.5, BACKWARD); // 14->11->15
            }
 
            else if(data == 119) //1110111
            {
                motor.setSpeed_percent(14.5, BACKWARD); 
            }
            else if(data == 111) //1101111
            {
                motor.setSpeed_percent(11.5, BACKWARD); 
            }
            else if((data == 95)) //1011111
            {
                motor.setSpeed_percent(6.5, BACKWARD); 
            }
            else if(data == 63) //0111111
            {
                motor.setSpeed_percent(6.5, BACKWARD); 
            }
           // pc.printf("data: %d, error: %f \r\n, Degree: %f\n ", data, error,degree);
            pc.printf(" Degree: %f\n ",degree);

        }
        
        // Every 50 ms, (20 hz)
        if(uiFlag_50ms >= 50) {
            uiFlag_50ms = 0;
            
            // clear plotting buffer
            plot.reset();
            
            // put data to buffer
            
            // For Hall Sensor 
            plot.put(hall.getCurrentSpeed_rps(),   0);
            plot.put(hall.getFilteredSpeed_rps(),  1);            
            // Plotting Sensor Data if you want. 
            // For Distance Sensor
            plot.put(pir.getCurrrendDistance_cm(), 2);
            plot.put(pir.getFilteredDistance_cm(), 3);         
            
            // send buffer
            plot.send(&pc);
        }
    }   
}