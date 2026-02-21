#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H
#include "all_module.h"

typedef struct
{
	bool Smart_Traffic_A_Recognition_State;    
	bool Smart_Traffic_B_Recognition_State;
	bool ETC_Open_Flag;
	uint8_t Voice_Report_Rx_State;
	uint8_t Special_LandForm_Back_State;
	uint8_t CarPort_Back_B_Infrared;
	uint8_t CarPort_Back_A_Infrared;
	uint8_t CarPort_Back_B_Level;
	uint8_t CarPort_Back_A_Level;
	bool Barrier_Open_Flag;
}Communication_Typedef;

extern Communication_Typedef Communication_Data;
extern uint8_t jixun;
extern uint8_t YT5_Carport;

void Can_ZigBeeRx_Check(void);


#endif
