#ifndef __RFID_H
#define __RFID_H
#include "all_module.h"

#define RFID_Debug 0   //数据显示到Debug显示屏

typedef struct
{
	void (*xRFID_Track_Read_1)(uint8_t,uint8_t);
    void (*xRFID_Track_Read_2)(uint8_t,uint8_t,uint8_t,uint8_t);
    void (*xRFID_Track_Read_2_1)(uint8_t,uint8_t,uint8_t,uint8_t);
    void (*xRFID_Track_Read_L1)(uint8_t,uint16_t,uint8_t);
    void (*xRFID_Track_Read_L2)(uint8_t,uint16_t,uint8_t,uint8_t,uint8_t);
	void (*xRFID_Track_Write)(uint8_t,uint8_t);
	void (*xRFID_Write)(uint8_t);
	void (*xRFID_Read_1)(uint8_t);
    void (*xRFID_Read_2)(uint8_t,uint8_t,uint8_t);
}RFID_Typedef;


extern RFID_Typedef RFID_Data;
extern volatile uint8_t RFID_Angle_or_shizilukou_Flag;
extern bool RFID_Track_Flag;
extern int card2_sector_block;  //卡2的地址
extern char card2_position[3];      //卡2在地图上的坐标

extern uint8_t READ_RFID_Store[17];		// 存放RFID读取到的数据
extern char Judge_READ_RFID[16];// 存放RFID读到的数据,用来判断卡片的真假 
extern char READ_RFID1[16];     //第一张卡有效数据
extern char READ_RFID2[16];     //第二张卡有效数据
extern char READ_RFID3[16];     //第三张卡有效数据
extern char Init_READ_RFID1[17];    
extern char Init_READ_RFID2[17];    
extern char Init_READ_RFID3[17];

extern uint8_t WRITE_RFID[16];     //存放RFID写入的数据
extern unsigned int hex_codes[3];//无线充电站开启码

extern char Judge_Card1_Str[];
extern char Judge_Card2_Str[];

//还未完成
extern bool RFID_Read_Flag;
extern uint16_t Distance;
extern bool Check_Distance;

void RFID_Track(uint8_t speed);
void xRFID_Track_Read_1(uint8_t speed,uint8_t card1);
void xRFID_Track_Read_2(uint8_t speed,uint8_t judge_block_card,uint8_t real_block_card1,uint8_t real_block_card2);
void xRFID_Track_Read_2_1(uint8_t speed,uint8_t judge_block_card,uint8_t real_block_card1,uint8_t real_block_card2);
void xRFID_Track_Read_L1(uint8_t speed,uint16_t time,uint8_t card1);
void xRFID_Track_Read_L2(uint8_t speed,uint16_t time,uint8_t judge_block_card,uint8_t real_block_card1,uint8_t real_block_card2);
void xRFID_Track_Write(uint8_t speed,uint8_t card);
void xRFID_Write(uint8_t add);
void xRFID_Read_1(uint8_t add);
void xRFID_Read_2(uint8_t judge_block_card,uint8_t real_block_card1,uint8_t real_block_card2);


void YT1_Handle_Card1_Data(char *data);
void YT2_Handle_Card1_Data(char *data);
void YT3_Handle_Card1_Data(void);
void YT5_Handle_Card1_Data(char *data);
#endif

