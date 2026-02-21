#include "Key.h"           
#include "systick.h"
#include "oled.h"
#include "stdio.h"
#include "RTC.h"
#include "Task_Cycle.h"
#include "HC.h"
#include "HX711.h"
#include "BH1750.h"
#include "My_RFID_Check.h"
#include "ADC.h"
#include "ShuMaGuan.h"
#include "Body_Hw.h"


#define view1_select_count     4    //第一页的选项数
#define view2_select_count     4    //第二页的选项数
#define view3_select_count     4
#define view3_1_select_count   3  //第三页的子页选项数
#define view3_1_2_select_count   4  //第三页的子页的第二选项的选项数

uint32_t LightData_yuzhi = 100;

int OLED_SELECT=0;        //当前的选择参数
int OLED_SELECT_flag=-1;   //上一次选择参数
uint8_t OLED_CONFIRM=0;       //确认键
uint8_t OLED_NUM=0;            //页面参数（上电为主页）
uint8_t RFID_Stop=0;
uint8_t select_card=0;
uint8_t LED_Speed_flag=0;        //数码管、点阵标志位
//任务开始标志
uint8_t task1_is_start = 0;  
uint8_t task2_is_start = 0;  
uint8_t task3_is_start = 0; 
uint8_t task4_is_start = 0;  
uint8_t task5_is_start = 0;
uint8_t task6_is_start = 0;
uint8_t task7_is_start = 0; 
uint8_t task8_is_start = 0;
uint8_t task9_is_start = 0;
uint8_t task10_is_start = 0;  
uint8_t task11_is_start = 0;  
uint8_t task12_is_start = 0;

uint8_t task9_1_is_start=0;
uint8_t task9_2_is_start=0;
uint8_t task9_3_is_start=0;



unsigned char flag = 1; // 1表示允许识别按键
uint8_t GPIO_PIN_Value=0x00;
uint8_t GPIO_PIN_Value2=0x00;
// 矩阵键盘的编码
unsigned char KeyCode[] = {
    0xE7, 0xD7, 0xB7, 0x77,
    0xEB, 0xDB, 0xBB, 0x7B,
    0xED, 0xDD, 0xBD, 0x7D,
    0xEE, 0xDE, 0xBE, 0x7E
};

void COL_Gao_and_ROW_Di(void)
{
    rcu_periph_clock_enable(KEY_RCU_GPIOX);//使能时钟
    gpio_mode_set(COL_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN);//上拉输入
    gpio_bit_set(COL_PORT, COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN); // 设置列为高电平
    
    gpio_mode_set(ROW_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN,ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN);//输出
    gpio_output_options_set(ROW_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN);//推挽输出，50M速度
    gpio_bit_reset(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN); // 设置行为低电平
    delay_1ms(5);
}
void COL_Di_and_ROW_Gao(void)
{
    rcu_periph_clock_enable(KEY_RCU_GPIOX);//使能时钟
    gpio_mode_set(COL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN);//输出
    gpio_output_options_set(COL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN);//推挽输出，50M速度
    gpio_bit_reset(COL_PORT, COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN); // 设置列为低电平
    
    gpio_mode_set(ROW_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN);//输入
    gpio_bit_set(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN); // 设置行为高电平
    delay_1ms(5);
}

