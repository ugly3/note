#include "Fan.h"
#include "TIM.h"   
#include "Task_Cycle.h"
#include "Key.h"

uint32_t temp_yuzhi=29,humi_yuzhi=50;

void Fan_motor_duty(uint8_t status)            //风扇 PA11
{
    if(status==1)
        SteerGear_PWM_CH3(8000);
    else if(status==2)
        SteerGear_PWM_CH3(18000);
    else if(status==0)
        SteerGear_PWM_CH3(0);
}

//风扇自动
void Fan_ZD(uint8_t status)
{
    if(status){
        if(temp_value>temp_yuzhi || humi_yuzhi>50){       //*************************************温度湿度阈值未可调
            Fan_status=1;
            task4_is_start=1;
        }
        else{
            Fan_status=0;
            task4_is_start=0;
        }
    }
}

