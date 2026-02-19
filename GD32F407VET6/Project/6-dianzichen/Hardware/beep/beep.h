#ifndef _BEEP_H
#define _BEEP_H

#include "gd32f4xx.h"
#include "systick.h"

#define Beep_RTC 		RCU_GPIOE
#define Beep_GPIO_Port	GPIOE
#define Beep_Pin 		GPIO_PIN_11



void Beep_config(void);
void Beep_on(void);
void Beep_off(void);
void Beep_control(uint16_t time_us ,uint16_t count);
 
#endif  /* _BEEP_H */