// 扫描矩阵键盘
uint8_t keyscanf(void)
{
//    u8 Buf[30];
	uint8_t key = NO_KEY;
	uint8_t a;
	static uint8_t key1=NO_KEY,key2,key3;
	static unsigned char time = 0;
	key3 = key2;
	key2 = key1;
	time++;
    
   COL_Gao_and_ROW_Di();
	GPIO_PIN_Value=gpio_input_port_get(ROW_PORT)>>8;

	if((flag==1)&&((GPIO_PIN_Value&0XF0)!=0XF0))
	{
        COL_Gao_and_ROW_Di();
        GPIO_PIN_Value=gpio_input_port_get(ROW_PORT)>>8;

		if((GPIO_PIN_Value&0XF0)!=0XF0) //为真，则有某一列有按键被按下
		{
			a = GPIO_PIN_Value&0XF0;      //获取列值
		}

            COL_Di_and_ROW_Gao();
            delay_1ms(15);
        GPIO_PIN_Value2=gpio_input_port_get(ROW_PORT)>>8;//扫描行
       
		if((GPIO_PIN_Value2&0X0F)!=0X0F) //为真，则有某一行有按键被按下
		{
			key1 = a |(GPIO_PIN_Value2&0X0F); //拼接行值和列值，就找出具体某个按键被按下
		}
		if((time==3)&&(key1==key2)&&(key1==key3)&&(key1!=NO_KEY))  //按键按下函数keyscanf()执行三次，且三次中key1的值保持不变则可确认按键被按下
		{
			time = 0;                                                 
			key = key1;                                              //赋给key，让其回传
			flag = 0;                                                //令flag=0，不再识别按键，防止重复
		}
	}
	else                                                         //全部按键没有按下时，flag为1继续识别
	{
		COL_Gao_and_ROW_Di();
        GPIO_PIN_Value=gpio_input_port_get(ROW_PORT)>>8;

        
		if((GPIO_PIN_Value&0XF0)==0XF0)
		{
			time = 0; 
			flag = 1;
		}
	}
	return key;
}


// 初始化三个独立按键
void key_tree_init(void)
{
    rcu_periph_clock_enable(KEY_tree_RCU_GPIOX);//使能时钟
    gpio_mode_set(KEY_tree_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_tree_PIN1|KEY_tree_PIN2|KEY_tree_PIN3);//输入
}


uint8_t key_tree_scanf(void)
{
    uint8_t KeyNum = 0;//设置变量
    
    if(0 == gpio_input_bit_get(KEY_tree_PORT,KEY_tree_PIN1))//代表按键按下,==1时表示等于高电压                                    
	{
		delay_1ms(15);
		while(0 == gpio_input_bit_get(KEY_tree_PORT, KEY_tree_PIN1));	
		delay_1ms(15);
		KeyNum = 1;
	}
	
	else if(0 == gpio_input_bit_get(KEY_tree_PORT,KEY_tree_PIN2))//代表按键按下,==0时表示等于低电压                                    
	{
		delay_1ms(15);
		while(0 == gpio_input_bit_get(KEY_tree_PORT, KEY_tree_PIN2));
		delay_1ms(15);	
		KeyNum = 2;
	}
    else if(0 == gpio_input_bit_get(KEY_tree_PORT,KEY_tree_PIN3))//代表按键按下,==0时表示等于低电压                                    
	{
		delay_1ms(15);
		while(0 == gpio_input_bit_get(KEY_tree_PORT, KEY_tree_PIN3));
		delay_1ms(15);	
		KeyNum = 3;
	}
     
	return KeyNum;
}

int8_t Num=0;           

