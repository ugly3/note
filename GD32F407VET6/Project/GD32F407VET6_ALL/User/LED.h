#ifndef __LED_H
#define __LED_H


#include "gd32f4xx.h"                   // Device header



#define LED_RCU_GPIOX   RCU_GPIOB
#define LED_PORT        GPIOB
#define LED_PIN         GPIO_PIN_4
#define LED_ON		1
#define LED_OFF		0

void LED_Init(void);
void set_led(uint8_t status);
void led_roll(uint8_t count);










#endif


