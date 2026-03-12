/**
************************************************************************
*    文件名： Main_car（主车部分）
*      说明： 主车与从车之间的通信
************************************************************************
**/
#include "Main_car.h"

MainCar_Typedef MainCar_Data =
	{
		.MainCar_Start_Finish_Flag = 0,		 // 主车完成启动标志位，置1启动
		.MainCar_Send_Licence_Data_Flag = 0, // 主车完成发送车牌给从车标志位
		.xSend_Command_To_MainCar = &xSend_Command_To_MainCar,
		.xStart_Command_To_MainCar = &xStart_Command_To_MainCar, // 发送启动命令给主车
		.XSend_dis_To_MainCar = &XSend_dis_To_MainCar,
		.XSend_Licence_To_MainCar = &XSend_Licence_To_MainCar,
		.XSend_Licence_To_MainCar1 = &XSend_Licence_To_MainCar1,
		.XSend_Text_To_MainCar = &XSend_Text_To_MainCar,
};
uint8_t MainCar_Send_Licence_Data_Store[6] = {0}; // 存储主车发送过来的车牌号
uint8_t MainCar_Send_Init_light = 0;			  // 存储主车传来的路灯初始档位信息
uint8_t MainCar_Send_Cengshu = 0;				  // 存储主车发送的车库层数

uint8_t MainCar_start[8] = {0x55, 0x01, 0xA0, 0x01, 0x00, 0x00, 0x00, 0xBB};		 // 主车启动
uint8_t FllowCar_start_Finish[8] = {0x55, 0x01, 0xA0, 0x02, 0x00, 0x00, 0x00, 0xBB}; // 从车已经启动

uint8_t Send_temperature_To_MainCar[8] = {0x55, 0x01, 0xA0, 0xA2, 0x00, 0x00, 0x00, 0xBB};			// 发送温度数据给主车
uint8_t Send_dis_To_MainCar[8] = {0x55, 0x01, 0xA0, 0xA3, 0x00, 0x00, 0x00, 0xBB};					// 发送距离数据给主车
uint8_t Send_Licence_To_MainCar[8] = {0x55, 0x01, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xBB};				// 发送前三位车牌给主车
uint8_t Send_Licence_To_MainCar1[8] = {0x55, 0x01, 0xA2, 0x00, 0x00, 0x00, 0x00, 0xBB};				// 发送后三位车牌给主车
uint8_t Send_Text_To_MainCar[8] = {0x55, 0x01, 0xA3, 0x00, 0x00, 0x00, 0x00, 0xBB};					// 发送文字数据给主车
uint8_t Send_wireless_open_To_MainCar[8] = {0x55, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0xBB};		// 发送无线充电标志物开启码给主车
uint8_t Send_Weizhi_To_MainCar[8] = {0x55, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0xBB};				// 发送位置给主车
uint8_t Send_Algorithm_Parameters_To_MainCar[8] = {0x55, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xBB}; // 发送算法参数给主车

#define MAX_LICENCE_LENGTH 8 // 假设车牌长度不超过 8 个字符
#define MAX_TEXT_LENGTH 20	 // 假设文字信息的最大长度为 20 个字符

/* 【从车发送给主车的格式】
帧头第一位--0x55 帧头第二位--0x01
主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX
校验和
帧尾--0xBB
*/

/* 【从车接收主车的格式】
帧头第一位--0x55 帧头第二位--0x02
主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX
校验和
帧尾--0xBB
*/

/*
附录1：（从车发送给主车，主车接收从车发送的指令）
帧头第一位--0x55,帧头第二位--0x01(主车)，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x01     |   0x00    | 0x00      |发送主车启动命令
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x02     |   0x00    | 0x00      |发送从车完成启动命令
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA1   |	0xA2     |   0xXX    | 0xXX      |发送温度数据给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA2   |	0xA3     |   0xXX    | 0xXX      |发送距离数据给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA3   |	0x00     |   0x00    | 0x00      |发送车牌前三位给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA4   |	0x00     |   0x00    | 0x00      |发送车牌后三位给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA5   |	0xXX     |   0xXX    | 0xXX      |发送文字数据给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0x01   |	0xXX     |   0xXX    | 0xXX      |发送无线充电标志物开启码给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0x02   |	0xXX     |   0xXX    | 0xXX      |发送位置给主车
|--------|-----------|-----------|-----------|-----------------------------------
| 0x03   |	0xXX     |   0xXX    | 0xXX      |发送算法参数给主车
|--------|-----------|-----------|-----------|-----------------------------------
*/

