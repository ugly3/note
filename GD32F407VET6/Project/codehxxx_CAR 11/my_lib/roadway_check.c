#include "all_module.h"
#include "roadway_check.h"


uint8_t wheel_L_Flag =0;                //左转标志位
uint8_t wheel_R_Flag = 0;               //右转标志位
uint8_t wheel_Nav_Flag = 0;             //码盘旋转标志位
uint8_t Go_Flag = 0;                    //前进标志位
uint8_t Back_Flag = 0;                  //后退标志位
uint8_t Track_Flag = 0;                 //循迹标志位
uint8_t Stop_Flag = 0;                  //停止标志位
uint8_t Line_Flag = 0;                  //暂时没用
uint16_t count = 0;                     //计时，具体还未知

extern uint8_t RFID_Flag;              //RFID检测标志位
int LSpeed = 0;                        //左轮速度
int RSpeed = 0;                        //右轮速度
int Car_Spend = 0;                     //车速度的临时变量
uint16_t temp_MP = 0;                  //码盘值临时变量
uint16_t temp_Nav = 0;                 //码盘旋转值临时变量

uint32_t Wheel_flag = 0;               //转弯计数值

void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);


uint16_t Count_ms = 0;//遇到特殊标志物或十字路口时的时间，根据时间长短，从而判断出特殊标志物在哪个坐标点
//_________________________________________________________
int16_t Roadway_cmp;                    //最终码盘值
extern int16_t CanHost_Mp;             //码盘临时变量值

/*
	码盘同步
**/
void Roadway_mp_syn(void)
{
	Roadway_cmp = CanHost_Mp;
}

/*
	码盘获取
**/
uint16_t Roadway_mp_Get(void)
{
	uint32_t ct;
	if(CanHost_Mp > Roadway_cmp)
		ct = CanHost_Mp - Roadway_cmp;
	else
		ct = Roadway_cmp - CanHost_Mp;
	if(ct > 0x8000)
		ct = 0xffff - ct;
	return ct;	
}

uint16_t Roadway_Navig;
extern uint16_t CanHost_Navig;

/*
	角度同步
**/
void Roadway_nav_syn(void)
{
	Roadway_Navig = CanHost_Navig;
}

/*
	获取角度差值
**/
uint16_t Roadway_nav_Get(void)
{
	uint16_t ct;
	if(CanHost_Navig > Roadway_Navig)
		ct = CanHost_Navig - Roadway_Navig;
	else
		ct = Roadway_Navig - CanHost_Navig;
	while(ct >= 36000)
		ct -= 36000;
	return ct;
}

//_______________________________________________________________

//清除小车所有标志位
void Roadway_Flag_clean(void)
{
	wheel_L_Flag =0;
	wheel_R_Flag = 0;
	Go_Flag = 0;
	Back_Flag = 0;
	Track_Flag = 0;
	Stop_Flag = 0;
    RFID_Angle_or_shizilukou_Flag = 0;
    Send_UpMotor(0,0);
    
//	temp_MP = 0;
}

/**
	前进监测
**/
void Go_and_Back_Check(void)
{	
	if(Go_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Go_Flag = 0;
			Stop_Flag = 3;
			Send_UpMotor(0,0);		// 停止
		}
	} 
	else if(Back_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Back_Flag = 0;
			Stop_Flag=3;
			Send_UpMotor(0,0);
		}
	}
}

//自己加的
void Check_diatace(void)
{
//    Send_InfoData_To_Fifo("%d\n",RFID_Read_Flag);
    if(1 == Check_Distance)
    {
        if(Distance <= Roadway_mp_Get())
        {
            RFID_Read_Flag = 0;
            Check_Distance = 0;            
        }
    }
}

uint8_t Roadway_GoBack_Check(void)
{
	return ((Go_Flag == 0)&&(Back_Flag == 0)&&(Track_Flag == 0)&&(wheel_L_Flag == 0)&&(wheel_R_Flag == 0))? 1:0;
}

/**
	码盘转弯
*/
void wheel_Nav_check(void)
{ 	
	uint16_t Mp_Value = 0;
	
	if(wheel_Nav_Flag)
	{
		Mp_Value = Roadway_mp_Get(); 
		if(Mp_Value >= temp_Nav)
		{
			wheel_Nav_Flag = 0;
			Stop_Flag = 2;
			Send_UpMotor(0,0);
		}
	}
}


