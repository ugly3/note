/**
************************************************************************
 *    文件名：special_landform 
 *      说明：特殊地形标志物
 *  通信方式：Zigbee通信
************************************************************************
**/
#include "special_landform.h"

#define Special_LanForm_Debug 1

Special_LandForm_Typedef Special_LanForm_Data = 
{
	.xSpecial_LandForm_Ack_State = &xSpecial_LandForm_Ack_State,
	.xSpecial_LandForm_Pass = &xSpecial_LandForm_Pass
};


/*
特殊地形标志物 帧头1，帧头2，帧尾固定
*/
uint8_t Speical_LanForm_Buf[8] = {0x55,0x10,0x10,0x01,0x00,0x00,0x00,0xBB};  //初始化为查询车辆通行状态




/*
 ******************************查询车辆通行状态（得出特殊地形标志物通过状态）*****************************
* 帧头1---> 0x55   * 帧头2---0x10    帧尾---> 0xBB
---------------------------------------------------------------------------------
   主指令 |   副指令1     |   副指令2  |   副指令3  |     说明
   0x10   |    0x01       |   0x00     |     0x00   | 查询车辆通行状态（请求回传命令）
   0x10	  |    0x01       |   0x31     |     0x00   | 车辆顺利通过（通行方向A->B）
   0x10   |    0x01       |   0x32     |     0x00   | 车辆顺利通过（通行方向B->A）
   0x10   |    0x01       |   0x33     |     0x00   | 车辆未顺利通过
**********************************************************************************
注意：特殊地形标志物不主动回传通行状态，需通过指令请求回传
参数：无
返回值：1（回传成功）---0（回传失败）
*/
uint8_t xSpecial_LandForm_Ack_State(void)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	uint8_t TimeOut = 0;
	memcpy(Temp,Speical_LanForm_Buf,sizeof(Speical_LanForm_Buf));
	
	
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum; //计算校验和
	while(Communication_Data.Special_LandForm_Back_State == 0)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		TimeOut++;
		if(TimeOut >25)
		{
			return 0;    //超时2.5s后返回0，回传失败
		}
	}
//#if Special_LanForm_Debug  // 打印回传特殊地形标志物的状态
//	uint8_t Buf[50];
//	sprintf((char*)Buf,"SpecialLandFormState:%d\n",Communication_Data.Special_LandForm_Back_State);
//	Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
//#endif	
	Communication_Data.Special_LandForm_Back_State = 0;
	return 1;            //通过成功，回传成功，回传1
}


/*
穿越特殊地形标志物
*/
void xSpecial_LandForm_Pass(void)
{
	Motor_Data.xCAR_Back(20,500);
    delay_ms(300); //延时
    xCAR_Track_Time(20,800);
    delay_ms(300); //延时
    Motor_Data.xCAR_Back(20,500);
    delay_ms(300); //延时
	Motor_Data.xCAR_Track_White(25);  //循迹到白线就停
	delay_ms(500); //延时
	xCAR_Go(25,700);
    xCAR_Go(25,550);
	
}



