#ifndef __BODY_HW_H
#define __BODY_HW_H

#include "gd32f4xx.h"                   // Device header

#define Body_Hw_GPIOX       RCU_GPIOE
#define Body_Hw_PORT        GPIOE
#define Body_Hw_PIN         GPIO_PIN_15

void Body_Hw_Init(void);
void Body_Hw_ZD(uint8_t status);















#endif

