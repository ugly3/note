#ifndef _FIRE_H
#define _FIRE_H

#include "gd32f4xx.h"
#include "systick.h"

#define Smoke_RTC 		RCU_GPIOD
#define Smoke_GPIO_Port	GPIOD
#define Smoke_Pin 		GPIO_PIN_5



void smoke_config(void);
uint8_t get_smoke_state(void);
 
#endif  /* _BEEP_H */

