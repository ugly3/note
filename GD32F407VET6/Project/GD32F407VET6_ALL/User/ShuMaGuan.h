#ifndef __SHUMAGUAN_H
#define __SHUMAGUAN_H


#include "gd32f4xx.h"                   //3.3V

#define ShuMaGuan_RTC 			RCU_GPIOD
#define ShuMaGuan_GPIO_Port	    GPIOD


#define SCLK_PIN GPIO_PIN_11
#define RCLK_PIN GPIO_PIN_12
#define DIO_PIN  GPIO_PIN_13



void ShuMaGuan_Display_Init(void);
void ShuMaGuan_Display(int16_t Data);
void ShuMaGuan_OFF(void);









#endif


