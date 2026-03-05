/**
************************************************************************
 * 帧头1---> 0x55   * 帧头2---> 0x03    帧尾---> 0xBB
 *    文件名：barrier_gate（道闸标志物）
 *      说明：关于道闸
 *  通信方式：ZigBee
************************************************************************
**/
#include "barrier_gate.h"




Barrier_Tyepdef Barrier_Data = 
{
	.Barrier_Gate_OPEN = 1,
	.Barrier_Gate_CLOSE = 2,
	.xBarrier_Open_Close = &xBarrier_Open_Close,
	.xBarrier_Up_Dowm = &xBarrier_Up_Dowm,
	.xBarrier_Licence_Tx = &xBarrier_Licence_Tx,
	.xBarrier_Ack_State = &xBarrier_Ack_State
};


/*道闸标志物固定指令发送：帧头1，帧头2，帧尾*/
uint8_t Barrier_Buf[8] = {0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xBB};


/*
 ***************************道闸开启和关闭*************************
 * 帧头1---> 0x55   * 帧头2---> 0x03    帧尾---> 0xBB
--------------------------------------------------------------------
   主指令 |   副指令1       |   副指令2  |   副指令3  | 说明
   0x01   |    0x01（开启） |   0x00     |     0x00   | 道闸闸门控制
   0x01	  |    0x02（关闭） |   0x00     |     0x00   | 道闸闸门控制
********************************************************************
参数：Barrier_Gate_OPEN（道闸开启）；  Barrier_Gate_CLOSE（道闸关闭）；
返回值：无
*/
void xBarrier_Open_Close(uint8_t OC)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t num = 2;
	uint8_t TimeOut = 0;
	
	memcpy(Temp,Barrier_Buf,sizeof(Barrier_Buf));
	Temp[2] = 0x01;  //主指令
	if(OC == 1)
	{
		Temp[3] = 0x01;   //道闸开启
		Temp[4] = 0x00;
		Temp[5] = 0x00;
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
      	Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);  //发送开启命令
		delay_ms(100);
		while(num != 1)
		{
			num = Barrier_Data.xBarrier_Ack_State();
			if(num == 1)   //为1时代表道闸已经打开
			{
				break;
			}
			else    //道闸关闭中
			{
				Send_ZigbeeData_To_Fifo(Temp,8);   //重新发送开启命令
			}
			delay_ms(300);
			TimeOut++;
			if(TimeOut > 15)   //超时10s，退出
			{
				TimeOut = 0;
		         break;
			}
		}		
	}
	else if(OC == 2)
	{
		Temp[3] = 0x02;   //道闸关闭
		Temp[4] = 0x00;
		Temp[5] = 0x00;
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
      	Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);  //发送开启命令
		delay_ms(100);
		while(num != 0)
		{
 			num = Barrier_Data.xBarrier_Ack_State();
			if(num == 0)   //为1时代表道闸已经关闭
			{
				break;
			}
			else    //道闸开启中
			{
				Send_ZigbeeData_To_Fifo(Temp,8);   //重新发送关闭命令
			}
			delay_ms(500);
			TimeOut++;
			if(TimeOut > 20)   //超时10s，退出
			{
				TimeOut = 0;
		         break;
			}
		}		

	}
	delay_ms(300);
}



/*
 ******************************道闸闸门上升和下降*************************
 * 帧头1---> 0x55   * 帧头2---> 0x03    帧尾---> 0xBB
--------------------------------------------------------------------------
   主指令 |   副指令1       |   副指令2  |   副指令3  |       说明
   0x09   |    0x01（上升） |   0x00     |     0x00   | 道闸闸门初始化角度
   0x09	  |    0x02（下降） |   0x00     |     0x00   | 道闸闸门初始化角度
**************************************************************************
参数：
返回值：无
*/

