/**
************************************************************************
 *    文件名： wireless_charge
 *      说明： 无线充电标志物
************************************************************************
**/
#include "wireless_charge.h"


Wireless_Charge_Typedef Wireless_Charge_Data = 
{
	.xWireless_Charge_Open_Close = &xWireless_Charge_Open_Close,
    .xWireless_Charge_KaiQiMa_Open = &xWireless_Charge_KaiQiMa_Open,
    .xWireless_Charge_KaiQiMa_XiuGai = &xWireless_Charge_KaiQiMa_XiuGai,
};

/*
无线充电标志物固定指令发送 帧头1，帧头2，帧尾
*/
uint8_t Wireless_Charge_Buf[8] = {0x55,0x0A,0x00,0x00,0x00,0x00,0x00,0xBB};   

/*
 *****************************开启，关闭无线充电功能**************************
 * 帧头1---> 0x55   * 帧头2--0x0A   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x01   |  0x01  |   0x00  |    0x00   | 开启无线充电功能
   0x01   |  0x02  |   0x00  |    0x00   | 关闭无线充电功能
******************************************************************************
注意：无线充电标志物开启后持续10秒，期间可使用关闭指令控制标志物关闭无线充电，
若10秒内未收到关闭指令，则无线充电自动关闭
参数1：Wireless_Charge_OPEN（开启无线充电） --- Wireless_Charge_CLOSE（关闭无线充电）
返回值：无
*/
void xWireless_Charge_Open_Close(uint8_t mode)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Wireless_Charge_Buf,sizeof(Wireless_Charge_Buf));
	Temp[2] = 0x01;  //主指令
	if(Wireless_Charge_OPEN == mode)  //无线充电开启
	{
		Temp[3] = 0x01;
	}
	else if(Wireless_Charge_CLOSE == mode)  //无线充电关闭
	{
		Temp[3] = 0x02;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}


/*
*********************************开启码开启*************************
 * 帧头1---> 0x55   * 帧头2--0x0A   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x02   |  0xXX  |   0xXX  |    0xXX   | 开启码开启无线充电功能
******************************************************************************
*/
void xWireless_Charge_KaiQiMa_Open(unsigned int*str)    
{
    uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Wireless_Charge_Buf,sizeof(Wireless_Charge_Buf));
    Temp[2] = 0x02;  //主指令
    Temp[3] = 0xA1;
    Temp[4] = 0X23;
    Temp[5] = 0XB4;
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
    Temp[6] = CheckSum;
    for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}



/*
*********************************修改开启码*************************
 * 帧头1---> 0x55   * 帧头2--0x0A   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x03   |  0xXX  |   0xXX  |    0xXX   | 修改开启码
******************************************************************************
*/
void xWireless_Charge_KaiQiMa_XiuGai(char*str)   
{
    uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Wireless_Charge_Buf,sizeof(Wireless_Charge_Buf));
    Temp[2] = 0x03;  //主指令
    Temp[3] = 0xA1;
    Temp[4] = 0x23;
    Temp[5] = 0xB4;
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
    Temp[6] = CheckSum;
    for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}













