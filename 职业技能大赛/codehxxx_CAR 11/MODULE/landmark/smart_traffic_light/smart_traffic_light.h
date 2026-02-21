#ifndef __SMART_TRAFFIC_LIGHT_H
#define __SMART_TRAFFIC_LIGHT_H
#include "all_module.h"

typedef struct
{
	uint8_t Red_Light;
	uint8_t Green_Light;
	uint8_t Yellow_Light;
	uint8_t Device_A;
	uint8_t Device_B;
	void (*xSmart_Traffic_Ask_State)(uint8_t);
	void (*xSmart_Traffic_Colour_Recognition)(uint8_t,uint8_t);
}Smart_Traffic_Typedef;

extern Smart_Traffic_Typedef Smart_Traffic_Data;




void xSmart_Traffic_Ask_State(uint8_t device);
void xSmart_Traffic_Colour_Recognition(uint8_t device,uint8_t color);








#endif