/**
	根据循迹线转弯
*/
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{ 	
	uint16_t Track_Value = 0;
	
	if(wheel_L_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		if(!(Track_Value & 0X10))		// 找到循迹线，停止
		{	
			if(Wheel_flag > 50)
			{
				wheel_L_Flag = 0;
				Wheel_flag=0;
				Stop_Flag=2;
				Send_UpMotor(0,0);
			}
		}
		else if(Track_Value == 0Xff)	// 循迹灯全亮
		{			
			Wheel_flag++;
		}
	} 
	else if(wheel_R_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);

		 if(!(Track_Value &0X08)) //找到循迹线，停止
			{	
				if(Wheel_flag > 50)
				{
					wheel_R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
					Send_UpMotor(0,0);
				}
			}
			else if( Track_Value == 0Xff) 
			{				
				Wheel_flag++;
			}
	}
}


/**
	循迹监测
*/
void Track_Check()
{	
	if(Track_Flag == 1)
	{
		Track_Correct(Get_Host_UpTrack(TRACK_H8));        //处理后八位循迹数据
	}
	if(Motor_Data.Track_White_Flag)
	{
		Motor_Data.xCAR_Track_Correct_White(Get_Host_UpTrack(TRACK_H8));       //循迹白线，特殊地形，到白线后停止
	}
}

void Roadway_Check(void)
{
	Go_and_Back_Check();         //前进后退检测
	wheel_Track_check();         //循迹线转弯检测
	wheel_Nav_check();           // 码盘转弯检测
	Track_Check();	             //循迹检测
//    Check_diatace();           //超出距离检测******************************有问题，开启后程序不能用了
    
}
	

/***************************************************************
** 功能：     电机控制函数
** 参数：	  L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：   无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
	}
	else 
	{
		if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //限制速度参数
	}	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//限制速度参数		
	}
	Send_UpMotor(L_Spend ,R_Spend);	
}

extern uint8_t RFID_Flag;
extern uint8_t Terrain_Flag;
/***************************************************************
** 功能：     循迹函数
** 参数：	  无参数
** 返回值：   无
****************************************************************/
// 函数：将一个字节的十六进制数转换为八位二进制字符串
void hexToBin(unsigned char hex, char* binStr) {
    for (int i = 7; i >= 0; i--) {
        binStr[7 - i] = (hex & (1 << i)) ? '1' : '0'; // 检查当前位是否为1
    }
    binStr[8] = '\0'; // 添加字符串结束符
}
// 函数：判断二进制字符串中是否包含四个或四个以上连续的0
int hasFourConsecutiveZeros(const char* binStr) {
    int zeroCount = 0; // 用于统计连续0的数量
    for (int i = 0; i < 8; i++) { // 遍历八位二进制字符串
        if (binStr[i] == '0') {
            zeroCount++; // 如果是0，计数加1
            if (zeroCount >= 4) return 1; // 如果连续0的数量达到4，返回1
        } else {
            zeroCount = 0; // 如果遇到1，重置计数
        }
    }
    return 0; // 如果遍历完没有找到四个连续的0，返回0
}

void Track_Correct(uint8_t gd)
{
	  uint8_t Buf[30];

    char binStr[9]; // 用于存储八位二进制字符串
    hexToBin(gd, binStr);// 将十六进制数转换为八位二进制字符串
    if (hasFourConsecutiveZeros(binStr))//灯出现四个或以上熄灭则判定为十字路口或卡片在十字路口
    {
        Track_Flag = 0;
        Stop_Flag = 1;
        Send_UpMotor(0,0);
        RFID_Angle_or_shizilukou_Flag=1;
        Count_ms = track_time_ms;
        track_time_ms=0;
        track_time_Start=0;
			 
		sprintf((char*)Buf,"%d\r\n",Count_ms);  
    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
    }
 
	else if(gd==0xE7)                             //E7---1110 0111  车在中间，正常行驶
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
	} 
	else if((gd == 0xF7) || (gd == 0XF3))           //F7---1111 0111   F3---1111 0011  车偏左
	{
		LSpeed=Car_Spend+20;
		RSpeed=Car_Spend-40;
	} 
	else if((gd == 0XFB) || (gd == 0XF9))           //FB---1111 1011    F9---1111 1001 车偏左+
	{
		LSpeed=Car_Spend+40;
		RSpeed=Car_Spend-60;
	} 
	else if((gd == 0XFD) || (gd == 0XFC))           //FD---1111 1101    FC---1111 1100  车偏左++
	{
		LSpeed=Car_Spend+60;
		RSpeed=Car_Spend-90;
	} 
	else if(gd == 0XFE)                           //FE---1111 1110    车偏左（FULL）
	{
		LSpeed = Car_Spend+80;
		RSpeed = Car_Spend-120;
	} 
	else if((gd == 0XEF) || (gd == 0XCF))           //EF---1110 1111      CF---1100 1111  车偏右
	{
		RSpeed = Car_Spend+20;
		LSpeed = Car_Spend-40;
	} 
	else if((gd == 0XDF) || (gd == 0X9F))          //DF---1101 1111       9F---1001 1111  车偏右+
	{
		RSpeed = Car_Spend+40;
		LSpeed = Car_Spend-60;
	}
	else if((gd == 0XBF) || (gd == 0X3F))          //BF---1011 1111       3F---0011 1111   车偏右++
	{
		RSpeed = Car_Spend+60;
		LSpeed = Car_Spend-90;
	} 
	else if(gd == 0X7F)                          //7F---0111 1111        车偏右（FULL）
	{
		RSpeed = Car_Spend+80;
		LSpeed = Car_Spend-120;
	}  
	
    else if((((gd&0x18)==0x18)&&(((gd&0x80)==0x00)||((gd&0x01)==0x00)))|| gd==0xF8 || gd==0x1F || gd==0XF0 || gd==0x0F || gd==0x07 || gd==0xC0 || gd==0x03 || gd==0x80 || gd==0x01)  //RFID在十字路口
    {
        Track_Flag = 0;
        Stop_Flag = 1;
        Send_UpMotor(0,0);
        RFID_Angle_or_shizilukou_Flag=1;
        Count_ms = track_time_ms;
        track_time_ms=0;
        track_time_Start=0;
			sprintf((char*)Buf,"%d\r\n",Count_ms);  
      Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
    } 
    else    //正常行驶
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
	}      

	if(gd==0xFF)   //循迹灯全亮
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
		if(count > 1200)
		{
			count=0;
			Send_UpMotor(0,0);
			Track_Flag=0;
			Stop_Flag = 4;
		}
		else {
			count++;
		}			
	}
	else 
	{
		count=0;
	}
		
	if(Track_Flag != 0)
	{
		Control(LSpeed,RSpeed);
	}
}

