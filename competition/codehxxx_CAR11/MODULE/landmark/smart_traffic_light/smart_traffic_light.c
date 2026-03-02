/**
************************************************************************
 *    文件名：smart_traffic_light
 *      说明：智能交通路灯标志物
 *  通信方式：Zigbee无线通信
************************************************************************
**/
#include "smart_traffic_light.h"

Smart_Traffic_Typedef Smart_Traffic_Data = 
{
	.Red_Light = 1,
	.Green_Light = 2,
	.Yellow_Light = 3,
	.Device_A = 1,
	.Device_B = 2,
	.xSmart_Traffic_Ask_State = &xSmart_Traffic_Ask_State,
	.xSmart_Traffic_Colour_Recognition = &xSmart_Traffic_Colour_Recognition,
};

/*
智能交通灯标志物固定指令发送 帧头1,帧尾
*/
uint8_t Smart_Traffic_Buf[8] = {0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB};    


/*
 ***************************请求进入识别模式************************************
 * 帧头1---> 0x55   * 帧头2--0x0E（设备A）--0x0F设备B    * 帧尾---> 0xBB
-----------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2|  副指令3  | 说明
   0x01   |  0x00  |   0x00  |    0x00   | 请求进入识别模式
************************************************************************************
参数：选择设备（A或者B）
返回值：无
*/

void xSmart_Traffic_Ask_State(uint8_t device)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	uint8_t TimeOut = 0;
	memcpy(Temp,Smart_Traffic_Buf,sizeof(Smart_Traffic_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0E;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x0F;
	}
	Temp[2] = 0x01;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //校验和
	Temp[6] = CheckSum;
	if(device == 1)  //设备A
	{
		while(Communication_Data.Smart_Traffic_A_Recognition_State == 0)  //如果未进入识别模式
		{
			Send_ZigbeeData_To_Fifo(Temp,8);   //发送请求识别模式
			delay_ms(200);
			TimeOut++;
			if(TimeOut > 15)
			{
				Communication_Data.Smart_Traffic_A_Recognition_State = 0;
				break;    //超时3s，进入识别模式失败
			}
		}
	}
	else if(device == 2)  //设备B
	{
		while(Communication_Data.Smart_Traffic_B_Recognition_State == 0)  //如果未进入识别模式
		{
			Send_ZigbeeData_To_Fifo(Temp,8);   //发送请求识别模式
			delay_ms(200);
			TimeOut++;
			if(TimeOut > 15)
			{
				Communication_Data.Smart_Traffic_B_Recognition_State = 0;
				break;//超时3s，进入识别模式失败
			}
		}   		
	}
    Communication_Data.Smart_Traffic_A_Recognition_State = 0;   //设备A识别标志位清零
	Communication_Data.Smart_Traffic_B_Recognition_State = 0;	//设备B识别标志位清零
	delay_ms(500);
	delay_ms(500);
	Android_Data.xMainCar_Send_Android(Identify_Traffic_Light);
}


/*
 ***************************（红色，绿色，黄色）识别结果请求确认*******************
 * 帧头1---> 0x55   * 帧头2--0x0E（设备A）--0x0F设备B    * 帧尾---> 0xBB
-----------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2|  副指令3  | 说明
   0x02   |  0x01  |   0x00  |    0x00   | “红色”识别结果请求确认
   0x02   |  0x02  |   0x00  |    0x00   | “绿色”识别结果请求确认
   0x02   |  0x03  |   0x00  |    0x00   | “黄色”识别结果请求确认
************************************************************************************
参数：
返回值：
*/
void xSmart_Traffic_Colour_Recognition(uint8_t device,uint8_t color)
{
	uint8_t CheckSum;
	uint8_t Temp[8];
	memcpy(Temp,Smart_Traffic_Buf,sizeof(Smart_Traffic_Buf));
	if(device == 1) //设备A
	{
		Temp[1] = 0x0E;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x0F;
	}
	Temp[2] = 0x02;  //主指令
	switch(color)
	{
		case 1:   //红色
		{
			Temp[3] = 0x01;
			break;
		}
		case 2:   //绿色
		{
			Temp[3] = 0x02;
			break;
		}
		case 3:   //黄色
		{
			Temp[3] = 0x03;
			break;
		}
		default:break;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);   //发三次数据
		delay_ms(60);
	}
	
}

//   红的校验和：0x03
//   绿的校验和：0x04
//   黄的校验和：0x05


// 0x05,0x0E,0x02,0x01,0x00,0x00,0x03,0xBB  红
// 0x05,0x0E,0x02,0x01,0x00,0x00,0x04,0xBB  绿
// 0x05,0x0E,0x02,0x01,0x00,0x00,0x05,0xBB  黄



