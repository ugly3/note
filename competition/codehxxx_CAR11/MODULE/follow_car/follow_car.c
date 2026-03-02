/**
************************************************************************
 *    文件名： follow_car（从车部分）
 *      说明： 主车与从车之间的通信
************************************************************************
**/
#include "follow_car.h"

FollowCar_Typedef FollowCar_Data = 
{
	.FollowCar_Arrive_CarPort = 0,  //从车到达车库
	.FollowCar_Need_Barrier_Gate = 0,    //从车需要打开道闸
	.FollowCar_Stat_Flag = 0,    //从车启动标志位，置1启动
	.FollowCar_Finish_Flag = 0,   //从车完成任务
    .Follow_Send_Licence_Data_Flag = 0,//从车完成发送车牌给主车标志位
    .Follow_temperature_Finish_Flag = 0,//从车成功接收到主车发过去的温度
	.xSend_Command_TO_FollowCar = xSend_Command_TO_FollowCar,   //不用
	.xStart_Command_To_FollowCar = &xStart_Command_To_FollowCar, //发送启动命令给从车
    .XSend_temperature_To_fromcar = &XSend_temperature_To_fromcar,
    .XSend_dis_To_fromcar = &XSend_dis_To_fromcar,
	.XSend_Licence_To_fromcar = &XSend_Licence_To_fromcar,
    .XSend_Licence_To_fromcar1 = &XSend_Licence_To_fromcar1,
	.XSend_Text_To_fromcar = XSend_Text_To_fromcar,
};
uint8_t Follow_Send_Licence_Data_Store[6] = {0};

uint8_t fromcar_start1[8] = {0x55,0x02,0xA0,0xA0,0x00,0x00,0x00,0xBB};// 从车第一次启动    A0
//uint8_t fromcar_sta8rt2[8] = {0x55,0x02,0xA0,0xA1,0x00,0x00,0x00,0xBB};// 从车第二次启动
uint8_t Send_temperature_To_fromcar[8] = {0x55,0x02,0xA0,0xA2,0x00,0x00,0x00,0xBB};//发送温度数据给从车    A2     
uint8_t Send_dis_To_fromcar[8] = {0x55,0x02,0xA0,0xA3,0x00,0x00,0x00,0xBB};//发送距离数据给从车         A3    
uint8_t Send_Licence_To_fromcar[8] = {0x55,0x02,0xA1,0x00,0x00,0x00,0x00,0xBB};//发送前三位车牌给从车      0xB8,0x54,0x2D
uint8_t Send_Licence_To_fromcar1[8] = {0x55,0x02,0xA2,0x00,0x00,0x00,0x00,0xBB};//发送后三位车牌给从车 
uint8_t Send_Text_To_fromcar[8] = {0x55,0x02,0xA3,0x00,0x00,0x00,0x00,0xBB};  //发送文字数据给从车


#define MAX_LICENCE_LENGTH 8  // 假设车牌长度不超过 8 个字符
#define MAX_TEXT_LENGTH 20  // 假设文字信息的最大长度为 20 个字符


/* 【主车发送给从车的格式】
帧头第一位--0x55 帧头第二位--0x02 
主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX 
校验和 
帧尾--0xBB
*/

/* 【主车接收从车的格式(即从车发送给主车的格式)】
帧头第一位--0x55 帧头第二位--0x01 
主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX 
校验和 
帧尾--0xBB
*/

