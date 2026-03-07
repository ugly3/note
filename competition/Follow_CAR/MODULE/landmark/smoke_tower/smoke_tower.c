/**
************************************************************************
 *    文件名：smoke_tower
 *      说明：烽火台（报警台）标志物
************************************************************************
**/
#include "smoke_tower.h"

SmokeTower_Typedef SmokeTower_Data = 
{
    .SmokeTower_Weizhi={0},
	.SmokeTower_Infrared_Open = &SmokeTower_Infrared_Open,
    .xSmokeTower_Zigbee_Open = &xSmokeTower_Zigbee_Open,
};

/*烽火台（报警台）标志物红外控制指令（开启码）【红外通信】*/
uint8_t SmokeTower_Infrared[6] = {0x03,0x05,0x14,0x45,0xDE,0x92};

/*烽火台（报警台）标志物发送控制命令【ZigBee通信】---帧头1，帧头2，帧尾固定*/
uint8_t SmokeTower_Zigbee[8] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB};



/*
 ******************************烽火台开启（红外通信）*****************************
参数1：SmokeTower_Infrared
返回值：无
*/
void SmokeTower_Infrared_Open(void)
{
	for(uint8_t i=0;i<3;i++)   
	{
		Infrared_Send(SmokeTower_Infrared,sizeof(SmokeTower_Infrared));   //发三次烽火台开启码
		delay_ms(100);
	}
}



/*
*********************************Zigbee发送开启码开启*************************
 * 帧头1---> 0x55   * 帧头2--0x07   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x010  |  0xXX  |   0xXX  |    0xXX   | 发送前三位开启码数据
   0x011  |  0xXX  |   0xXX  |    0xXX   | 发送后三位开启码数据
******************************************************************************
*/
void xSmokeTower_Zigbee_Open(uint8_t *str)    
{
    uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,SmokeTower_Zigbee,sizeof(SmokeTower_Zigbee));
    Temp[2] = 0x10;  //主指令
    Temp[3] = *(str+0);
    Temp[4] = *(str+1);
    Temp[5] = *(str+2);
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
    Temp[6] = CheckSum;
    for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
    Temp[2] = 0x11;  //主指令
    Temp[3] = *(str+3);
    Temp[4] = *(str+4);
    Temp[5] = *(str+5);
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
    Temp[6] = CheckSum;
    for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}
/*
*********************************Zigbee发送请求回传随机救援位置坐标点************************
 * 帧头1---> 0x55   * 帧头2--0x07   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x009  |  0x00  |   0x00  |    0x00   | 请求回传随机救援位置坐标点
******************************************************************************
*/
void xSmokeTower_Zigbee_Weizhi(void)    
{
    uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,SmokeTower_Zigbee,sizeof(SmokeTower_Zigbee));
    Temp[2] = 0x09;  //主指令
    Temp[3] = 0x00;
    Temp[4] = 0x00;
    Temp[5] = 0x00;
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
    Temp[6] = CheckSum;
    for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}





