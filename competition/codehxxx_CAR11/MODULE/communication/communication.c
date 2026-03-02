/**
************************************************************************
 *    文件名：communication（通信部分）
 *      说明： wifi接收，红外发射，ZigBee的发送与接收
 *  相关函数：
三个发送数据的函数
1.发送数据到DeBug
Send_InfoData_To_Fifo(*p,16);                //三个函数的参数都一样，  参数：1.需要发送的数据；  2.发送数据的长度

2.通过ZigBee发送数据
Send_ZigbeeData_To_Fifo(AGV_Thread1,8);

3.通过WIFI发送数据
Send_WifiData_To_Fifo(Principal_Tab,13);

红外发送数据
Infrared_Send(uint8_t *s,int n)            //参数: *s：指向要发送的数据   n：数据长度

接收安卓端下发的数据
Can_WifiRx_Check();                             // Wifi交互数据处理

接收zigbee数据：标志物，从车
Can_ZigBeeRx_Check();                           // Zigbee交互数据处理

************************************************************************
**/
#include "communication.h"

Communication_Typedef Communication_Data = 
{
	.Voice_Report_Rx_State = 0,   //语音播报回传状态（1表示回传RTC日期，2表示回传RTC时间，3表示回传天气数据与温度数据）
	.Smart_Traffic_A_Recognition_State = 0,  //智能交通灯A识别  （1代表识别模式成功）
	.Smart_Traffic_B_Recognition_State = 0,  //智能交通灯A识别  （1代表识别模式成功）
	.ETC_Open_Flag = 0,   //ETC标志物开启标志位
	.Special_LandForm_Back_State = 0,  //特殊地形回传状态
	.CarPort_Back_B_Infrared = 0,  //车库B回传前后触发状态
	.CarPort_Back_A_Infrared = 0,  //车库A回传前后触发状态	
	.CarPort_Back_B_Level = 0,  //车库B回传层数
	.CarPort_Back_A_Level = 0,  //车库A回传层数
	.Barrier_Open_Flag = 0,     //道闸开启标志位
};

