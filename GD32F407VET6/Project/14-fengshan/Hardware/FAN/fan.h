#ifndef _FAN_H
#define _FAN_H

#include "gd32f4xx.h"
#include "systick.h"

#define FAN_RTC 		RCU_GPIOD
#define FAN_GPIO_Port	GPIOD
#define FAN_Pin 		GPIO_PIN_5



void FAN_config(void);
void FAN_on(void);
void FAN_off(void);

 
#endif  /* _BEEP_H */

