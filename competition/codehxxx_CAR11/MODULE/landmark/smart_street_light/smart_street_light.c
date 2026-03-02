/**
************************************************************************
 *    文件名：smart_street_light 
 *      说明：智能路灯标志物
 *  通信方式：红外无线通信
************************************************************************
**/
#include "smart_street_light.h"


Smart_Light_Typedef Smart_Light_Data = 
{
	.xSmart_Light_Get_Init_Level = &xSmart_Light_Get_Init_Level,
	.xSmart_Light_Appoint_Level = &xSmart_Light_Appoint_Level,
	.xSmart_Light_Add_Level = &xSmart_Light_Add_Level,
};
#include <stm32f4xx.h>


/*
智能路灯标志物固定指令发送 帧头1，帧头2，无帧尾
*/
uint8_t Smart_Light_Buf[4] = {0x00,0xFF,0x00,0x00}; 

//保存获取的四个光度值
uint16_t Smart_Light_Temp[4] = {0};   //保存光度值一定要用uint16_t


/*
 **********************************光源当为控制*****************************
 * 帧头1---> 0x00  * 帧头2---> 0xFF    
----------------------------------------------------------------------------------------------------
  数据 |数据反码| 说明
  0x0C |   0xF3 | 光源挡位加一挡
  0x18 |   0xE7 | 光源挡位加二挡
  0x5E |   0xA1 | 光源挡位加三挡
****************************************************************************************************
参数：
返回值：无
*/
void xSmart_Light_Add_Level(uint8_t grade)
{
	uint8_t Temp[4] = {0};
	memcpy(Temp,Smart_Light_Buf,sizeof(Smart_Light_Buf));
	
	switch(grade)
	{
		case 1:   //光源挡位+1
		{
			Temp[2] = 0x0C;  //数据
			Temp[3] = 0xF3;  //数据反码
			break;
		}
		case 2:   //光源挡位+2
		{
			Temp[2] = 0x18;  //数据
			Temp[3] = 0xE7;  //数据反码
            break;			
		}
		case 3:  //光源挡位+3
		{
			Temp[2] = 0x5E; //数据
			Temp[3] = 0xA1;	//数据反码		
			break;
		}
		default:
			break;	
	}
	Infrared_Send(Temp,4);  //发送红外数据
	delay_ms(200);
}

/*
功能：判断当前挡位，调节到指定的挡位
参数：
返回值：
*/
void xSmart_Light_Appoint_Level(uint8_t appoint_grade)
{
	uint16_t Temp;
//	uint8_t Buf[50];
	uint8_t Begin_val;
//	uint8_t End_val;
	for(uint8_t i=0;i<4;i++)
	{
		Smart_Light_Temp[i]=Get_Bh_Value();
		Smart_Light_Data.xSmart_Light_Add_Level(1);  //+1挡位
		delay_ms(500);
		delay_ms(500);   //延时1秒
	}
	Temp = Smart_Light_Temp[0];   //保存第一次读取到的值
//	sprintf((char*)Buf,"%d %d %d %d \n",Smart_Light_Temp[0],Smart_Light_Temp[1],Smart_Light_Temp[2],Smart_Light_Temp[3]);
//	Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
	Mixture_Data.xBubble_Sort(Smart_Light_Temp,4);   //冒泡排序从小到大
	for(uint8_t i=0;i<4;i++)   //确认最初挡位值
	{
		if(Smart_Light_Temp[i] == Temp)   //判断哪个光读值与第一次保存的光度值相等
		{
			Begin_val = i+1;    //得出初始挡位值
			break;
		}
	}
	switch(appoint_grade)  
	{
		case 1:   // 将挡位调到1挡
		{
			if(Begin_val == 1)  //如果初始值为1挡，则直接退出
			{
				break;
			}
			else if(Begin_val == 2)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(3);  //如果初始值为2挡，则加三档回到1挡				
			}
			else if(Begin_val == 3)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(2);  			
			}
			else if(Begin_val == 4)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(1);  			
			}
			break;
		}
		case 2:   // 将挡位调到2挡
		{
			if(Begin_val == 1)  
			{
				Smart_Light_Data.xSmart_Light_Add_Level(1);
			}
			else if(Begin_val == 2)
			{
				break;			
			}
			else if(Begin_val == 3)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(3);  			
			}
			else if(Begin_val == 4)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(2);  			
			}
			break;
		}
		case 3:   // 将挡位调到3挡
		{
			if(Begin_val == 1) 
			{
				Smart_Light_Data.xSmart_Light_Add_Level(2);
			}
			else if(Begin_val == 2)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(1);			
			}
			else if(Begin_val == 3)
			{
				break;  			
			}
			else if(Begin_val == 4)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(3);  			
			}
			break;
		}
		case 4:   // 将挡位调到4挡
		{
			if(Begin_val == 1) 
			{
				Smart_Light_Data.xSmart_Light_Add_Level(3);
			}
			else if(Begin_val == 2)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(2);			
			}
			else if(Begin_val == 3)
			{
				Smart_Light_Data.xSmart_Light_Add_Level(1);	 			
			}
			else if(Begin_val == 4)
			{
				break;   			
			}
			break;
		}
		default:
			break;
	}	
}


/*
获取初始挡位
返回值：初始挡位
*/
uint8_t xSmart_Light_Get_Init_Level(void)
{
	uint16_t Temp_val[4] = {0};  //保存智能路灯4个挡位的值     
	uint16_t Temp_Begin_val;
	uint8_t back_Init_val;
	for(uint8_t i=0;i<4;i++)   //每1s读取一个挡位
	{
		Temp_val[i] = Get_Bh_Value();
		Smart_Light_Data.xSmart_Light_Add_Level(1);	//+1档
		delay_ms(500);
		delay_ms(500);						
	}
	Temp_Begin_val = Temp_val[0];   //获取初始挡位
	for(uint8_t i=0;i<4;i++)
	{
		if(Temp_val[i] == Temp_Begin_val)
		{
			back_Init_val = i+1;
			return back_Init_val;
		}
	}
	return 0;
}

