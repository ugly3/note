#ifndef _FIRE_H
#define _FIRE_H

#include "gd32f4xx.h"
#include "systick.h"

#define Fire_RTC 		RCU_GPIOD
#define Fire_GPIO_Port	GPIOD
#define Fire_Pin 		GPIO_PIN_5



void Fire_config(void);
uint8_t get_fire_state(void);
 
#endif  /* _BEEP_H */

