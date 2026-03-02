/**
************************************************************************
 *    文件名：voice_report
 *      说明：语音播报标志物
 *  无线通信：ZigBee
************************************************************************
**/
#include "voice_report.h"



Voice_Report_Typedef Voice_Report_Data = 
{
	.xVoice_Report_Rx_Weather_Temperatur = {0},
	.xVoice_Report_Rx_RTC_Time = {0},
	.xVoice_Report_Rx_RTC_Date = {0},
	.xVoice_Report_Appoint_Command = &xVoice_Report_Appoint_Command,
	.xVoice_Report_Random_Command = &xVoice_Report_Random_Command,
	.xVoice_Report_SetRTC_Start_Date = &xVoice_Report_SetRTC_Start_Date,
	.xVoice_Report_SetRTC_Start_Time = &xVoice_Report_SetRTC_Start_Time,
	.xVoice_Report_InquireRTC_Current_Date = &xVoice_Report_InquireRTC_Current_Date,
	.xVoice_Report_InquireRTC_Current_Time = &xVoice_Report_InquireRTC_Current_Time,
	.xVoice_Report_Set_Weather_Temperatur = &xVoice_Report_Set_Weather_Temperatur,
	.xVoice_Report_Inquire_Weather_Temperatur = &xVoice_Report_Inquire_Weather_Temperatur,
	.xVoice_Report_Set_Complex_Command = &xVoice_Report_Set_Complex_Command,
	.xVoice_Report_Complex_Control = &xVoice_Report_Complex_Control,
    .xVoice_Report_Speak_Weather = &xVoice_Report_Speak_Weather,
    .xVoice_Report_Speak_temperature = &xVoice_Report_Speak_temperature,
};


//语音播报标志物固定指令发送 帧头1，帧头2，帧尾
uint8_t Voice_Report_Buf[8] = {0x55,0x06,0x00,0x00,0x00,0x00,0x00,0xBB};

/*
 *****************************播报指定语音命令*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  | 说明
   0x10   |  0x01  |   0x00  |    0x00   |  播报“技能成才”（编号：01）
   0x10   |  0x02  |   0x00  |    0x00   |  播报“匠心筑梦”（编号：02）
   0x10   |  0x03  |   0x00  |    0x00   |  播报“筑梦杨威”（编号：03）
   0x10   |  0x04  |   0x00  |    0x00   |  播报“技行天下”（编号：04）
   0x10   |  0x05  |   0x00  |    0x00   |  播报“展行业百技”（编号：05）
   0x10   |  0x06  |   0x00  |    0x00   |  播报“树人才新规”（编号：06）
******************************************************************************
注意：实际跟手册不一样，编号为00~06
参数：播报指定语音指令,命令集在voice_report.h文件里
返回值：无
*/
void xVoice_Report_Appoint_Command(uint8_t type)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x10;  //主指令
	switch(type)
	{
		case 0x01:         //“当前温度为”
		{
			Temp[3] = 0x01;
			break;
		}
		case 0x02:        //播报温度的第十位数
		{
			Temp[3] = 0x02;
			break;
		}
		case 0x03:        //“十”   
		{
			Temp[3] = 0x03;
			break;
		}
		case 0x04:            
		{
			Temp[3] = 0x04;
			break;
		}
		case 0x05:               
		{
			Temp[3] = 0x05;
			break;
		}
		case 0x06:             
		{
			Temp[3] = 0x06;
			break;
		}
    default:break;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}


/*
 *****************************播报随机语音命令*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1| 副指令2 |  副指令3  |          说明
   0x20   |  0x01  |   0x00  |    0x00   |  随机播报语音编号 01~06
******************************************************************************
参数：无
返回值：无
*/
void xVoice_Report_Random_Command(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x20;  //主指令
	Temp[3] = 0x01;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}	
}

/*
 *****************************设置RTC起始日期*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
-----------------------------------------------------------------------------
   主指令 | 副指令1     | 副指令2       |  副指令3        |   说明
   0x30   |  0xXX（年） |   0xXX（月）  |    0xXX（日）   |  设置RTC起始日期
******************************************************************************
参数：年，月，日  ----- 0x（对应你想要的数字）
参数例子：如写2023年，5月1日，  则year---0x23; month---0x05; day---0x01;
返回值：无
*/
void xVoice_Report_SetRTC_Start_Date(uint8_t year,uint8_t month,uint8_t day)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x30;  //主指令
	Temp[3] = year;  //年
	Temp[4] = month;  //月
	Temp[5] = day;  //日
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);		
	}
}

