#ifndef _LED_H
#define _LED_H

#include "gd32f4xx.h"
#include "systick.h"

#define LED1_RTC 		RCU_GPIOB
#define LED1_GPIO_Port	GPIOB
#define LED1_Pin 		GPIO_PIN_2

#define LED2_RTC 		RCU_GPIOE
#define LED2_GPIO_Port	GPIOE
#define LED2_Pin 		GPIO_PIN_7

#define LED3_RTC 		RCU_GPIOE
#define LED3_GPIO_Port	GPIOE
#define LED3_Pin 		GPIO_PIN_8

#define LED4_RTC 		RCU_GPIOE
#define LED4_GPIO_Port	GPIOE
#define LED4_Pin 		GPIO_PIN_9

void LED_config(void);

 
#endif  /* _LED_H */

