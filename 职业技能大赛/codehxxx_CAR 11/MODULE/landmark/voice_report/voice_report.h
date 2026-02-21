#ifndef __VOICE_REPORT_H
#define __VOICE_REPORT_H
#include "all_module.h"

/*****************************语音播报命令集*****************************************/
//播报指定语音命令
//#define   JiNengChengCai          0x01      //技能成才
//#define   JiangXinZhuMeng         0x02      //匠心筑梦
//#define   ZhuMengYangWei          0x03      //筑梦扬威
//#define   JiXingTianXia           0x04      //技行天下
//#define   ZhanHangYeBaiJi         0x05      //展行业百技
//#define   ShuRenCaiXinGui         0x06      //树人才新规

//天气数据命令
#define   gale                       1      //大风
#define   cloudy                     2      //多云
#define   sunny                      3      //晴
#define   little_snow                4      //小雪
#define   little_rain                5      //小雨
#define   overcase                   6      //阴天

//语音合成控制指令
#define   stop_complex               1      //停止合成语音
#define   pause_complex              2      //暂停合成语音
#define   recover_complex            3      //恢复合成语音
/***************************************************************************************/

typedef struct
{
	uint8_t xVoice_Report_Rx_Weather_Temperatur[2];
	uint8_t xVoice_Report_Rx_RTC_Time[3];
	uint8_t xVoice_Report_Rx_RTC_Date[3];
	void (*xVoice_Report_Appoint_Command)(uint8_t);
	void (*xVoice_Report_Random_Command)(void);
	void (*xVoice_Report_SetRTC_Start_Date)(uint8_t,uint8_t,uint8_t);
	void (*xVoice_Report_SetRTC_Start_Time)(uint8_t,uint8_t,uint8_t);
	uint8_t (*xVoice_Report_InquireRTC_Current_Date)(void);
	uint8_t (*xVoice_Report_InquireRTC_Current_Time)(void);
	void (*xVoice_Report_Set_Weather_Temperatur)(uint8_t,uint8_t);
	uint8_t (*xVoice_Report_Inquire_Weather_Temperatur)(void);
	void (*xVoice_Report_Set_Complex_Command)(char*);
	void (*xVoice_Report_Complex_Control)(uint8_t);
    void(*xVoice_Report_Speak_Weather)(void);
    void(*xVoice_Report_Speak_temperature)(void);
    
}Voice_Report_Typedef;


extern Voice_Report_Typedef Voice_Report_Data;



void xVoice_Report_Appoint_Command(uint8_t type);
void xVoice_Report_Random_Command(void);
void xVoice_Report_SetRTC_Start_Date(uint8_t year,uint8_t month,uint8_t day);
void xVoice_Report_SetRTC_Start_Time(uint8_t hour,uint8_t minute,uint8_t second);
uint8_t xVoice_Report_InquireRTC_Current_Date(void);
uint8_t xVoice_Report_InquireRTC_Current_Time(void);
void xVoice_Report_Set_Weather_Temperatur(uint8_t weather,uint8_t temperatur);
uint8_t xVoice_Report_Inquire_Weather_Temperatur(void);

void xVoice_Report_Set_Complex_Command(char*str);
void xVoice_Report_Complex_Control(uint8_t type);
void xVoice_Report_Speak_Weather(void);
void xVoice_Report_Speak_temperature(void);


#endif