/*
附录2：（主车发给从车，从车接收主车发过来的指令）
帧头第一位--0x55,帧头第二位--0x02(从车)，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x01     |   0x00    | 0x00      |接收后从车启动
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x02     |   0x00    | 0x00      |主车已经启动
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA1   |	0xA2     |   0xXX    | 0xXX      |接收主车发送的温度数据
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA2   |	0xA3     |   0xXX    | 0xXX      |接收主车发送的距离数据
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA3   |	0xXX     |   0xXX    | 0xXX      |接收主车发的车牌前三位数据
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA4   |	0xXX     |   0xXX    | 0xXX      |接收主车发的车牌后三位数据
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA5   |	0xXX     |   0xXX    | 0xXX      |接收主车发送的文字数据
|--------|-----------|-----------|-----------|-----------------------------------
| 0x01   |	0xXX     |   0xXX    | 0xXX      |接收主车发送的车库层数
|--------|-----------|-----------|-----------|-----------------------------------
*/

void xSend_Command_To_MainCar(uint8_t *Cmd)
{
	uint8_t CheckSum;
	CheckSum = Mixture_Data.xGet_CheckSum(Cmd[2], Cmd[3], Cmd[4], Cmd[5]);
	Cmd[6] = CheckSum;
	Send_ZigbeeData_To_Fifo(Cmd, 8);
	delay_ms(50);
}

// 发送命令让主车启动
void xStart_Command_To_MainCar(void)
{
	uint8_t TimeOut = 0;
	while (!MainCar_Data.MainCar_Start_Finish_Flag) // 从车没有启动，执行此循环
	{
		MainCar_Data.xSend_Command_To_MainCar(MainCar_start);
		TimeOut++;
		delay_ms(300);
		if (TimeOut >= 5)
		{
			break;
		}
	}
	MainCar_Data.MainCar_Start_Finish_Flag = 0;
}

void XSend_dis_To_MainCar(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum; // 校验和
	uint8_t TimeOut = 0;
	memcpy(Temp, Send_dis_To_MainCar, sizeof(Send_dis_To_MainCar));
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]); // 计算校验和
	Temp[6] = CheckSum;

	while (!MainCar_Data.MainCar_dis_Finish_Flag) // 从车没有启动，执行此循环
	{
		MainCar_Data.xSend_Command_To_MainCar(Temp);
		TimeOut++;
		delay_ms(300);
		if (TimeOut >= 5)
		{
			break;
		}
	}
	MainCar_Data.MainCar_dis_Finish_Flag = 0;
}

void XSend_Licence_To_MainCar(char *str)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp, Send_Licence_To_MainCar, sizeof(Send_Licence_To_MainCar));
	Temp[2] = 0xA1; // 先发送前三位
	Temp[3] = TFT_License_Data_Store[0];
	Temp[4] = TFT_License_Data_Store[1];
	Temp[5] = TFT_License_Data_Store[2];
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]); // 计算校验和
	Temp[6] = CheckSum;
	while (!MainCar_Data.MainCar_License_Finish_Flag) // 从车没有启动，执行此循环
	{
		MainCar_Data.xSend_Command_To_MainCar(Temp);
		TimeOut++;
		delay_ms(300);
		if (TimeOut >= 5)
		{
			break;
		}
	}
	MainCar_Data.MainCar_License_Finish_Flag = 0; // 重置标志位
}

void XSend_Licence_To_MainCar1(char *str)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp, Send_Licence_To_MainCar1, sizeof(Send_Licence_To_MainCar1));

	Temp[2] = 0xA2; // 再发送后三位
	Temp[3] = TFT_License_Data_Store[3];
	Temp[4] = TFT_License_Data_Store[4];
	Temp[5] = TFT_License_Data_Store[5];
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]); // 计算校验和
	Temp[6] = CheckSum;
	while (!MainCar_Data.MainCar_License_Finish_Flag) // 从车没有启动，执行此循环
	{
		MainCar_Data.xSend_Command_To_MainCar(Temp);
		TimeOut++;
		delay_ms(300);
		if (TimeOut >= 5)
		{
			break;
		}
	}
	MainCar_Data.MainCar_License_Finish_Flag = 0; // 重置标志位
}

void XSend_Text_To_MainCar(const char *text)
{
	uint8_t Temp[30] = {0}; // 缓冲区，假设协议头尾加上文字信息不超过 30 字节
	uint8_t CheckSum;		// 校验和
	uint8_t TimeOut = 0;
	Temp[0] = 0x55; // 帧头1
	Temp[1] = 0x02; // 帧头2
	size_t length = strlen(text);
	if (length > MAX_TEXT_LENGTH)
	{
		length = MAX_TEXT_LENGTH; // 限制最大长度
	}
	memcpy(Temp + 2, text, length);											   // 将文字信息存储在 Temp[2] 开始的位置
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]); // 计算校验和
	Temp[2 + length] = CheckSum;											   // 校验和存储在车牌信息之后
	Temp[2 + length + 1] = 0xBB;											   // 帧尾
	while (!MainCar_Data.MainCar_Text_Finish_Flag)							   // 从车没有启动，执行此循环
	{
		MainCar_Data.xSend_Command_To_MainCar(Temp);
		TimeOut++;
		delay_ms(300);
		if (TimeOut >= 5)
		{
			break;
		}
	}
	MainCar_Data.MainCar_Text_Finish_Flag = 0;
}