/*
Zigbees数据交互
标志物，从车，安卓的回传命令
原函数在can_user.c文件里
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(Zigb_Rx_Buf[1] == 0x03)  //********************道闸标志物
		{
			if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[4] == 0x05))  //如果进入，则代表回传到指令，道闸处于开启状态
			{
				Communication_Data.Barrier_Open_Flag = 1; 				
			}
		}
		if((Zigb_Rx_Buf[1] == CarPort_Data.Device_A || Zigb_Rx_Buf[1] == CarPort_Data.Device_B))   //**********************立体车库标志物
		{
			if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x01))         //返回车库位于第几层的数据
			{
				if(Zigb_Rx_Buf[1] == CarPort_Data.Device_A)       //判断为A设备
				{
					Communication_Data.CarPort_Back_A_Level = Zigb_Rx_Buf[4];   //返回当前车库位于第几层数据
				}
				else if(Zigb_Rx_Buf[1] == CarPort_Data.Device_B)        //判断为B设备
				{
					Communication_Data.CarPort_Back_B_Level = Zigb_Rx_Buf[4];   //返回当前车库位于第几层数据
				}
			}
			else if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x02))      //返回前/后侧红外状态
			{
				if(Zigb_Rx_Buf[1] == CarPort_Data.Device_A)         //判断为A设备
				{
					if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))  //前后侧都被触发  （工作室：前后侧都未触发）
					{
						Communication_Data.CarPort_Back_A_Infrared = 1;
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02)) //前后侧未触发  （工作室：前后侧都被触发）
					{
						Communication_Data.CarPort_Back_A_Infrared = 2;
					}
					else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))  //前侧触发，后侧未触发（工作室：前侧未触发，后侧触发）
					{
						Communication_Data.CarPort_Back_A_Infrared = 3;
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))  //前侧未触发，后侧触发（工作室：前侧未触发，后侧触发）
					{
						Communication_Data.CarPort_Back_A_Infrared = 4;
					}
					else
					{
					}
				}
				else if(Zigb_Rx_Buf[1] == CarPort_Data.Device_B )          //判断为B设备
				{
					if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))  //前后侧都被触发
					{
						Communication_Data.CarPort_Back_B_Infrared = 1;
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02)) //前后侧未触发
					{
						Communication_Data.CarPort_Back_B_Infrared = 2;
					}
					else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))  //前侧触发，后侧未触发
					{
						Communication_Data.CarPort_Back_B_Infrared = 3;
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))  //前侧未触发，后侧触发
					{
						Communication_Data.CarPort_Back_B_Infrared = 4;
					}
					else
					{
						
					}					
				}
			}
		}
		
		if(Zigb_Rx_Buf[1] == 0x10)    //*****************************特殊地形标志物
		{
			if((Zigb_Rx_Buf[2] == 0x10) == (Zigb_Rx_Buf[3] == 0x01))
			{
				if(Zigb_Rx_Buf[4] == 0x31)    
				{
					Communication_Data.Special_LandForm_Back_State = 1;      //车辆顺利通过，通行方向（A->B）
				}
				else if(Zigb_Rx_Buf[4] == 0x32) 
				{
					Communication_Data.Special_LandForm_Back_State = 2;     //车辆顺利通过，通行方向（A->B）
				}
				else if(Zigb_Rx_Buf[4] == 0x33) 
				{
					Communication_Data.Special_LandForm_Back_State = 3;     //车辆未顺利通过
				}
				else 
				{
					
				}
			}
		}
		
		if(Zigb_Rx_Buf[1] == 0x0C)   //**************************ETC系统标志物
		{
			if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[4] == 0x06) && (Zigb_Rx_Buf[5] == 0x00)&& ETC_Data.ETC_Car_Start==1)   
			{
				Communication_Data.ETC_Open_Flag = 1;        //闸门开启状态
			}
		}
		
     if((Zigb_Rx_Buf[1] == 0x0E) || (Zigb_Rx_Buf[1] == 0x0F))    //*******************智能交通灯标志物
		{
			if(Zigb_Rx_Buf[1] == 0x0E)   //设备A
			{
				if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[5] == 0x00))
				{
					if(Zigb_Rx_Buf[4] == 0x07)        //进入识别模式成功
					{
						Communication_Data.Smart_Traffic_A_Recognition_State = 1;
					}
					else if(Zigb_Rx_Buf[4] == 0x08)  //进入识别模式失败
					{
						Communication_Data.Smart_Traffic_A_Recognition_State = 0;
					}
				}
			}
			else if(Zigb_Rx_Buf[1] == 0x0F)   //设备B
			{
				if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[5] == 0x00))
				{
					if(Zigb_Rx_Buf[4] == 0x07)   //进入识别模式成功
					{
						Communication_Data.Smart_Traffic_B_Recognition_State = 1;
					}
					else if(Zigb_Rx_Buf[4] == 0x08)  //进入识别模式失败
					{
						Communication_Data.Smart_Traffic_B_Recognition_State = 0;
					}					
				}				
			}
		}
		
     if(0x06 == Zigb_Rx_Buf[1])                   //**************************语音播报标志物
		{
			if(0x02 == Zigb_Rx_Buf[2])  //语音播报回传RTC日期
			{
				Voice_Report_Data.xVoice_Report_Rx_RTC_Date[0] = Zigb_Rx_Buf[3];  //回传RTC年份
				Voice_Report_Data.xVoice_Report_Rx_RTC_Date[1] = Zigb_Rx_Buf[4];  //回传RTC月份
				Voice_Report_Data.xVoice_Report_Rx_RTC_Date[2] = Zigb_Rx_Buf[5];  //回传RTC日
				Communication_Data.Voice_Report_Rx_State = 1;  //回传RTC日期状态
			}
			else if(0x03 == Zigb_Rx_Buf[2])  //语音播报回传RTC时间
			{
				Voice_Report_Data.xVoice_Report_Rx_RTC_Time[0] = Zigb_Rx_Buf[3];  //回传RTC时
				Voice_Report_Data.xVoice_Report_Rx_RTC_Time[1] = Zigb_Rx_Buf[4];  //回传RTC分
				Voice_Report_Data.xVoice_Report_Rx_RTC_Time[2] = Zigb_Rx_Buf[5];  //回传RTC秒
				Communication_Data.Voice_Report_Rx_State = 2;  //回传RTC时间状态
			}
			else if(0x04 == Zigb_Rx_Buf[2])  //语音播报回传天气数据与温度数据（16进制，单位度）
			{
				Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0] = Zigb_Rx_Buf[3];  //回传天气数据
				Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1] = Zigb_Rx_Buf[4];  //回传温度数据
				Communication_Data.Voice_Report_Rx_State = 3;  //回传天气数据与温度数据状态
			}
			else
			{
				
			}
		}

		if(0x01 == Zigb_Rx_Buf[1])    //*****************接收从车发过来的数据
		{
			if(0xA0 == Zigb_Rx_Buf[2]) 
			{
				FollowCar_Data.FollowCar_Stat_Flag  = 1;   //如果从车启动了，标志位置1
			}
			else if(0xAA == Zigb_Rx_Buf[2])    // 从车完成任务
			{
				FollowCar_Data.FollowCar_Finish_Flag = 1;
			}
			else if(0xA2 == Zigb_Rx_Buf[2])    // 从车到达车库
			{
				FollowCar_Data.FollowCar_Arrive_CarPort = 1;
			}
      else if(0xA3 == Zigb_Rx_Buf[2])    //从车传来的车牌信息
			{
				FollowCar_Data.Follow_Send_Licence_Data_Flag = 1;
                Follow_Send_Licence_Data_Store[0] = Zigb_Rx_Buf[3];
                Follow_Send_Licence_Data_Store[1] = Zigb_Rx_Buf[3];
                Follow_Send_Licence_Data_Store[2] = Zigb_Rx_Buf[3];
                Follow_Send_Licence_Data_Store[3] = Zigb_Rx_Buf[3];
                Follow_Send_Licence_Data_Store[4] = Zigb_Rx_Buf[3];
                Follow_Send_Licence_Data_Store[5] = Zigb_Rx_Buf[3];
			}
            else if(0xA4 == Zigb_Rx_Buf[2])    //从车传来的车牌信息
            {
                FollowCar_Data.Follow_temperature_Finish_Flag=1;  
            }
		}
//		uint8_t Buf[50];
//		sprintf((char*)Buf,"data is : %x-%x-%x \r\n",Zigb_Rx_Buf[1],Zigb_Rx_Buf[2],Zigb_Rx_Buf[7]);  
//		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  
		
//		if(Zigb_Rx_Buf[7] == 0xBB)
//		{
//			memset(Zigb_Rx_Buf,0,sizeof(Zigb_Rx_Buf));  //对zigbee接收缓存清零	
//			Zigbee_Rx_flag = 0; //接收完成标志位置为1		
//		} 
		
		memset(Zigb_Rx_Buf,0,sizeof(Zigb_Rx_Buf));
        Zigbee_Rx_flag = 0;    
	}
}






uint8_t App_Rx_Flag;
void Normal_data(void)	  // 正常接收11字节控制指令
{
	uint8_t sum=0;

	if(Wifi_Rx_Buf[10] == 0xBB)	 // 判断包尾
	{									  
		//主指令与三位副指令左求和校验
		//注意：在求和溢出时应该对和做256取余。
        sum = (Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5]+Wifi_Rx_Buf[6]+Wifi_Rx_Buf[7]+Wifi_Rx_Buf[8])%256;
		if(sum == Wifi_Rx_Buf[9])
		{
			App_Rx_Flag =1;
		}
		else 
			App_Rx_Flag =0;
		
//		App_Rx_Flag =1;
	}
	else
	{
		
		App_Rx_Flag = 0;
	}
//	else if(Wifi_Rx_Buf[7] == 0xBB)
//	{
//		App_Rx_Flag =1;
//	}

}

void Abnormal_data(void)	  //数据异常处理
{
	uint8_t i,j;
	uint8_t sum = 0;
	
	if(Wifi_Rx_num < 11)			// 异常数据字节数小于11字节不做处理
	{
	   App_Rx_Flag = 0;
	}
	else 
	{
		for(i=0;i<=(Wifi_Rx_num - 10);i++)  
		{
			if(Wifi_Rx_Buf[i] == 0x55)	   // 寻找包头
			{
			   if(Wifi_Rx_Buf[i+10] == 0xBB)	// 判断包尾
			   {
						sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5]+Wifi_Rx_Buf[i+6]+Wifi_Rx_Buf[i+7]+Wifi_Rx_Buf[i+8])%256;
					 if(sum == Wifi_Rx_Buf[i+9])	 // 判断求和
						{
							 for(j=0;j<11;j++)
							 {
									Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // 数据搬移
							 }
							App_Rx_Flag =1;
						}
					 else 
						 App_Rx_Flag =0;
		     }
		   }
	   }
	}
}

uint8_t jixun=0;
uint8_t YT5_Carport=0;

/* WIFI 数据接收 （安卓与主车通信）*/
void Can_WifiRx_Check(void)
{
    uint8_t Buf1[30];
	if(Wifi_Rx_flag)
	{	
			if(Wifi_Rx_Buf[0] == 0x55)   
			{              
				Normal_data();    //数据正常11字节
			}
			else
			{
                jixun=1;
				Abnormal_data();   //数据异常   
			} 	
			Wifi_Rx_flag = 0;
	}
	
	if(App_Rx_Flag)    //处理安卓终端发过来的数据
	{
		if(Wifi_Rx_Buf[1] == 0xEE)     //安卓终端发送给主车命令
		{
			switch(Wifi_Rx_Buf[2])    //判断主指令
			{
				case 0xA0:    //主车启动 
				{
					Android_Data.Android_Main_Car_Start_Flag = 1;
					break;
				}
				case 0x01:    //红绿灯识别成功
				{
					if(Wifi_Rx_Buf[3] == 0x01)   //识别为红灯
					{
						Android_Data.Red_State = 1;
					}
					else if(Wifi_Rx_Buf[3] == 0x02)   //识别为黄灯
					{
						Android_Data.Yellow_State = 1;
					}
					else if(Wifi_Rx_Buf[3] == 0x03)   //识别为绿灯
					{
						Android_Data.Green_State = 1;
					}
					break;
				}
				case 0x02:  //识别二维码成功
				{
					Android_Data.Two_Code_State = 1;
					Two_Code_Data_Store[0] = Wifi_Rx_Buf[3];
					Two_Code_Data_Store[1] = Wifi_Rx_Buf[4];
					Two_Code_Data_Store[2] = Wifi_Rx_Buf[5];
					Two_Code_Data_Store[3] = Wifi_Rx_Buf[6];
					Two_Code_Data_Store[4] = Wifi_Rx_Buf[7];
					Two_Code_Data_Store[5] = Wifi_Rx_Buf[8];
					Two_Code_Data_Store[6] = Wifi_Rx_Buf[9];			
					break;
				}
				case 0x03:    //识别 TFT 车牌成功
				{
                    if(!(Wifi_Rx_Buf[3]==0x00))
                    {
                         Android_Data.TFT_ChePai_sucess_flag=1;
                        
                        sprintf((char*)Buf1,"%x\r\n",Wifi_Rx_Buf[3]);  
                        Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1)); 
                    }
                    else
                        Android_Data.TFT_ChePai_sucess_flag=0;
                    
					Android_Data.TFT_Licence_Flag = 1;
					TFT_License_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_License_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_License_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_License_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_License_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_License_Data_Store[5] = Wifi_Rx_Buf[8];	
					break;				
				}
				case 0x04:     //识别 TFT 交通标志物成功
				{
					Android_Data.TFT_Traffic_Flag = 1;
					TFT_Traffic_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_Traffic_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_Traffic_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_Traffic_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_Traffic_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_Traffic_Data_Store[5] = Wifi_Rx_Buf[8];
					TFT_Traffic_Data_Store[6] = Wifi_Rx_Buf[9];
					break;
				}
				case 0x05:   // 安卓回传给主车，主车控制TFT向下翻页
				{
					Android_Data.Control_TFT_Paging = 1;
					if(Wifi_Rx_Buf[3]==0x01)
									Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
					else if(Wifi_Rx_Buf[3]==0x02)
									Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_B,2);
					else if(Wifi_Rx_Buf[3]==0x03)
									Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_C,2);
					break;
				}
				case 0x06:    //TFT图形数量识别成功
				{
					Android_Data.TFT_GraphCount_Flag = 1;
					TFT_GraphCount_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_GraphCount_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_GraphCount_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_GraphCount_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_GraphCount_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_GraphCount_Data_Store[5] = Wifi_Rx_Buf[8];
					TFT_GraphCount_Data_Store[6] = Wifi_Rx_Buf[9];
					break;
				}
				case 0x07:    //TFT图形颜色识别成功 
				{
					Android_Data.TFT_GraphColour_Flag = 1;
					TFT_GraphColour_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_GraphColour_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_GraphColour_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_GraphColour_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_GraphColour_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_GraphColour_Data_Store[5] = Wifi_Rx_Buf[8];
					TFT_GraphColour_Data_Store[6] = Wifi_Rx_Buf[9];
					break;
				}
				case 0x08: //TFT图形识别成功 （数量+颜色）
				{
					Android_Data.TFT_Graph_CountAndColour_Flag = 1;
                    if(Wifi_Rx_Buf[3]==0x01)
                    {
                        TFT_GraphAndColour_Data_Store[0] = Wifi_Rx_Buf[4];
                        TFT_GraphAndColour_Data_Store[1] = Wifi_Rx_Buf[5];
                        TFT_GraphAndColour_Data_Store[2] = Wifi_Rx_Buf[6];
                    }
                    else if(Wifi_Rx_Buf[3]==0x02)
                    {
                        TFT_GraphAndColour_Data_Store[3] = Wifi_Rx_Buf[7];
                        TFT_GraphAndColour_Data_Store[4] = Wifi_Rx_Buf[8];
                        TFT_GraphAndColour_Data_Store[5] = Wifi_Rx_Buf[9];
                    }
                     sprintf((char*)Buf1,"%s\r\n",TFT_GraphAndColour_Data_Store);  
                        Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1)); 
                    
					break;
				}
			    case 0x09:  //TFT口罩数量识别成功  
				{
					Android_Data.TFT_Mask_Flag = 1;
					TFT_Mask_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_Mask_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_Mask_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_Mask_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_Mask_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_Mask_Data_Store[5] = Wifi_Rx_Buf[8];
					TFT_Mask_Data_Store[6] = Wifi_Rx_Buf[9];

					for(uint8_t i=0;i<7;i++)
					{
							sprintf((char*)Buf1,"%02x ",TFT_Mask_Data_Store[i]);  
							Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1));
					}
					break;
				}
				case 0x10: //TFT文字识别成功
				{
					Android_Data.TFT_Words_Flag = 1;
					TFT_Words_Data_Store[0] = Wifi_Rx_Buf[3];
					TFT_Words_Data_Store[1] = Wifi_Rx_Buf[4];
					TFT_Words_Data_Store[2] = Wifi_Rx_Buf[5];
					TFT_Words_Data_Store[3] = Wifi_Rx_Buf[6];
					TFT_Words_Data_Store[4] = Wifi_Rx_Buf[7];
					TFT_Words_Data_Store[5] = Wifi_Rx_Buf[8];
					TFT_Words_Data_Store[6] = Wifi_Rx_Buf[9];						
					break;
				}
                case 0x11:
                {
                    if(Wifi_Rx_Buf[3]==0x01 || Wifi_Rx_Buf[3]==0x02 || Wifi_Rx_Buf[3]==0x03 || Wifi_Rx_Buf[3]==0x04)
                    {
                        Android_Data.TFT_CheXin_sucess_flag=1;
                        if(Wifi_Rx_Buf[3]==0x01)
                            YT5_Carport=1;
                        else if(Wifi_Rx_Buf[3]==0x02)
                            YT5_Carport=2;
                        else if(Wifi_Rx_Buf[3]==0x03)
                            YT5_Carport=3;
                        else if(Wifi_Rx_Buf[3]==0x04)
                            YT5_Carport=4;
                        sprintf((char*)Buf1,"CheXin:%x\r\n",Wifi_Rx_Buf[3]);  
                        Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1));
                    }
                    else
                    {
                        Android_Data.TFT_CheXin_sucess_flag=0;

                    }
                }
			}
		}
		else   //安卓发送给主车的数据直接发送给相关的标志物（例如识别到红绿灯后直接通过主车的Zigbee发送给红绿灯）
		{
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		}
		App_Rx_Flag = 0;
	}
	memset(Wifi_Rx_Buf, 0, sizeof(Wifi_Rx_Buf));   //将接收到的数据清零
}


