#include "ALL.h"


void Task_cycle(void)
{
    if(DLED_view_flag==0)
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
//            BH1750_Get_Data();                          //获取光照强度 (必须接上，否则注释)
//            DHT11_Read_Data(&temp_value,&humi_value);   //获取温湿度 (必须接上，否则注释)
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
        sprintf((char*)Buf,"temp=%d",temp_value);
        Send_TO_Screen((uint8_t *)Buf); // 设置温度
        
        sprintf((char*)Buf,"smokescope=%d",adc_value[0]);
        Send_TO_Screen((uint8_t *)Buf); // 设置烟雾
        
        sprintf((char*)Buf,"humidity=%d",humi_value);
        Send_TO_Screen((uint8_t *)Buf); // 设置湿度
        
        
        index_1300ms_flag=0;
    }
}    





