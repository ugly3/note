#ifndef _SERVO_H
#define _SERVO_H

#include "gd32f4xx.h"
#include "systick.h"

#define SERVO_RTC 		   RCU_GPIOC
#define SERVO_GPIO_Port GPIOC
#define SERVO_Pin 		   GPIO_PIN_9
				

void servo_init(void);
void Set_Servo_Angle(uint16_t angle);

 
#endif  /* _SERVO_H */

