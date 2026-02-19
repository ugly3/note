#ifndef __IR_H
#define __IR_H

#include "gd32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "systick.h"
#include "oled.h"

#define IR_RTC 			RCU_GPIOD
#define IR_GPIO_Port	GPIOD
#define IR_Pin 			GPIO_PIN_3

#define	IR	gpio_input_bit_get(IR_GPIO_Port,IR_Pin) 

void IR_Init(void);
void IR_Test(void);

#endif 

