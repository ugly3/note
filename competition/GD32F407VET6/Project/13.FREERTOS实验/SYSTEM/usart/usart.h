#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h"
#include "gd32f407v_start.h"


// USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCU_USART1
#define  DEBUG_USART_BAUDRATE           115200U

// USART1 GPIO
#define  DEBUG_USART_GPIO_CLK           RCU_GPIOA

#define  DEBUG_USART_TX_GPIO_PORT       GPIOA
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_2
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_3

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

#define  DEBUG_USART_AF                 GPIO_AF_7



void DEBUG_USART_TX_IRQHandler(void);
void DEBUG_USART_RX_IRQHandler(void);
void Debug_USART_Init(uint32_t baud);


#endif


