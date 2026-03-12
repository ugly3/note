#ifndef __THREE_DIM_CARPORT_H
#define __THREE_DIM_CARPORT_H
#include "all_module.h"

typedef struct
{
	uint8_t Device_A;
	uint8_t Device_B;
	uint8_t (*xCarPort_Ack_State)(uint8_t);
	uint8_t (*xCarPort_Ack_Infrared_State)(uint8_t);
	void (*xCarPort_Control_Arrive_Level)(uint8_t, uint8_t);
	void (*xCarPort_CarBack_Into)(uint8_t);
} CarPort_Tyepdef;

extern CarPort_Tyepdef CarPort_Data;

void xCarPort_Control_Arrive_Level(uint8_t device, uint8_t num);
uint8_t xCarPort_Ack_State(uint8_t device);
uint8_t xCarPort_Ack_Infrared_State(uint8_t device);
void xCarPort_CarBack_Into(uint8_t device);

#endif
