#include "ALL.h"



void Door_Servo_SetAngle(uint8_t flag)           //大门   定时器0 CH1  PA9
{
    if(flag==1)
        SteerGear_PWM_CH1(90.0 / 180 * 2000 + 500);
    else if(flag==0)
        SteerGear_PWM_CH1(0.0 / 180 * 2000 + 500);
}


void Lajitong_Servo_SetAngle(uint8_t flag)         //垃圾桶   PA10
{
    if(flag==1)
        SteerGear_PWM_CH2(90.0 / 180 * 2000 + 500);
    else if(flag==0)
        SteerGear_PWM_CH2(0.0 / 180 * 2000 + 500);
}










