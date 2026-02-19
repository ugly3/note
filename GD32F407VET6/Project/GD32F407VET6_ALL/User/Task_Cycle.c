#include "Task_Cycle.h"
#include "TIM.h"     
#include "ShuMaGuan.h"
#include "HC.h"
#include "BH1750.h"
#include "Mpu6050.h"
#include "RTC.h"
#include "LED.h"
#include "Servo.h"
#include "Walk_Motor.h"
#include "DHT11.h"
#include "HX711.h"
#include "My_RFID_Check.h"
#include "Key.h"    
#include "Fan.h"
#include "ADC.h"
#include "Buzz.h"
#include "Body_Hw.h"
#include "Usart.h"

uint16_t temp_value =0;
uint16_t humi_value =0;

//硬件控制标志位，0代表关，1代表开
uint8_t led_status = 0;
uint8_t door_status = 0;
uint8_t Walkmotor_status = 0;
uint8_t Fan_status=0;
uint8_t HC_status = 0;
uint8_t Weight_status = 0;
uint8_t Body_Hw_ZD_status=0;
uint8_t Fan_ZD_status=0;

uint32_t MQ2_yuzhi = 100,Huoyan_yuzhi = 150;

uint8_t str[30];
void Task_cycle(void)
{
    if(LED_Speed_flag==0)
    {
        if(index_50ms_flag==1) 
        {  
            HC_Get_Data(HC_status);           //获取超声波测距
            index_50ms_flag=0;
        }
        if(index_200ms_flag==1) 
        {
            set_led(led_status);                   //灯
            Door_Servo_SetAngle(door_status);      //大门
            Fan_ZD(Fan_ZD_status);
            Fan_motor_duty(Fan_status);            //风扇
            Set_Walkmotor(Walkmotor_status);          //为1顺时针1圈，为0逆时针
            
            index_200ms_flag=0;
        }
        
        
        if(index_800ms_flag==1)
        {
            BH1750_Get_Data();                          //获取光照强度 (必须接上，否则注释)
            DHT11_Read_Data(&temp_value,&humi_value);   //获取温湿度 (必须接上，否则注释)
            index_800ms_flag=0;
        }
        if(index_1000ms_flag==1)
        {
            if(RFID_Stop==0)                        //检测RFID卡  
                RFID_Check();
            
           
            index_1000ms_flag=0;
        }
    }
    
    if(index_5ms_flag==1)
    {
        
        index_5ms_flag=0;
    }
    if(index_10ms_flag==1) 
    {  

        index_10ms_flag=0;
    }
    
    if(index_500ms_flag==1) 
    {
        Body_Hw_ZD(Body_Hw_ZD_status);           //人体红外检测，灯亮灭
        Get_Weight(Weight_status);               //称重
        RTC_Show_Time();                         //获取时间
        
            if(adc_value[0]>MQ2_yuzhi || adc_value[1]>Huoyan_yuzhi)              //烟雾浓度报警阈值未可调************火焰传感器阈值未可调
                Set_Buzze(1);
            else
                Set_Buzze(0);

        index_500ms_flag=0;
    }
    
    
    if(index_1300ms_flag ==1)  
    {
        sprintf((char*)str,"temp=%d",temp_value);
        Send_TO_Screen((uint8_t *)str); // 设置温度
        
        sprintf((char*)str,"smokescope=%d",adc_value[0]);
        Send_TO_Screen((uint8_t *)str); // 设置烟雾
        
        sprintf((char*)str,"humidity=%d",humi_value);
        Send_TO_Screen((uint8_t *)str); // 设置湿度
        
        
        index_1300ms_flag=0;
    }
}    





