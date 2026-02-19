#ifndef _BUTTON_H
#define _BUTTON_H

#include "gd32f4xx.h"
#include "systick.h"

#define Btn1_RTC 		RCU_GPIOA
#define Btn1_GPIO_Port	GPIOA
#define Btn1_Pin 		GPIO_PIN_0
				
#define Btn2_RTC 		RCU_GPIOA
#define Btn2_GPIO_Port	GPIOA
#define Btn2_Pin 		GPIO_PIN_1
				
#define Btn3_RTC 		RCU_GPIOA
#define Btn3_GPIO_Port	GPIOA
#define Btn3_Pin 		GPIO_PIN_4
				
#define Btn4_RTC 		RCU_GPIOA
#define Btn4_GPIO_Port	GPIOA
#define Btn4_Pin 		GPIO_PIN_5

#define Btn1_PRES 1
#define Btn2_PRES	2
#define Btn3_PRES	3
#define Btn4_PRES 4

#define	Btn1	gpio_input_bit_get(Btn1_GPIO_Port,Btn1_Pin) 
#define	Btn2	gpio_input_bit_get(Btn2_GPIO_Port,Btn2_Pin)	
#define	Btn3	gpio_input_bit_get(Btn3_GPIO_Port,Btn3_Pin)
#define	Btn4	gpio_input_bit_get(Btn4_GPIO_Port,Btn4_Pin)	


void Btn_config(void);
uint8_t Btn_Scan(uint8_t mode);  
 
#endif  /* _BUTTON_H */

