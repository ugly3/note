#ifndef _RGB_LED_H
#define _RGB_LED_H

#include "gd32f4xx.h"
#include "systick.h"

#define RGBnum 1

#define RGBled_RTC 		RCU_GPIOE
#define RGBled_GPIO_Port	GPIOE
#define RGBled_Pin 		GPIO_PIN_10


void RGBsendT1(void);
void RGBled_config(void);
void RGB_set_color(uint8_t num,uint8_t r ,uint8_t g ,uint8_t b , uint8_t brightness);
void RGB_set_color_32(uint8_t num,uint32_t c,uint8_t brightness);
void RGB_Show(void);
#endif  /* _RGB_LED_H */

