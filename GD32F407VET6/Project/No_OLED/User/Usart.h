#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"                   // Device header


#define RX_PACKET_SIZE 100 // 定义接收缓冲区大小

// PD5(TX),PD6(RX)
#define USART_TX_RTC 		RCU_GPIOD
#define USART_TX_GPIO_Port	GPIOD
#define USART_TX_Pin 		GPIO_PIN_5

#define USART_RX_RTC 		RCU_GPIOD
#define USART_RX_GPIO_Port	GPIOD
#define USART_RX_Pin 		GPIO_PIN_6
#define RTC_USARTx 			RCU_USART1

#define USARTx				USART1
#define baudrate			9600
#define GPIO_AF_x			GPIO_AF_7

void USART_Init(void);
void USART_Test(void);
void USART_Send_Byte(uint32_t USART_ID,uint8_t data);
void USART_Send_Buff(uint32_t USART_ID,uint8_t *data,uint32_t len);

void Send_TO_Screen(uint8_t *data);
void Usart_Send_and_Receive_Data(void);

extern uint8_t Serial_RxFlag;
extern char Serial_RxPacket[];

#endif 