void Key_tree_Manage(void)
{
    Num = key_tree_scanf();
    if(Num==1)
    {
        OLED_SELECT+=2;
        Num=0;
    }
    else if(Num==2)
    {
        OLED_SELECT-=2;
        Num=0;
    }
    else if(Num==3)
    {
        OLED_CONFIRM=1;
        Num=0;
    }
        /*主页*/
        if(0==OLED_NUM)
        {
            if(OLED_SELECT>0)//进入第一页
            {
                OLED_Clear();
                OLED_NUM=1;
                OLED_SELECT=0;
            }
            else if(OLED_SELECT<0)//进入第三页
            {
                OLED_Clear();
                OLED_NUM=3;
                OLED_SELECT=(view3_select_count*2)-2;
            }
        }
        
        /*第一页*/
        else if(1==OLED_NUM)
        {
            if(OLED_SELECT<0)//返回主页
            {
                OLED_Clear();
                OLED_NUM=0;
                OLED_SELECT=0;
            }
            else if(OLED_SELECT>=view1_select_count*2)//进入第二页
            {
                OLED_Clear();
                OLED_NUM=2;
                OLED_SELECT=0;
            }
            else//选择
            {
                OLED_SELECT=OLED_SELECT % (view1_select_count*2);
                if(OLED_SELECT_flag!=OLED_SELECT)
                {
                    OLED_ShowString(0,OLED_SELECT_flag,(uint8_t *)"  ");//清除记号->
                   
                    OLED_SELECT_flag=OLED_SELECT;
                }
                OLED_ShowString(0,OLED_SELECT,(uint8_t *)"->");  
            }
        }
        /*第二页*/
        else if(2==OLED_NUM && 0==task5_is_start && 0==task6_is_start && 0==task7_is_start && 0==task8_is_start)//****************************************
        {
            if(OLED_SELECT<0)//返回第一页
            {
                OLED_Clear();
                OLED_NUM=1;
                OLED_SELECT=(view1_select_count*2)-2;
            }
            else if(OLED_SELECT>=view2_select_count*2)//进入第三页
            {
                OLED_Clear();
                OLED_NUM=3;
                OLED_SELECT=0;
            }
            else//选择
            {
                OLED_SELECT=OLED_SELECT % (view2_select_count*2);
                if(OLED_SELECT_flag!=OLED_SELECT)
                {
                    OLED_ShowString(0,OLED_SELECT_flag,(uint8_t *)"  ");//清除记号->
                   
                    OLED_SELECT_flag=OLED_SELECT;
                }
                OLED_ShowString(0,OLED_SELECT,(uint8_t *)"->");  
            }
        }
        /*第三页*/
        else if(3==OLED_NUM && 0==task9_is_start)
        {
            if(OLED_SELECT<0)//返回第二页
            {
                OLED_Clear();
                OLED_NUM=2;
                OLED_SELECT=(view2_select_count*2)-2;
            }
            else if(OLED_SELECT>=view3_select_count*2)//返回主页
            {
                OLED_Clear();
                OLED_NUM=0;
                OLED_SELECT=0;
            }
            else//选择
            {
                OLED_SELECT=OLED_SELECT % (view3_select_count*2);
                if(OLED_SELECT_flag!=OLED_SELECT)
                {
                    OLED_ShowString(0,OLED_SELECT_flag,(uint8_t *)"  ");//清除记号->
                   
                    OLED_SELECT_flag=OLED_SELECT;
                }
                OLED_ShowString(0,OLED_SELECT,(uint8_t *)"->");  
            }
        }
        
        //RFID修改的子页
        else if (3==OLED_NUM && 1==task9_is_start && 0==task9_1_is_start && 0==task9_2_is_start)
        {
            if(OLED_SELECT<0)//返回到本页最后一个选项
            {
                OLED_SELECT=(view3_1_select_count*2)-2;
            }
//            else if(OLED_SELECT>=view3_1_select_count*2)//返回到第一个选项
//            {
//                OLED_SELECT=0;
//            }
            else//选择
            {
                OLED_SELECT=OLED_SELECT % (view3_1_select_count*2);
                if(OLED_SELECT_flag!=OLED_SELECT)
                {
                    OLED_ShowString(0,OLED_SELECT_flag,(uint8_t *)"  ");//清除记号->
                   
                    OLED_SELECT_flag=OLED_SELECT;
                }
                OLED_ShowString(0,OLED_SELECT,(uint8_t *)"->");  
            }
        }
        //（RFID修改）的子页的第二个选项（删除RFID）的选项数
        else if (3==OLED_NUM && 1==task9_is_start && 1==task9_2_is_start)
        {
            if(OLED_SELECT<0)//返回到本页最后一个选项
            {
                OLED_SELECT=(view3_1_2_select_count*2)-2;
            }
            else//选择
            {
                OLED_SELECT=OLED_SELECT % (view3_1_2_select_count*2);
                if(OLED_SELECT_flag!=OLED_SELECT)
                {
                    select_card=(OLED_SELECT/2)+1;
                   
                    OLED_SELECT_flag=OLED_SELECT;
                }
            }
        }
        
        
  //*******************************************************************************************************************       
        //按键置任务执行标志位
        if(1==OLED_CONFIRM && 1==OLED_NUM)
        {
             switch (OLED_SELECT) 
             {
                    case 0:
                        task1_is_start^=1;
                        break;
                    case 2:
                        task2_is_start^=1;
                        break;
                    case 4:
                        task3_is_start^=1;
                        break;
                    case 6:
                        task4_is_start^=1;
                        break;
                    default:
                        break;
              }
                OLED_CONFIRM=0;
         }
        else if(1==OLED_CONFIRM && 2==OLED_NUM && 0==task5_is_start && 0==task6_is_start && 0==task7_is_start && 0==task8_is_start)//************************
        {
            switch (OLED_SELECT) 
            {
                    case 0:
                        task5_is_start=1;
                        OLED_Clear();
                        break;
                    case 2:
                        task6_is_start=1;
                        OLED_Clear();
                        break;
                    case 4:
                        task7_is_start=1;
                        Weight_status=1;
                        OLED_Clear();
                        break;
                    case 6:
                        task8_is_start=1;
                        OLED_Clear();
                        break;
                    default:
                        break;
            }
             OLED_CONFIRM=0;
        }
    
        else if(1==OLED_CONFIRM && 3==OLED_NUM && 0==task9_is_start)//*****************
        {
            switch (OLED_SELECT) 
            {
                    case 0:
                        task9_is_start=1;
                        OLED_Clear();
                        break;
                    case 2:
                        task10_is_start^=1;
                        
                        break;
                    case 4:
                        task11_is_start^=1;
                       
                        break;
                    case 6:
                        task12_is_start^=1;
                        break;
                    default:
                        break;
            }
             OLED_CONFIRM=0;
        }
        else if(1==OLED_CONFIRM && 3==OLED_NUM && 1==task9_is_start && 0==task9_1_is_start && 0==task9_2_is_start)//*****************
        {
            switch (OLED_SELECT) 
            {
                    case 0:
                        task9_1_is_start=1;
                        task9_2_is_start=0;
                        OLED_Clear();
                        break;
                    case 2:
                        task9_1_is_start=0;
                        task9_2_is_start=1;
                        OLED_Clear();
                        break;
                    case 4:
                        task9_is_start=0;
                        task9_1_is_start=0;
                        task9_2_is_start=0;
                        OLED_SELECT=0;
                        OLED_Clear();
                        break;
                    default:
                        break;
            }
             OLED_CONFIRM=0;
        }
       
        
        
        
     //********************************************************************子页退出函数*******************************************************************************
        else if(1==OLED_CONFIRM && 2==OLED_NUM && 1==task5_is_start)
        {
            task5_is_start=0;
            HC_status=0;
            OLED_SELECT=OLED_SELECT_flag;
            OLED_CONFIRM=0;
            OLED_Clear();
        }
        else if(1==OLED_CONFIRM && 2==OLED_NUM && 1==task6_is_start)
        {
            task6_is_start=0;
            OLED_SELECT=OLED_SELECT_flag;
            OLED_CONFIRM=0;
            OLED_Clear();
        }
        else if(1==OLED_CONFIRM && 2==OLED_NUM && 1==task7_is_start)
        {
            LED_Speed_flag=0;
            task7_is_start=0;
            Weight_status=0;
            OLED_SELECT=OLED_SELECT_flag;
            OLED_CONFIRM=0;
            OLED_Clear();
            ShuMaGuan_OFF();
        }
        else if(1==OLED_CONFIRM && 2==OLED_NUM && 1==task8_is_start)
        {
            task8_is_start=0;
            OLED_SELECT=OLED_SELECT_flag;
            OLED_CONFIRM=0;
            OLED_Clear();
        }
        else if(1==OLED_CONFIRM && 3==OLED_NUM && 1==task9_is_start && 1==task9_1_is_start)
        {
            task9_1_is_start=0;
            task9_2_is_start=0;
            OLED_CONFIRM=0;
            OLED_Clear();
        }
        
}