/*
 *****************************设置RTC起始时间*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
-----------------------------------------------------------------------------
   主指令 | 副指令1     | 副指令2       |  副指令3        |   说明
   0x40   |  0xXX（时） |   0xXX（分）  |    0xXX（秒）   |  设置RTC起始时间
******************************************************************************
参数：时，分，秒  ----- 0x（对应你想要的数字）
参数例子：如写16时，36分，29秒，  则hour---0x16; minute---0x36; second---0x29;
返回值：无
*/
void xVoice_Report_SetRTC_Start_Time(uint8_t hour,uint8_t minute,uint8_t second)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x40;  //主指令
	Temp[3] = hour;    //时
	Temp[4] = minute;  //分
	Temp[5] = second;  //秒
    CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);		
	}	
}


/*
语音播报标志物状态回传指令数据结构与基本控制指令数据结构一致，通信方式相同
*/
/*
 *****************************查询RTC当前日期*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
-----------------------------------------------------------------------------
   主指令 | 副指令1  | 副指令2   |  副指令3   |   说明
   0x31   |  0x01    |   0x00    |    0x00    |  查询RTC当前日期
******************************************************************************
查询RTC当前日期并且将当前日期存储在了Voice_Report_Data.xVoice_Report_Rx_RTC_Date
参数：无
返回值：1代表回传成功，0代表回传失败
*/
uint8_t xVoice_Report_InquireRTC_Current_Date(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x31;  //主指令
	Temp[3] = 0x01;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	while(Communication_Data.Voice_Report_Rx_State != 1)    //接收到RTC回传日期指令则跳出
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		TimeOut++;
		if(TimeOut >25)
		{
			return 0;
//			break;    //超时2.5s后，退出此循环，防止卡死
		}
	}
	Communication_Data.Voice_Report_Rx_State = 0;
	return 1;
}

/*
 *****************************查询RTC当前时间*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
-----------------------------------------------------------------------------
   主指令 | 副指令1  | 副指令2   |  副指令3   |   说明
   0x41   |  0x01    |   0x00    |    0x00    |  查询RTC当前时间
******************************************************************************
查询RTC当前的时间并将当前时间存储在了Voice_Report_Data.xVoice_Report_Rx_RTC_Time
参数：无
返回值：1代表回传成功，0代表回传失败
*/
uint8_t xVoice_Report_InquireRTC_Current_Time(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x41;  //主指令
	Temp[3] = 0x01;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	while(Communication_Data.Voice_Report_Rx_State != 2)   //接收到RTC回传时间指令则跳出
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		TimeOut++;
		if(TimeOut >25)
		{
			return 0;
//			break;    //超时2.5s后，退出此循环，防止卡死
		}
	}
	Communication_Data.Voice_Report_Rx_State = 0;
	return 1;
}



/*
语音播报标志物状态回传指令数据结构与基本控制指令数据结构一致，通信方式相同
*/

/*
 *****************************设置天气数据与温度数据（16进制，单位度）*******************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
-----------------------------------------------------------------------------
   主指令 |    副指令1    |    副指令2     |  副指令3   |   说明
   0x42   |  0xXX（天气） |   0xXX（温度） |    0x00    |  设置天气数据与温度数据
******************************************************************************
参数1：设置天气状态（天气状态在天气数据命令,命令集在voice_report.h文件里）
参数2：设置温度值   （十六进制，例如16度，则写--0x16）
返回值：无
*/
void xVoice_Report_Set_Weather_Temperatur(uint8_t weather,uint8_t temperatur)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x42;   //主指令
	switch(weather)
	{
		case gale:
		{
			Temp[3] = 0x00;   //大风
			break;
		}
		case cloudy:         //多云
		{
			Temp[3] = 0x01;   
			break;
		}
		case sunny:          //晴
		{
			Temp[3] = 0x02;   
			break;
		}
		case little_snow:    //小雪
		{
			Temp[3] = 0x03;    
			break;
		}
		case little_rain:     //小雨
		{
			Temp[3] = 0x04;
			break;
		}
		case overcase:        //阴天
		{
			Temp[3] = 0x05;
			break;
		}
		default:break;
	}
	Temp[4] = temperatur;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}


