/**
************************************************************************
 *    文件名：etc（ETC标志物）
 *      说明：
 *  通信方式：Zigbee通信
************************************************************************
**/
#include "etc.h"

ETC_Typedef ETC_Data =
{
	.Left_Gate_Up = 1,
	.Right_Gate_Up = 2,	
	.Left_Gate_Dowm = 3,
	.Right_Gate_Dowm = 4,
    .ETC_Car_Start = 0,
	.xETC_Init_Angle_Adjust = &xETC_Init_Angle_Adjust,
	.xETC_Pass = &xETC_Pass,
    .xETC_Pass_RFID = &xETC_Pass_RFID
};




/*
ETC标志物固定指令发送 帧头1，帧头2，帧尾
*/
uint8_t ETC_Buf[8] = {0x55,0x0C,0x00,0x00,0x00,0x00,0x00,0xBB};



/*
 ***************************初始化ETC左侧和右侧闸门上升和下降*****************************************
 * 帧头1---> 0x55   * 帧头2---> 0x0C    帧尾---> 0xBB
----------------------------------------------------------------------------------------------------
   主指令 |         副指令1         |         副指令2        |   副指令3 | 说明
   0x08   |    0x01（左侧闸门上升） |   0x01（右侧闸门上升） |    0x00   | ETC系统闸门初始化角度调节
   0x08	  |    0x02（左侧闸门下降） |   0x02（右侧闸门下降） |    0x00   | ETC系统闸门初始化角度调节
****************************************************************************************************
注意：ETC系统闸门初始化角度调节的目的是控制ETC系统处于关闭状态时，左右两侧闸门处于水平状态。
（该指令未辅助功能，设备固件更新后仅需要设置一次，掉电不丢失）
参数：Left_Gate_Up（左上）--Right_Gate_Up（右上）-- Left_Gate_Dowm（左下）--Right_Gate_Dowm（右下）
返回值：无
初始化角度调节只有在ETC处于关闭状态才可以调节
*/
void xETC_Init_Angle_Adjust(uint8_t AngleAdjust)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,ETC_Buf,sizeof(ETC_Buf));
	Temp[2] = 0x08;
	if(AngleAdjust == 1)   //左侧闸门上升
	{
		Temp[3] = 0x01;
	}
	else if(AngleAdjust == 2)  //右侧闸门上升
	{
		Temp[4] = 0x01;
	}
	else if(AngleAdjust == 3)  //左侧闸门下降
	{
		Temp[3] = 0x02;
	}
	else if(AngleAdjust == 4)  //右侧闸门下降
	{
		Temp[4] = 0x02;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	Send_ZigbeeData_To_Fifo(Temp,8);
}



extern Communication_Typedef Communication_Data;
/*
通过ETC没有寻卡
*/
void xETC_Pass(void)
{
    ETC_Data.ETC_Car_Start=1;
	while(Communication_Data.ETC_Open_Flag == 0)    //ETC是关闭状态
	{	
#if ETC_Cross    //ETC 放置在十字路口
        if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
		{
            delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_Track_Go();
            delay_ms(200);
            break;
        }
        else   
		{
			Motor_Data.xCAR_Back(15,350);//往后一点
		}
        delay_ms(200);
        if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
		{
            delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_Track_Go();
            delay_ms(200);
            break;
        }
        else   
		{
            Motor_Data.xCAR_Go(18,400);//往前一点
		}
        
#endif
			
#if ETC_Middle   //ETC 放置在路中间

		if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
		{
			delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
			Motor_Data.xCAR_Track_Go(); 
            delay_ms(200);  
			break;
		}
		else   
		{
			Motor_Data.xCAR_Back(15,350);//往后一点
		}
        
		delay_ms(200);
		if(Communication_Data.ETC_Open_Flag == 1)     //ETC打开
		{
			delay_ms(250);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0; 
			Motor_Data.xCAR_Track_Go();
            delay_ms(200);  

			break;
		}
		else
		{
            Motor_Data.xCAR_Go(18,400);//往前一点
		}
#endif		
	}

#if ETC_Cross    //ETC 放置在十字路口
    if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
    {
        delay_ms(200);
        Communication_Data.ETC_Open_Flag = 0;
        ETC_Data.ETC_Car_Start = 0;
        Motor_Data.xCAR_Track_Go();
        Motor_Data.xCAR_Track_Go();
        delay_ms(200);
    }
   
#endif    
    
#if ETC_Middle   //ETC 放置在路中间	
	if(Communication_Data.ETC_Open_Flag == 1)   // 一开始就是打开状态
	{
		delay_ms(250);
        Communication_Data.ETC_Open_Flag = 0;
        ETC_Data.ETC_Car_Start = 0;
		Motor_Data.xCAR_Track_Go();
        delay_ms(200);  
	}    
#endif	
  
}

