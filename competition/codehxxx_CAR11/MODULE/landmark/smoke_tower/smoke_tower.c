/**
************************************************************************
 *    文件名：smoke_tower
 *      说明：烽火台（报警台）标志物
************************************************************************
**/
#include "smoke_tower.h"

SmokeTower_Typedef SmokeTower_Data = 
{
	.SmokeTower_Infrared_Open = &SmokeTower_Infrared_Open,
};

/*烽火台（报警台）标志物红外控制指令（开启码）【红外通信】*/
uint8_t SmokeTower_Infrared[6] = {0x03,0x05,0x14,0x45,0xDE,0x92};

/*烽火台（报警台）标志物发送控制命令【ZigBee通信】---帧头1，帧头2，帧尾固定*/
uint8_t SmokeTower_Zigbee[8] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB};



/*
 ******************************烽火台开启*****************************
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










