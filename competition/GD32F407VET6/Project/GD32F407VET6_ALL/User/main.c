#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "stdio.h"
#include "systick.h"
#include "Key.h" 
#include "I2C_Key.h"
#include "oled.h"
#include "LED.H"
#include "ShuMaGuan.h" 
#include "HC.h"
#include "BH1750.h"
#include "Task_Cycle.h"
#include "Walk_Motor.h"
#include "Servo.h"
#include "DHT11.h"
#include "Mpu6050.h"
#include "HX711.h"
#include "Hc595.h"
#include "RC522.h"
#include "Buzz.h"
#include "RTC.h"
#include "ADC.h"
#include "Fan.h"
#include "Body_Hw.h"
#include "usart.h"

void OLED_view_MPU(void);

u8 Buf[30];

int i,j=0;
short Accel[3];//加速度
short Gyro [3];//角速度
long AccelData[3];//单位mg
long GyroData[3];//单位mdps

uint16_t value=0;
uint8_t key_num=0;
uint8_t key_Data;

int main(void)
{
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    
    LED_Init();
	OLED_Init();              //OLED初始化
    Buzze_Init();             //蜂鸣器初始化
    MyRTC_Init();             //RTC时钟初始化
    HC_Init();                //超声波初始化
    ADC_config();             //三个ADC通道
    Walk_Motor_Init();        //步进电机初始化
    Body_Hw_Init();           //人体红外检测模块初始化
//    HX711_Init();             //压力传感器初始化
    
    BH1750_Init();            //光强度传感器初始化 （必须连上，否则卡住）
    DHT11_Init();		      // 温湿度传感器初始化 （必须连上，否则卡住）
    
    InitRc522();              //RFID卡
    
    key_tree_init();

    TIMER0_config();          //PWM初始化
    TIMER2_config();          //1ms中断
    
    ShuMaGuan_Display_Init();   //数码管初始化
    HC595_Init();               //点阵
    
    USART_Init();
   
//     MPU6050_Init();             //MPU6050初始化    I2C1
//    I2C_Key_Init();              //有问题
    

	OLED_Clear();//OLED清屏 
//    delay_1ms(500);


//    Door_Servo_SetAngle(1);     //PA9
//    delay_1ms(1000);
//    Door_Servo_SetAngle(0);
//    
//    Lajitong_Servo_SetAngle(1);    //PA10
//    delay_1ms(1000);
//    Lajitong_Servo_SetAngle(0);

	while(1)
	{
        OLED_view();
        Key_tree_Manage();
        Task_cycle();
        Usart_Send_and_Receive_Data();

        
        if(LED_Speed_flag==1)
            ShuMaGuan_Display(Weight_Shiwu*10);         //数码管显示
        if(LED_Speed_flag==2)
            Matrix_LED_RIGHT_LEFT_Run_Display(DZLL,sizeof(DZLL));    //点阵
        
        //按键
//        I2c_key_scan();
//        process_password_input();
        
        
//         Get_Weight(1);              //称重
//        OLED_ShowString(0,4,(u8*)"2");
//        OLED_ShowString(0,0,(u8*)"1");
//        sprintf((char *)Buf,"Weight: %dg ",Weight_Shiwu);
//        OLED_ShowString(0,2,Buf);
        
//        delay_1ms(200);
//        MPU6050_earthquake();
//        OLED_view_MPU();

        
        //矩阵键盘
//        gpio_bit_set(COL_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN|COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN); // 设置列为高电平
//        gpio_bit_reset(COL_PORT, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
       
//      key_num = keyscanf();
//		if(key_num != NO_KEY)
//		{
//			for(i=0;i<16;i++)
//			{
//				if(key_num==KeyCode[i])
//				{
//                    sprintf((char *)Buf," %0.2X",key_num);
//                    OLED_ShowString(0,0,Buf);
//					break;
//				}
//			}
//		}

        
//            key_Data=KEY_SCAN();
//            if(key_Data!=0)
//            {
//                OLED_ShowNum(48,4,key_Data,3,16);//显示数字
//            }

//            OLED_ShowNum(48,6,key_Data,3,16);//显示数字


	}
}



void OLED_view_MPU(void)
{
    MPU6050Read_Acc_Gyro(Accel,Gyro);
        for(i=0;i<3;i++)
        {
            if(Accel[i]>=0)
                AccelData[i]=Accel[i]*2000/32768;//转换成单位mg
            else
                AccelData[i]=-(-Accel[i]+1)*2000/32768;
            if(Gyro[i]>=0)
                GyroData[i]=Gyro[i]*2000/32768;//转换成单位mdps
            else
                GyroData[i]=-(-Gyro[i]+1)*2000/32768;
        }
        
        if(AccelData[0]<0)//判断是否为负数
		{
			OLED_ShowString(0,3,"-");
			OLED_ShowNum(8,3,-AccelData[0],4,16);//加速度x	
		}
        else
			OLED_ShowNum(8,3,AccelData[0],4,16);//加速度x
		
		if(AccelData[1]<0)//判断是否为负数
		{
			OLED_ShowString(40,3,"-");
			OLED_ShowNum(48,3,-AccelData[1],4,16);//加速度y
		}
        else
			OLED_ShowNum(48,3,AccelData[1],4,16);//加速度y
		
		if(AccelData[2]<0)//判断是否为负数
		{
			OLED_ShowString(80,3,"-");
			OLED_ShowNum(88,3,-AccelData[2],4,16);//加速度z
		}
        else
			OLED_ShowNum(88,3,AccelData[2],4,16);//加速度z
        if(GyroData[0]<0)//判断是否为负数
		{
			OLED_ShowString(0,6,"-");
			OLED_ShowNum(8,6,-GyroData[0],4,16);//陀螺仪x轴
		}
        else
			OLED_ShowNum(8,6,GyroData[0],4,16);//陀螺仪x轴
        
		if(GyroData[1]<0)//判断是否为负数
		{
			OLED_ShowString(40,6,"-");
			OLED_ShowNum(48,6,-GyroData[1],4,16);//陀螺仪y轴
		}
        else
			OLED_ShowNum(48,6,GyroData[1],4,16);//陀螺仪y轴
		
		if(GyroData[2]<0)//判断是否为负数
		{
			OLED_ShowString(80,6,"-");
			OLED_ShowNum(88,6,-GyroData[2],4,16);//陀螺仪z轴
		}
        else
			OLED_ShowNum(88,6,GyroData[2],4,16);//陀螺仪z轴

}
