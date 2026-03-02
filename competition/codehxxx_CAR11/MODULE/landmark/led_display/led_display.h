#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H
#include "all_module.h"


typedef struct
{
	uint8_t TimeClose;
	uint8_t TimeStart;
	uint8_t TimeReset;
	void (*xLED_Display_Data)(uint8_t,uint8_t,uint8_t,uint8_t);
	void (*xLED_Display_Time)(uint8_t);
	void (*xLED_Display_Distance)(uint16_t);
}Led_Display_Typedef;

extern Led_Display_Typedef LED_Display_Data;


void xLED_Display_Data(uint8_t One,uint8_t Two,uint8_t Three,uint8_t Rank);
void xLED_Display_Time(uint8_t mode);
void xLED_Display_Distance(uint16_t dis);

#endif
