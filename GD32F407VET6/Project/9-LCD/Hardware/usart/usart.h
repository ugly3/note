#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "gd32f4xx_usart.h"
#include "led.h"
#include "beep.h"

extern uint8_t rxbuffer[3];
extern uint8_t txbuffer[3];

//PA3 
#define USART_TX_RTC 		RCU_GPIOA
#define USART_TX_GPIO_Port	GPIOA
#define USART_TX_Pin 		GPIO_PIN_3
//PA4
#define USART_RX_RTC 		RCU_GPIOA
#define USART_RX_GPIO_Port	GPIOA
#define USART_RX_Pin 		GPIO_PIN_2
#define RTC_USARTx 			RCU_USART1	//APB2

#define USARTx				USART1
#define baudrate			115200
#define GPIO_AF_x			GPIO_AF_7

void USART_Init(void);
void USART_Send_Byte(uint32_t USART_ID,uint8_t data);
void USART_Send_Buff(uint32_t USART_ID,uint8_t *data,uint32_t len);
void USART_Test(void);


#endif 

