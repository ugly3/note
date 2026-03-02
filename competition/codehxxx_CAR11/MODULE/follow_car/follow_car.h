#ifndef __FOLLOW_CAR_H
#define __FOLLOW_CAR_H
#include "all_module.h"

typedef struct
{
	bool FollowCar_Arrive_CarPort;
	bool FollowCar_Need_Barrier_Gate;
	bool FollowCar_Stat_Flag;
	bool FollowCar_Finish_Flag;
	bool Follow_Send_Licence_Data_Flag;
	bool Follow_temperature_Finish_Flag;
	bool Follow_dis_Finish_Flag;
	bool Follow_License_Finish_Flag;
	bool Follow_Text_Finish_Flag;
	void (*xSend_Command_TO_FollowCar)(uint8_t*);
	void (*xStart_Command_To_FollowCar)(uint8_t*);
    void (*XSend_temperature_To_fromcar)(void);
	void (*XSend_dis_To_fromcar)(void);
	void (*XSend_Licence_To_fromcar)(char*str);
    void (*XSend_Licence_To_fromcar1)(char*str);
	void (*XSend_Text_To_fromcar)(const char*);
}FollowCar_Typedef;

extern FollowCar_Typedef FollowCar_Data;
extern uint8_t fromcar_start1[8];
extern uint8_t fromcar_start2[8];
extern uint8_t Follow_Send_Licence_Data_Store[6];

void xSend_Command_TO_FollowCar(uint8_t* Cmd);
void xStart_Command_To_FollowCar(uint8_t* str);
void XSend_temperature_To_fromcar(void);
void XSend_dis_To_fromcar(void);
void XSend_Licence_To_fromcar(char*str);
void XSend_Licence_To_fromcar1(char*str);
void XSend_Text_To_fromcar(const char*text);
#endif