//uint8_t App_Rx_Flag ;


///* WIFI 数据接收 （安卓与主车通信）*/
//void Can_WifiRx_Check(void)
//{
//	if(Wifi_Rx_flag)
//	{	
//		if(Wifi_Rx_Buf[0] == 0xAA && Wifi_Rx_Buf[9] == 0xBB)
//			App_Rx_Flag = 1;
//		else
//			App_Rx_Flag = 0;
//	}
//	
//	if(App_Rx_Flag)    //处理安卓终端发过来的数据
//	{
//			switch(Wifi_Rx_Buf[1])   
//			{
//				case 0x01:    //主车启动 
//				{
//					Android_Data.Android_Main_Car_Start_Flag = 1;
//					break;
//				}
//				case 0x02:    //红绿灯识别成功
//				{
//					Android_Data.Android_Traffic_Light_Flag = 1;
//					break;
//				}
//				case 0x03:    //识别 TFT 车牌成功
//				{
//					char temp[15];
//					Android_Data.Android_TFT_Licence_Flag = 1;
//					Android_TFT_License_Data_Arr[0] = Wifi_Rx_Buf[2];
//					Android_TFT_License_Data_Arr[1] = Wifi_Rx_Buf[3];
//					Android_TFT_License_Data_Arr[2] = Wifi_Rx_Buf[4];
//					Android_TFT_License_Data_Arr[3] = Wifi_Rx_Buf[5];
//					Android_TFT_License_Data_Arr[4] = Wifi_Rx_Buf[6];
//					Android_TFT_License_Data_Arr[5] = Wifi_Rx_Buf[7];
//					Android_TFT_License_Data_Arr[6] = Wifi_Rx_Buf[8];
//								
//					Barrier_Data.xBarrier_Licence_Tx(Android_TFT_License_Data_Arr);
//					uint8_t Buf[50];
//					sprintf((char*)Buf,"License:%s\r\n",Android_TFT_License_Data_Arr);  //将错误指令打印到Debug
//					Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf)); 
//					break;
//				}
//				case 0x04:
//				{
//					
//				}
//				case 0x05:  //安卓发送给主车的数据直接发送给相关的标志物（例如识别到红绿灯后直接通过主车的Zigbee发送给红绿灯）
//				{
//					Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
//				}
//			}
//		App_Rx_Flag = 0;
//	}
//	memset(Wifi_Rx_Buf, 0, sizeof(Wifi_Rx_Buf));   //将接收到的数据清零
//}








