/*
过ETC同时寻卡
*/
void xETC_Pass_RFID(uint8_t card1,uint8_t card2,uint8_t card3)
{
    ETC_Data.ETC_Car_Start=1;
	while(Communication_Data.ETC_Open_Flag == 0)    //ETC是关闭状态
	{	
#if ETC_Cross    //ETC 放置在十字路口
        if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
				{
            delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
            Motor_Data.xCAR_Track_Go();		
            RFID_Data.xRFID_Track_Read(25,card1,card2,card3);  //寻卡
            delay_ms(200);
            break;
        }
        else   
				{
						Motor_Data.xCAR_Back(15,350);//往后一点
				}
        delay_ms(200);
        if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
				{
            delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
            Motor_Data.xCAR_Track_Go();
            RFID_Data.xRFID_Track_Read(25,card1,card2,card3);  //寻卡
            delay_ms(200);
            break;
        }
        else   				
				{
//						Motor_Data.xCAR_Back(15,350);
            Motor_Data.xCAR_Go(20,400);//往前一点  
				}
        
#endif
			
#if ETC_Middle   //ETC 放置在路中间

		if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
		{
			delay_ms(200);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0;
						
            Motor_Data.xCAR_Go(40,180);//向前一点，防止可能重读
            RFID_Data.xRFID_Track_Read(25,card1,card2,card3);  //寻卡
            delay_ms(200);  
			break;
		}
		else   
		{
			Motor_Data.xCAR_Back(15,350);//往后一点
		}
        
		delay_ms(200);
		if(Communication_Data.ETC_Open_Flag == 1)     //ETC打开
		{
			delay_ms(250);
            Communication_Data.ETC_Open_Flag = 0;
            ETC_Data.ETC_Car_Start = 0; 
						
            Motor_Data.xCAR_Go(40,180);//向前一点，防止可能重读
            RFID_Data.xRFID_Track_Read(25,card1,card2,card3); //寻卡
            delay_ms(200);  

			break;
		}
		else
		{
//						Motor_Data.xCAR_Back(40,180);
            Motor_Data.xCAR_Go(18,400);//往前一点
		}
#endif		
	}

#if ETC_Cross    //ETC 放置在十字路口
    if(Communication_Data.ETC_Open_Flag == 1)   //ETC打开
    {
        delay_ms(200);
        Communication_Data.ETC_Open_Flag = 0;
        ETC_Data.ETC_Car_Start = 0;
        Motor_Data.xCAR_Track_Go();
        RFID_Data.xRFID_Track_Read(25,card1,card2,card3);  //寻卡
        delay_ms(200);
    }
   
#endif    
    
#if ETC_Middle   //ETC 放置在路中间	
	if(Communication_Data.ETC_Open_Flag == 1)   // 一开始就是打开状态
	{
		delay_ms(250);
        Communication_Data.ETC_Open_Flag = 0;
        ETC_Data.ETC_Car_Start = 0;
				
        Motor_Data.xCAR_Go(40,180);//向前一点，防止可能重读
        RFID_Data.xRFID_Track_Read(25,card1,card2,card3);
        delay_ms(200);  
	}    
#endif	
  
}