/*
附录1：
帧头第一位--0x55,帧头第二位--0x02(从车)，帧尾--0xBB  （主车发送给从车，从车接收主车发送的指令）
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x00     |   0x00    | 0x00      |发送从车第一次启动命令
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0xA1     |   0x00    | 0x00      |发送从车第二次启动命令
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA1   |	0xB8     |   0x54    | 0x2D      |发送车牌给从车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0xA2     |   0xXX    | 0xXX      |备用----发送温度数据给从车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0xA3     |   0xXX    | 0xXX      |备用----发送距离数据给从车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA3   |	0xXX     |   0xXX    | 0xXX      |备用----发送文字数据给从车
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA5   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/


/*
附录2：
帧头第一位--0x55,帧头第二位--0x01(从车)，帧尾--0xBB   （从车发给主车，主车接收从车发过来的指令）
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x00     |   0x00    | 0x00      |从车已经启动
|--------|-----------|-----------|-----------|-----------------------------------
| 0xAA   |	0xXX     |   0xXX    | 0xXX      |从车完成任务
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA2   |	0xXX     |   0xXX    | 0xXX      |从车到达车库后，主车控制车库到达指定位置
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA3   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA4   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA5   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/


void xSend_Command_TO_FollowCar(uint8_t* Cmd)
{
	uint8_t CheckSum;
	CheckSum = Mixture_Data.xGet_CheckSum(Cmd[2], Cmd[3], Cmd[4], Cmd[5]);
	Cmd[6] = CheckSum;
	Send_ZigbeeData_To_Fifo(Cmd, 8);
	delay_ms(50);	
}



//发送启动命令给从车
void xStart_Command_To_FollowCar(uint8_t* str)
{
    uint8_t Temp[8] = {0};
	uint8_t CheckSum;   //校验和
	uint8_t TimeOut = 0;
    memcpy(Temp,fromcar_start1,sizeof(Send_temperature_To_fromcar));
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]);   //计算校验和
	Temp[6] = CheckSum;
	while(!FollowCar_Data.FollowCar_Stat_Flag)     //从车没有启动，执行此循环
	{
		FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
		TimeOut++;
		delay_ms(300);
		if(TimeOut >= 5)
		{
			break;
		}
	}
	FollowCar_Data.FollowCar_Stat_Flag = 0;    
}

void XSend_temperature_To_fromcar(void)
{
    uint8_t Temp[8] = {0};
    uint8_t CheckSum;   //校验和
		uint8_t TimeOut = 0;
    memcpy(Temp,Send_temperature_To_fromcar,sizeof(Send_temperature_To_fromcar));
    Temp[4] = Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1];
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]);   //计算校验和
	Temp[6] = CheckSum;
    
	while(!FollowCar_Data.Follow_temperature_Finish_Flag)     //从车没有启动，执行此循环
	{
		FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
		TimeOut++;
		delay_ms(300);
		if(TimeOut >= 5)
		{
			break;
		}
	}
	FollowCar_Data.Follow_temperature_Finish_Flag=0;
}


void XSend_dis_To_fromcar(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;   //校验和
	uint8_t TimeOut = 0;
	memcpy(Temp,Send_dis_To_fromcar,sizeof(Send_dis_To_fromcar));
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]);   //计算校验和
	Temp[6] = CheckSum;
    
	while(!FollowCar_Data.Follow_dis_Finish_Flag)     //从车没有启动，执行此循环
	{
		FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
		TimeOut++;
		delay_ms(300);
		if(TimeOut >= 5)
		{
			break;
		}
	}
	FollowCar_Data.Follow_dis_Finish_Flag=0;
}

void XSend_Licence_To_fromcar(char*str)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp,Send_Licence_To_fromcar,sizeof(Send_Licence_To_fromcar));
		Temp[2] = 0xA1;    //先发送前三位
		Temp[3] = TFT_License_Data_Store[0];	
		Temp[4] = TFT_License_Data_Store[1];
		Temp[5] = TFT_License_Data_Store[2];
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
		Temp[6] = CheckSum;
		while(!FollowCar_Data.Follow_License_Finish_Flag)     //从车没有启动，执行此循环
		{
			FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
			TimeOut++;
			delay_ms(300);
			if(TimeOut >= 5)
			{
				break;
			}
		}
	FollowCar_Data.Follow_License_Finish_Flag = 0;  // 重置标志位
}
	
void XSend_Licence_To_fromcar1(char*str)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
    memcpy(Temp,Send_Licence_To_fromcar1,sizeof(Send_Licence_To_fromcar1));
		
		Temp[2] = 0xA2;    //再发送后三位
		Temp[3] = TFT_License_Data_Store[3];
		Temp[4] = TFT_License_Data_Store[4];
		Temp[5] = TFT_License_Data_Store[5];
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
		Temp[6] = CheckSum;
		while(!FollowCar_Data.Follow_License_Finish_Flag)     //从车没有启动，执行此循环
		{
			FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
			TimeOut++;
			delay_ms(300);
			if(TimeOut >= 5)
			{
				break;
			}
		}
	FollowCar_Data.Follow_License_Finish_Flag = 0;  // 重置标志位
}	


void XSend_Text_To_fromcar(const char* text)
{
	uint8_t Temp[30] = {0};   // 缓冲区，假设协议头尾加上文字信息不超过 30 字节
	uint8_t CheckSum;   //校验和
	uint8_t TimeOut = 0;
	Temp[0] = 0x55;  // 帧头1
	Temp[1] = 0x02;  // 帧头2
	size_t length = strlen(text);
	if (length > MAX_TEXT_LENGTH)
	{
			length = MAX_TEXT_LENGTH;  // 限制最大长度
	}
  memcpy(Temp + 2, text, length);  // 将文字信息存储在 Temp[2] 开始的位置
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2], Temp[3], Temp[4], Temp[5]);   //计算校验和
	Temp[2 + length] = CheckSum;  // 校验和存储在车牌信息之后
	Temp[2 + length + 1] = 0xBB;  // 帧尾
	while(!FollowCar_Data.Follow_Text_Finish_Flag)     //从车没有启动，执行此循环
	{
		FollowCar_Data.xSend_Command_TO_FollowCar(Temp);
		TimeOut++;
		delay_ms(300);
		if(TimeOut >= 5)
		{
			break;
		}
	}
	FollowCar_Data.Follow_Text_Finish_Flag=0;
}






































