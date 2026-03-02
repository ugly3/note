#include "ALL.h"

void OLED_view_MPU(void);

//uint8_t key_num=0;


int main(void)
{
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    
    LED_Init();
    IIC_config(&hiic1);
    OLED_Init(&hiic1);
    Buzze_Init();             //蜂鸣器初始化
    MyRTC_Init();             //RTC时钟初始化
    HC_Init();                //超声波初始化
    ADC_config();             //三个ADC通道
    Walk_Motor_Init();        //步进电机初始化
    Body_Hw_Init();           //人梯红外检测模块初始化
//    HX711_Init();             //压力传感器初始化
    
//    BH1750_Init();            //光强度传感器初始化 （必须连上，否则卡住）
//    DHT11_Init();		      // 温湿度传感器初始化 （必须连上，否则卡住）
    
    InitRc522();              //RFID卡
    
    key_tree_init();

    TIMER0_config();          //PWM初始化
    TIMER2_config();          //1ms中断
    
    ShuMaGuan_Display_Init();   //数码管初始化
    HC595_Init();               //点阵
    
    USART_Init();
   


//	OLED_Clear();//OLED清屏 


    delay_1ms(500);


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

        
        if(DLED_view_flag==1)
            ShuMaGuan_Display(Weight_Shiwu*10);         //数码管显示
        if(DLED_view_flag==2)
            Matrix_LED_RIGHT_LEFT_Run_Display(DZLL,sizeof(DZLL));    //点阵
        
        //I2C按键

//        process_password_input();
        
        
//        Get_Weight(1);              //称重
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

	}
}


