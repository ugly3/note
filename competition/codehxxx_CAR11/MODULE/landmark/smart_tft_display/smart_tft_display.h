#ifndef __SMART_TFT_DISPLAY_H
#define __SMART_TFT_DISPLAY_H
#include "all_module.h"

typedef struct
{
	uint8_t Device_A;
	uint8_t Device_B;
	uint8_t Device_C;
	void (*xSmart_TFT_Image_Display)(uint8_t,uint8_t);
	void (*xSmart_TFT_Image_Up_Dowm_Auto)(uint8_t,uint8_t);
	void (*xSmart_TFT_Licence_Display)(uint8_t,char*);
	void (*xSmart_TFT_Time_Diaplay)(uint8_t,uint8_t);
	void (*xSmart_TFT_Hex_Diaplay)(uint8_t,char*);
	void (*xSmart_TFT_Distance_Diaplay)(uint8_t,uint16_t);
	void (*xSmart_TFT_Traffic_Flag_Diaplay)(uint8_t,uint8_t);
}Smart_TFT_Typedef;

extern Smart_TFT_Typedef Smart_TFT_Data;



void xSmart_TFT_Image_Display(uint8_t device,uint8_t num);
void xSmart_TFT_Image_Up_Dowm_Auto(uint8_t device,uint8_t mode);
void xSmart_TFT_Licence_Display(uint8_t device,char*str);
void xSmart_TFT_Time_Diaplay(uint8_t device,uint8_t mode);
void xSmart_TFT_Hex_Diaplay(uint8_t device,char*str);
void xSmart_TFT_Distance_Diaplay(uint8_t device,uint16_t dis);
void xSmart_TFT_Traffic_Flag_Diaplay(uint8_t device,uint8_t flag);




#endif
