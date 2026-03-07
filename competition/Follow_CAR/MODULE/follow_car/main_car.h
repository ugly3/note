#ifndef __MAIN_CAR_H
#define __MAIN_CAR_H
#include "all_module.h"

typedef struct
{
	bool MainCar_Start_Finish_Flag;
	bool MainCar_Send_Licence_Data_Flag;
	bool MainCar_dis_Finish_Flag;
	bool MainCar_License_Finish_Flag;
	bool MainCar_Text_Finish_Flag;
	void (*xSend_Command_To_MainCar)(uint8_t*);
	void (*xStart_Command_To_MainCar)(void);
	void (*XSend_dis_To_MainCar)(void);
	void (*XSend_Licence_To_MainCar)(char*);
    void (*XSend_Licence_To_MainCar1)(char*);
	void (*XSend_Text_To_MainCar)(const char*);
}MainCar_Typedef;

extern MainCar_Typedef MainCar_Data;
extern uint8_t MainCar_start[8];
extern uint8_t FllowCar_start_Finish[8];
extern uint8_t MainCar_Send_Licence_Data_Store[6];
extern uint8_t MainCar_Send_Cengshu;
extern uint8_t Send_wireless_open_To_MainCar[8];  //发送无线充电标志物开启码给主车
extern uint8_t Send_Weizhi_To_MainCar;//发送位置给主车

void xSend_Command_To_MainCar(uint8_t* Cmd);
void xStart_Command_To_MainCar(void);
void XSend_dis_To_MainCar(void);
void XSend_Licence_To_MainCar(char*str);
void XSend_Licence_To_MainCar1(char*str);
void XSend_Text_To_MainCar(const char*text);



#endif
