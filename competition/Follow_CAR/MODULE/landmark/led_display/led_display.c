/**
************************************************************************
 * 帧头1---> 0x55   * 帧头2---> 0x04    帧尾---> 0xBB
 *    文件名：led_display（LED显示标志物）
 *      说明：
 *  通信方式：ZigBee
************************************************************************
**/
#include "led_display.h"


/*
 **********LED显示标志物控制指令数据结构*************
 * 帧头1---> 0x55   * 帧头2---> 0x04    帧尾---> 0xBB
--------------------------------------------------------
   主指令 |    指令说明
   0x01   |    第一排数码管显示指定数据
   0x02	  |    第二排数码管显示指定数据
   0x03	  |    第一排数码管显示计时模式
   0x04	  |    第二排数码管显示距离模式
*******************************************************
*/
/*LED显示标志物固定指令发送：帧头1，帧头2，帧尾*/
uint8_t LED_Diaplay_Buf[8]={0x55,0x04,0x00,0x00,0x00,0x00,0x00,0xBB};


Led_Display_Typedef LED_Display_Data = 
{
	.TimeClose = 0,
	.TimeStart = 1,
	.TimeReset = 2,
	.xLED_Display_Data = &xLED_Display_Data,
	.xLED_Display_Time = &xLED_Display_Time,
	.xLED_Display_Distance = &xLED_Display_Distance
};


/*
 *****************LED显示数据***************************
 * 帧头1---> 0x55   * 帧头2---> 0x04    帧尾---> 0xBB
--------------------------------------------------------
   主指令 |   副指令1  |   副指令2  |   副指令3  | 
   0x01   |    0xXX    |   0xXX     |     0xXX   | 显示时是直接显示XX
   0x02	  |    0xXX    |   0xXX     |     0xXX   | 显示时是直接显示XX
   0x01   |    第一排数码管显示指定数据
   0x02	  |    第二排数码管显示指定数据
*********************************************************
*/
/**********************************************************************
 * 函 数 名 ：  LED显示标志物显示数据
 * 参    数 ：  One,Two,Three   数据（十六进制显示格式）
                Rank    1 -> 第一行数码管显示数据
                        2 -> 第二行数码管显示数据
 * 返 回 值 ：  无
**********************************************************************/
void xLED_Display_Data(uint8_t One,uint8_t Two,uint8_t Three,uint8_t Rank)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;    //校验和
	
	memcpy(Temp,LED_Diaplay_Buf,sizeof(LED_Diaplay_Buf));   //将帧头和帧尾赋给要发送的值
	if(Rank == 1)
	{
		Temp[2] = 0x01;
	}
	else if(Rank == 2)
	{
		Temp[2] = 0x02;
	}
	Temp[3] = One; 
	Temp[4] = Two;
	Temp[5] = Three;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)  //发三次数据
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
	delay_ms(500);  //如果需要连续发第一排和第二排则需要延时500ms以上，否则会出现第二排改变而第一排不改变；
}



/*
 *****************LED计时关闭，开启，清零***************
 * 帧头1---> 0x55   * 帧头2---> 0x04    帧尾---> 0xBB
--------------------------------------------------------
   主指令 |        副指令1       |   副指令2  |   副指令3  | 
   0x03   |    0x00  [计时关闭]  |   0x00     |     0x00   | 
   0x03	  |    0x01  [计时开启]  |   0x00     |     0x00   | 
   0x03	  |    0x02  [计时清零]  |   0x00     |     0x00   | 
   0x03	  |    第一排数码管显示计时模式  
*********************************************************
*/
/*************************************************************************************
 * 函 数 名 ：  LED计时
 * 参    数 ： 模式（1--关闭； 2---开启； 3---清零）
 * 返 回 值 ：  TimeClose（计时关闭）；  TimeStart（计时开启）    TimeReset（计时清零）
**************************************************************************************/
void xLED_Display_Time(uint8_t mode)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;    //校验和
	
	memcpy(Temp,LED_Diaplay_Buf,sizeof(LED_Diaplay_Buf));
	Temp[2] = 0x03; //主命令
	Temp[4] = 0x00;
	Temp[5] = 0x00;
	switch(mode)
	{
		case 0:
		{
			Temp[3] = 0x00;  //计时器关闭
			break;
		}
		case 1:
		{
			Temp[3] = 0x01; //计时器开启
			break;
		}
		case 2:
		{
			Temp[3] = 0x02; //计时器清零
			break;
		}
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;  //校验和
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
	delay_ms(300);
}



/*
 *****************LED显示距离模式***************
 * 帧头1---> 0x55   * 帧头2---> 0x04    帧尾---> 0xBB
--------------------------------------------------------
   主指令 |  副指令1  |   副指令2  |   副指令3  | 
   0x04   |    0x00   |   0x0X     |     0xXX   | 
   0x04	  |    第二排数码管显示距离模式   第二排显示距离
注意：第二位和第三位副指令中 “X”为要显示距离值，单位为毫米，显示格式为十进制；
*********************************************************
*/
/**********************************************************************
 * 函 数 名 ：  LED显示标志物显示测距信息 （第二排显示距离，前三个数码管显示JL-表示距离）
 * 参    数 ：  dis 距离值
 * 返 回 值 ：  无 
**********************************************************************/
void xLED_Display_Distance(uint16_t dis)
{
	uint8_t Temp[8] = {0};  
	uint8_t CheckSum;
	
	memcpy(Temp,LED_Diaplay_Buf,sizeof(LED_Diaplay_Buf));
	Temp[2] =  0x04; 
	Temp[3] =  0x00;
	Temp[4] = (uint8_t)(dis / 100 % 10);   //将距离的百位转换为十六进制
	Temp[5] = ((dis / 10 % 10) * 16 + (dis % 10));   //将距离的的低位转换为十六进制
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
	delay_ms(500);
}



