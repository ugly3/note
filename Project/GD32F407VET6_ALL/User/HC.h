#ifndef __HC_H
#define __HC_H

#include "gd32f4xx.h"                   // Device header
#include "TIM.h" 
#include "systick.h"

#define HC_RCU_GPIOX        RCU_GPIOA
#define HC_PORT             GPIOA
#define HC_PIN_Trig         GPIO_PIN_0
#define HC_PIN_Echo         GPIO_PIN_1

void HC_Init(void);
void HC_Get_Data(uint8_t status);

extern uint32_t HalTime1,HalTime2;
extern uint32_t Distance;





#endif


