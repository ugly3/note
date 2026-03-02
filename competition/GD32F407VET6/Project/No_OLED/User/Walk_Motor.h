#ifndef __WALK_MOTOR_H
#define __WALK_MOTOR_H

#include "gd32f4xx.h"                   // Device header

#define Motor_RCU_GPIOX     RCU_GPIOE
#define Motor_PORT          GPIOE
#define Motor_PIN           GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5



void Walk_Motor_Init(void);
void Set_Walkmotor(uint8_t status);










#endif


