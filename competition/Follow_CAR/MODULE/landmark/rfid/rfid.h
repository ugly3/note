#ifndef __RFID_H
#define __RFID_H
#include "all_module.h"

#define RFID_Debug 0   //数据显示到Debug显示屏

typedef struct
{
	void (*xRFID_Track_Read)(uint8_t,uint8_t,uint8_t,uint8_t);
    void (*xRFID_Track_Read_L)(uint8_t,uint16_t,uint8_t);
	void (*xRFID_Track_Write)(uint8_t,uint8_t);
	void (*xRFID_Write)(uint8_t);
	void (*xRFID_Read)(uint8_t);
}RFID_Typedef;


extern RFID_Typedef RFID_Data;
extern uint8_t RFID_Angle_or_shizilukou_Flag;
extern bool RFID_Track_Flag;
extern int card2_sector_block;  //卡2的地址
extern char card2_position[3];      //卡2在地图上的坐标



//还未完成
extern bool RFID_Read_Flag;
extern uint16_t Distance;
extern bool Check_Distance;

void RFID_Track(uint8_t speed);
void xRFID_Track_Read(uint8_t speed,uint8_t card1,uint8_t card2,uint8_t card3);
void xRFID_Track_Read_L(uint8_t speed,uint16_t time,uint8_t card);
void xRFID_Track_Write(uint8_t speed,uint8_t card);
void xRFID_Write(uint8_t add);
void xRFID_Read(uint8_t add);
void YT1_Handle_Card1_Data(char *data);
void YT2_Handle_Card1_Data(char *data);
void YT3_Handle_Card1_Data(void);
void YT5_Handle_Card1_Data(char *data);
#endif