uint8_t aShowTime[16] = "hh:ms:ss";
uint8_t aShowDate[16] = "dd-mm-yyyy";

void OLED_view(void)
{
    u8 Buf[30];
    if(0==OLED_NUM)//主页
    {
        OLED_ShowCHinese(6,0,0); //智
        OLED_ShowCHinese(22,0,1);//能
        OLED_ShowCHinese(38,0,2);//家
        OLED_ShowCHinese(54,0,3);//居
        
        OLED_ShowCHinese(50,2,4);//智
        OLED_ShowCHinese(66,2,5);//联
        OLED_ShowCHinese(82,2,6);//未
        OLED_ShowCHinese(98,2,7);//来

        sprintf((char *)aShowDate, "%02x-%02x-%02x",0x2000 + rtc_initpara.year,rtc_initpara.month,rtc_initpara.date );
        OLED_ShowString(10,4,aShowDate);

        sprintf((char *)aShowTime, "%02x:%02x:%02x", rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
        OLED_ShowString(25,6,aShowTime);
        
        OLED_ShowNum(90,6,adc_value[1],3,16);
    }
    else if(1==OLED_NUM)
    {
        OLED_ShowCHinese(17,0,8); //灯
        
        OLED_ShowCHinese(17,2,9);  //大
        OLED_ShowCHinese(34,2,10); //门
        
        OLED_ShowCHinese(17,4,11); //窗
        OLED_ShowCHinese(34,4,12); //帘
        
        OLED_ShowCHinese(17,6,63); //风
        OLED_ShowCHinese(34,6,64); //扇
    //*****************************************************   
        //灯
        if(task1_is_start==0){
            OLED_ShowCHinese(99,0,31);//关
            led_status=0;          
        }else{
            OLED_ShowCHinese(99,0,32);//开
            led_status=1;          
        }
        //大门
        if(task2_is_start==0){
            OLED_ShowCHinese(99,2,31);//关
            door_status=0;          
        }else{
            OLED_ShowCHinese(99,2,32);//开
            door_status=1;         
        }
        //窗帘
         if(task3_is_start==0){
            OLED_ShowCHinese(99,4,31);//关
            Walkmotor_status=0;          
        }else{
            OLED_ShowCHinese(99,4,32);//开
            Walkmotor_status=1;         
        }
        //风扇
         if(task4_is_start==0){
            OLED_ShowCHinese(99,6,31);//关
            Fan_status=0;          
        }else{
            OLED_ShowCHinese(99,6,32);//开
            Fan_status=1;         
        }
        
        
        
    //******************************************************   
        
    }
    else if(2==OLED_NUM && 0==task5_is_start && 0==task6_is_start && 0==task7_is_start && 0==task8_is_start)//*********************
    {
        OLED_ShowCHinese(17,0,13); //超
        OLED_ShowCHinese(34,0,14); //声
        OLED_ShowCHinese(50,0,15); //波
        OLED_ShowCHinese(66,0,16); //测
        OLED_ShowCHinese(82,0,17); //距
        
        OLED_ShowCHinese(17,2,18); //查
        OLED_ShowCHinese(34,2,19); //看
        OLED_ShowCHinese(51,2,20); //温
        OLED_ShowCHinese(68,2,21); //湿
        OLED_ShowCHinese(85,2,22); //度
        
        OLED_ShowCHinese(17,4,23); //称
        OLED_ShowCHinese(34,4,24); //重
        
        OLED_ShowCHinese(17,6,18); //查
        OLED_ShowCHinese(34,6,19); //看
        OLED_ShowCHinese(51,6,25); //光
        OLED_ShowCHinese(68,6,26); //照
        OLED_ShowCHinese(85,6,27); //强
        OLED_ShowCHinese(102,6,22); //度
    }
    else if(3==OLED_NUM && 0==task9_is_start && 0==task9_1_is_start && 0==task9_2_is_start)//*********************
    {
            OLED_ShowString(17,0,(u8*)"RFID");
            OLED_ShowCHinese(51,0,28); //卡
            OLED_ShowCHinese(68,0,29); //修
            OLED_ShowCHinese(85,0,30); //改
            
            OLED_ShowCHinese(25,2,8); //灯
            OLED_ShowCHinese(51,2,65); //自
            OLED_ShowCHinese(68,2,66); //动
            
            OLED_ShowCHinese(17,4,63); //风
            OLED_ShowCHinese(34,4,64); //扇
            OLED_ShowCHinese(51,4,65); //自
            OLED_ShowCHinese(68,4,66); //动

            OLED_ShowCHinese(17,6,11); //窗
            OLED_ShowCHinese(34,6,12); //帘
            OLED_ShowCHinese(51,6,65); //自
            OLED_ShowCHinese(68,6,66); //动
       
            //灯自动模式
            if(task10_is_start==0){
                    OLED_ShowCHinese(99,2,31);//关
                Body_Hw_ZD_status=0;
            }
            else{
                OLED_ShowCHinese(99,2,32);//开
                Body_Hw_ZD_status=1;
            }
            //风扇自动模式
            if(task11_is_start==0){
                OLED_ShowCHinese(99,4,31);//关
                Fan_ZD_status=0;
            }
            else{
                OLED_ShowCHinese(99,4,32);//开
                Fan_ZD_status=1;
            }
            //窗帘自动模式
            if(task12_is_start==0){
                OLED_ShowCHinese(99,6,31);//关
            }
            else{
                OLED_ShowCHinese(99,6,32);//开
                if(LightData_Hex>LightData_yuzhi) {         //************************************************光照阈值未可调
                    Walkmotor_status=1;
                    task3_is_start=1;
                }                
                else{
                    Walkmotor_status=0;
                    task3_is_start=0;
                }
            }
        
    }
    
    
    else if(task5_is_start)
    {
        HC_status=1;
        //超声波测距
        OLED_ShowString(20,2,(u8*)"Dis: ");  
        OLED_ShowNum(50,4,Distance,3,16);//显示距离，单位cm  
        OLED_ShowString(80,4,(u8*)"cm");
    }
    else if(task6_is_start)
    {
        //温湿度
        sprintf((char *)Buf,"humidity: %d",humi_value);
        OLED_ShowString(0,0,Buf);
        sprintf((char *)Buf,"temperature: %d",temp_value);
        OLED_ShowString(0,2,Buf);
    }
    else if(task7_is_start)
    {
        //压力传感器
        LED_Speed_flag=1;
        OLED_ShowString(90,2,(u8*)"        ");
        sprintf((char *)Buf,"Weight: %0.1fg ",Weight_Shiwu);
        OLED_ShowString(0,2,Buf);
    }
    else if(task8_is_start)
    {
       //光强度
        OLED_ShowString(99,4,(u8*)"      ");
        OLED_ShowString(0,2,(u8*)"LightData:");
        sprintf((char *)Buf,"%d lx ",LightData_Hex);
        OLED_ShowString(65,4,Buf);
    }
    else if(1==task9_is_start && 0==task9_1_is_start && 0==task9_2_is_start && 0==task9_3_is_start)
    {
        OLED_ShowCHinese(17,0,33);//录
        OLED_ShowCHinese(34,0,34);//入
        OLED_ShowCHinese(51,0,35);//卡
        OLED_ShowCHinese(68,0,36);//片
        
        OLED_ShowCHinese(17,2,37);//删
        OLED_ShowCHinese(34,2,38);//除
        OLED_ShowCHinese(51,2,35);//卡
        OLED_ShowCHinese(68,2,36);//片
        
        OLED_ShowCHinese(17,4,39);//返
        OLED_ShowCHinese(34,4,40);//回
        OLED_ShowCHinese(51,4,41);//上
        OLED_ShowCHinese(68,4,42);//一
        OLED_ShowCHinese(85,4,43);//级
        RFID_Stop=0;
    }
    else if(task9_1_is_start)
    {
        OLED_ShowCHinese(17,2,44);//请
        OLED_ShowCHinese(34,2,45);//放
        OLED_ShowCHinese(51,2,46);//置
        OLED_ShowCHinese(68,2,35);//卡
        OLED_ShowCHinese(85,2,36);//片
        RFID_Stop=1;
        
        //以下为卡片录入代码
        RFID_ADD();
    }
    else if(task9_2_is_start)
    {
        OLED_ShowCHinese(17,2,47);//选
        OLED_ShowCHinese(34,2,48);//择
        OLED_ShowCHinese(51,2,37);//删
        OLED_ShowCHinese(68,2,38);//除
        sprintf((char *)Buf," %d",select_card);
        OLED_ShowString(85,2,Buf);
        OLED_ShowCHinese(102,2,35);//卡
        
        //以下执行卡片删除代码
        RFID_DELETE();
    }
    
    
    
    
}