///***************************************************************
//** 功能：     循迹函数
//** 参数：	  无参数
//** 返回值：   无
//****************************************************************/
//extern bool RFID_Track_Flag;
//void Track_Roadway(uint8_t gd)
//{
//	if(gd == 0x00)	// 循迹灯全灭 停止
//	{
//		if(RFID_Track_Flag)
//		{
//			Track_Flag = 1;
//			Stop_Flag = 0;
//		}
//		else
//		{
//			Track_Flag = 0;		
//			Stop_Flag = 1;
//			Send_UpMotor(0,0);
//		}	
//	} else if(gd==0x18)
//	{
//		LSpeed=Car_Spend;
//		RSpeed=Car_Spend;
//		
//	} else if((gd==0x08) || (gd==0x0C))
//	{
//		LSpeed=Car_Spend+20;
//		RSpeed=Car_Spend-40;
//	} else if((gd==0x04) || (gd==0x06))
//	{
//		LSpeed=Car_Spend+40;
//		RSpeed=Car_Spend-60;
//	} else if((gd==0x02) || (gd==0x03))
//	{
//		LSpeed=Car_Spend+60;
//		RSpeed=Car_Spend-90;
//		
//	} else if(gd==0x01)
//	{
//		LSpeed=Car_Spend+80;
//		RSpeed=Car_Spend-120;
//		
//	} else if((gd==0x10) || (gd==0x30))
//	{
//		RSpeed = Car_Spend+20;
//		LSpeed = Car_Spend-40;
//		
//	} else if((gd==0x20) || (gd==0x60))
//	{
//		RSpeed = Car_Spend+40;
//		LSpeed = Car_Spend-60;
//		
//	} else if((gd==0x40) || (gd==0xC0))
//	{
//		RSpeed = Car_Spend+60;
//		LSpeed = Car_Spend-90;
//		
//	} else if(gd==0X80)
//	{
//		RSpeed = Car_Spend+80;
//		LSpeed = Car_Spend-120;
//		
//	} else 
//	{
//		LSpeed=Car_Spend;
//		RSpeed=Car_Spend;
//	}
//	
//	if(gd==0xFF)   //循迹灯全亮
//	{
//		if(count > 1000)
//		{
//			count=0;
//			Send_UpMotor(0,0);
//			Track_Flag=0;
//			Stop_Flag = 4;
//		}
//		else {
//			count++;
//		}			
//	}
//	else 
//	{
//		count=0;
//	}
//		
//	if(Track_Flag != 0)
//	{
//		Control(LSpeed,RSpeed);
//	}
//}

uint16_t track_time_ms=0;
uint8_t track_time_Start=0;
void RFID_Track_Time(void)
{
    if(track_time_Start==1)
        track_time_ms++;
    else
        track_time_ms=0;
}


void roadway_check_TimInit(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM9,&TIM_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM9, ENABLE);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) == SET)
	{
		Roadway_Check();		// 路况检测
        RFID_Track_Time();
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}


//end file