void xBarrier_Up_Dowm(uint8_t UD)   //用于初始化角度，很少使用
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t X=2;
	
	memcpy(Temp,Barrier_Buf,sizeof(Barrier_Buf));
	Temp[2] = 0x09;  //主指令
	if(UD == Barrier_Data.Barrier_Gate_OPEN)    //选择道闸开启
	{
		Temp[3] = 0x01;
		Temp[4] = 0x00;
		Temp[5] = 0x00;
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
		Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		while(X!=1)
		{
			X=Barrier_Data.xBarrier_Ack_State();  //请求道闸回传命令
			if(X == 1)   //处于打开状态
			{
				break;
			}
			else         //处于关闭状态
			{
				Send_ZigbeeData_To_Fifo(Temp,8);  //若未打开，再发一次开启命令
			}
			break;
		}
	}
	else if(UD == Barrier_Data.Barrier_Gate_CLOSE) //选择道闸关闭
	{
		Temp[3] = 0x02;   
		Temp[4] = 0x00;
		Temp[5] = 0x00;
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
		Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		while(X!=0)
		{
			X=Barrier_Data.xBarrier_Ack_State();  //请求道闸回传命令
			if(X == 0)   //处于关闭状态
			{
				break;
			}
			else   
			{
				Send_ZigbeeData_To_Fifo(Temp,8);  //若未关闭，在发一次关闭命令
			}
			break;
		}
	}	
	delay_ms(500);
}

/*
 ************************道闸系统标志物显示车牌*************************
 * 帧头1---> 0x55   * 帧头2---> 0x03    帧尾---> 0xBB
------------------------------------------------------------------------
   主指令 |   副指令1 |   副指令2  |   副指令3  |   说明
   0x10   |    0xXX   |   0xXX     |     0xXX   | 车牌前三位数据（ASCII）
   0x11	  |    0xXX   |   0xXX     |     0xXX   | 车牌后三位数据（ASCII）
************************************************************************
参数： 六位车牌（如："AAA123"）
返回值：无
发送这个指令同时还会打开道闸
*/
void xBarrier_Licence_Tx(char* licence)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	bool Flag = 0;
	
	memcpy(Temp,Barrier_Buf,sizeof(Barrier_Buf));    
	while(TimeOut < 3)
	{
		Temp[2] = 0x10;    //先发送前三位
		Temp[3] = *(licence+0);	
		Temp[4] = *(licence+1);
		Temp[5] = *(licence+2);
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
		Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(200);
		Temp[2] = 0x11;    //再发送后三位
		Temp[3] = *(licence+3);	
		Temp[4] = *(licence+4);
		Temp[5] = *(licence+5);
		CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
		Temp[6] = CheckSum;
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(200);
		Flag = Barrier_Data.xBarrier_Ack_State();
		if(Flag) //判断道闸是否打开，如果打开则直接退出，如果还在关闭就一直循环，知道超时
		{
			break;
		}
		delay_ms(100);
		Flag = Barrier_Data.xBarrier_Ack_State();
		if(Flag) //判断道闸是否打开，如果打开则直接退出，如果还在关闭就一直循环，知道超时
		{
			break;
		}
		delay_ms(100);
		Flag = Barrier_Data.xBarrier_Ack_State();
		if(Flag) //判断道闸是否打开，如果打开则直接退出，如果还在关闭就一直循环，知道超时
		{
			break;
		}
		TimeOut++;      //超时1.8s后退出，
		if(TimeOut == 3)
		{
			Barrier_Data.xBarrier_Open_Close(Barrier_Data.Barrier_Gate_OPEN);   //如果超时后还未打开道闸，则强制打开道闸
            break;
		}
	}	
}

/*   
 ************************请求回传道闸状态*******************************
 * 帧头1---> 0x55   * 帧头2---> 0x03    帧尾---> 0xBB
------------------------------------------------------------------------
   主指令 |   副指令1 |   副指令2  |   副指令3  |   说明
   0x20   |    0x01   |   0x00     |     0x00   | 请求回传道闸状态
------------------------------------------------------------------------
   主指令 |   副指令1 |   副指令2  |   副指令3  |   说明
   0x01   |    0x00   |   0x05     |     0x00   | 接收到的回传指令,如果接收到，代表道闸处于开启状态                                                                 
************************************************************************
参数： 
返回值：无
*/
/*
注意：道闸处于关闭状态时请求回传状态，不会回任何指令
发送道闸回传指令，得到的主指令，副1，2，3分别为 0x01,0x00,0x05,0x00
*/
uint8_t xBarrier_Ack_State(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	
	memcpy(Temp,Barrier_Buf,sizeof(Barrier_Buf));
	Temp[2] = 0x20;
	Temp[3] = 0x01;
    Temp[4] = 0x00;
	Temp[5] = 0x00;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);  //计算校验和
	Temp[6] = CheckSum;
	Send_ZigbeeData_To_Fifo(Temp,8);
	delay_ms(100);
	if(Communication_Data.Barrier_Open_Flag)   //道闸打开
	{
		Communication_Data.Barrier_Open_Flag = 0;
		return 1;
	}
    return 0;	 //返回0表示道闸关闭，返回1表示道闸开启
}
