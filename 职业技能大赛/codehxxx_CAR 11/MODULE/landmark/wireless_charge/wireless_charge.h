#ifndef __WIRELESS_CHARGE_H
#define __WIRELESS_CHARGE_H
#include "all_module.h"

typedef struct
{
	void (*xWireless_Charge_Open_Close)(uint8_t);
    void (*xWireless_Charge_KaiQiMa_Open)(unsigned int*);    //uint8_t*
    void (*xWireless_Charge_KaiQiMa_XiuGai)(char*);
}Wireless_Charge_Typedef;

extern Wireless_Charge_Typedef Wireless_Charge_Data;

#define Wireless_Charge_OPEN  1  //无线充电开启
#define Wireless_Charge_CLOSE 2  //无线充电关闭


void xWireless_Charge_Open_Close(uint8_t mode);
void xWireless_Charge_KaiQiMa_Open(unsigned int*str);   //uint8_t*str
void xWireless_Charge_KaiQiMa_XiuGai(char*str);

#endif
