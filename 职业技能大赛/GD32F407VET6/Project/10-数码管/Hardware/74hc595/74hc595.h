#ifndef _HC595_H
#define _HC595_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>




#define	SI_RTC RCU_GPIOD
#define	 SI_Port GPIOD
#define	 SI_PIN GPIO_PIN_13

#define	RCK_RTC RCU_GPIOD
#define	RCK_Port GPIOD
#define RCK_PIN  GPIO_PIN_12
	
#define	SCK_RTC RCU_GPIOD
#define	 SCK_Port GPIOD
#define	SCK_PIN GPIO_PIN_11







void hc595_init(void);
void hc595_WriteByte(uint8_t data);
void hc595_WriteByte16(uint16_t data);

void HC595_Send_16Bit(uint16_t data) ;
 
#endif  /* _SERVO_H */

