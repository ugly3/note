#ifndef __SMART_STREET_LIGHT_H
#define __SMART_STREET_LIGHT_H
#include "all_module.h"

typedef struct
{
	uint8_t (*xSmart_Light_Get_Init_Level)(void);
	void (*xSmart_Light_Appoint_Level)(uint8_t);
    void (*xSmart_Light_Add_Level)(uint8_t);	
}Smart_Light_Typedef;

extern Smart_Light_Typedef Smart_Light_Data;



void xSmart_Light_Add_Level(uint8_t grade);
void xSmart_Light_Appoint_Level(uint8_t appoint_grade);
uint8_t xSmart_Light_Get_Init_Level(void);


#endif