/*
 *****************************请求回传天气数据与温度数据***********************************
 * 帧头1---> 0x55   * 帧头2--0x06   * 帧尾---> 0xBB
------------------------------------------------------------------------------------------
   主指令 | 副指令1  |  副指令2 |  副指令3   |   说明
   0x43   |  0x00    |   0x00   |    0x00    |  请求回传天气数据与温度数据（16进制，单位度）
*******************************************************************************************
参数：无
返回值：1（表示接收到天气与温度数据）---0（表示接收失败）
*/
uint8_t xVoice_Report_Inquire_Weather_Temperatur(void)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	memcpy(Temp,Voice_Report_Buf,sizeof(Voice_Report_Buf));
	Temp[2] = 0x43;   //主指令
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;	
	while(Communication_Data.Voice_Report_Rx_State != 3)   //接收到天气与温度数据指令则跳出
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
		TimeOut++;
		if(TimeOut >25)
		{
			return 0;
//			break;    //超时2.5s后，退出此循环，防止卡死
		}
	}
	Communication_Data.Voice_Report_Rx_State = 0;
	return 1;		
}


/*
 *********************************语音合成指定数据**************************************** 
------------------------------------------------------------------------------------------
  帧头 |        数据区长度        |                      数据区
       |   高字节   |   低字节    |  命令字  |   文本编码格式    |       待合成文本 
------------------------------------------------------------------------------------------
  0xFD |    0x00    |    0x01     |    0x01  |      0xXX         | ···········
*******************************************************************************************
文本编码格式说明
  0x00   |   GB2312编码  
  0x01   |   GBK编码（GBK是汉字内码扩展规范的缩写，也叫国标码，包含简体中文和繁体中文）通常使用这个
  0x02   |   BIG5编码
  0x03   |   Unicode编码
参数：写入中文，如：  "你好世界"
返回值：
*/
void xVoice_Report_Set_Complex_Command(char*str)
{
    uint16_t str_len = strlen(str);             // 文本长度
	uint8_t Temp[5] = {0};
	Temp[0] = 0xFD;  //帧头
    Temp[1] = 0xff & ((str_len + 2) >> 8); // 数据区长度高八位
    Temp[2] = 0xff & (str_len + 2);        // 数据区长度低八位
	Temp[3] = 0x01; //命令字
	Temp[4] = 0x01; //文本编码格式，选择了GBK编码
    Send_ZigbeeData_To_Fifo(Temp, 5);      //先发送命令
    Send_ZigbeeData_To_Fifo((uint8_t *)str, str_len);  //再次发送文本内容和长度
    delay_ms(100);
}


/*
 *****************************停止，暂停，回复合成语音*********
  帧头 |        数据区长度        |   数据区
       |   高字节   |   低字节    |  命令字  
---------------------------------------------------------------
  0xFD |    0x00    |    0x01     |    0x0X  
***************************************************************
命令字说明：
  0x02  |   停止合成语音
  0x03  |   暂停合成语音
  0x04  |   恢复合成语音
参数：参考voic_report.h 语音合成控制指令
返回值：无
*/
void xVoice_Report_Complex_Control(uint8_t type)
{
	uint8_t Temp[4] = {0};
	Temp[0] = 0xFD;  //帧头
	Temp[1] = 0x00;  //高字节
	Temp[2] = 0x01;  //低字节
	switch(type)
	{
		case stop_complex:   //停止合成语音
		{
			Temp[3] = 0x02;
			break;
		}
		case pause_complex:   //暂停合成语音
		{
			Temp[3] = 0x02;
			break;
		}
		case recover_complex:  //恢复合成语音
		{
			Temp[3] = 0x02;
			break;
		}
		default:break;
	}
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,4);
		delay_ms(100);
	}
}


//小创接收公交站回传的天气并播报
void xVoice_Report_Speak_Weather(void)
{
    delay_ms(500);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]+0x04);
    delay_ms(500); 
    delay_ms(500);
    delay_ms(500);

}

//小创接收公交站回传的温度并播报
void xVoice_Report_Speak_temperature(void)
{
    delay_ms(500);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x01);//播报“当前温度为”
    delay_ms(500);
    delay_ms(500);
    delay_ms(500);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang((Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]/10)+'0');//播报十位数
    delay_ms(300);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x02);//播报“十”
    delay_ms(300);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang((Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]%10)+'0');//播报个位数
    delay_ms(300);
    XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x03);//播报“度”
    for(uint8_t i=0;i<3;i++)
    {
        delay_ms(500);
        delay_ms(500);
    }
    
}







