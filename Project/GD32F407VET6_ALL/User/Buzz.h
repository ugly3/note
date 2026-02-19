#ifndef __BUZZE_H
#define __BUZZE_H


#include "gd32f4xx.h"                   // Device header



#define Buzze_RCU_GPIOX   RCU_GPIOA
#define Buzze_PORT        GPIOA
#define Buzze_PIN         GPIO_PIN_15
#define Buzze_ON		1
#define Buzze_OFF		0

void Buzze_Init(void);
void Set_Buzze(uint8_t status);
void Buzze_roll(uint8_t count);











#endif




