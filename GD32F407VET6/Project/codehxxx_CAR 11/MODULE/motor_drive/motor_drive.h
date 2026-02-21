#ifndef __MOTOR_DRIVE_H
#define __MOTOR_DRIVE_H
#include "all_module.h"

#define CAR_CODE 0

/*官方主车前进后退原代码*/
#if CAR_CODE
void Car_Go(uint8_t speed, uint16_t temp);
void Car_Back(uint8_t speed, uint16_t temp);
#endif



typedef struct
{
	
	bool Track_Back_Flag;
	
	uint16_t GO_Time;
	uint16_t xLR90_time;
	uint16_t xLR45_time;
	uint8_t xLR90_speed;
	uint8_t xLR45_speed;
	uint8_t Go_speed;
  uint16_t Go_Mp_disance;
  bool Track_White_Flag;	
	void (*xCAR_Track)(uint8_t);
	void (*xCAR_Track_Time)(uint8_t,uint16_t);
	void (*xCAR_Go)(uint8_t,uint16_t);
	void (*xCAR_Back)(uint8_t,uint16_t);
	void (*xCAR_Track_Go)(void);
	void (*xCAR_L90)(uint8_t,uint16_t);
	void (*xCAR_R90)(uint8_t,uint16_t);
	void (*xCAR_L45)(uint8_t,uint16_t);
	void (*xCAR_R45)(uint8_t,uint16_t);
	void (*xCAR_Track_Correct_White)(uint8_t);
	void (*xCAR_Track_White)(uint8_t);
}Motor_Typedef;


extern Motor_Typedef Motor_Data;

void xCAR_Track(uint8_t speed);
void xCAR_Track_Time(uint8_t speed,uint16_t time);
void xCAR_Go(uint8_t speed,uint16_t time);
void xCAR_Back(uint8_t speed,uint16_t time);
void xCAR_Track_Go(void);
void xCAR_L90(uint8_t speed,uint16_t time);
void xCAR_R90(uint8_t speed,uint16_t time);
void xCAR_L45(uint8_t speed,uint16_t time);
void xCAR_R45(uint8_t speed,uint16_t time);
void xCAR_Track_Correct_White(uint8_t gd);
void xCAR_Track_White(uint8_t speed);

void xCAR_Track_Back_Time(uint8_t speed,uint16_t time);

#endif
