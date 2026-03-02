#ifndef __HC595_H
#define __HC595_H

#include "gd32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "systick.h"

#define OE_RTC 		RCU_GPIOD
#define OE_GPIO_Port 	GPIOD
#define OE_Pin 		GPIO_PIN_0

#define RCLK_RTC 		RCU_GPIOD
#define RCLK_GPIO_Port 	GPIOD
#define RCLK_Pin 		GPIO_PIN_2

#define SCLK_RTC 		RCU_GPIOD
#define SCLK_GPIO_Port 	GPIOD
#define SCLK_Pin 		GPIO_PIN_1

#define SER_RTC 	  	RCU_GPIOD
#define SER_GPIO_Port	GPIOD
#define SER_Pin 		GPIO_PIN_3

#define RCLK_H gpio_bit_set(RCLK_GPIO_Port,RCLK_Pin)
#define RCLK_L gpio_bit_reset(RCLK_GPIO_Port,RCLK_Pin)

#define SCLK_H gpio_bit_set(SCLK_GPIO_Port,SCLK_Pin)
#define SCLK_L gpio_bit_reset(SCLK_GPIO_Port,SCLK_Pin)

#define SER_H  gpio_bit_set(SER_GPIO_Port,SER_Pin)
#define SER_L  gpio_bit_reset(SER_GPIO_Port,SER_Pin)



void HC595_Init(void);
void HC595_Send_16Bit(uint16_t data);
void HC595_OUT(uint8_t* data);


void Matrix_LED_RIGHT_LEFT_Run_Display(unsigned char *data, unsigned int len);


extern unsigned char DZLL[224];
extern uint8_t love[8];
#endif 


