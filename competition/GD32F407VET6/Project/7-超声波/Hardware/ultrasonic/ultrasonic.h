#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "gd32f4xx.h"
#include "systick.h"

#define ECHO_RTC 		   RCU_GPIOE
#define ECHO_GPIO_Port GPIOE
#define ECHO_Pin 		   GPIO_PIN_6
				
#define Trig_RTC 		    RCU_GPIOE
#define Trig_GPIO_Port	GPIOE
#define Trig_Pin 		    GPIO_PIN_5
				
void Ultrasonic_init(void);
uint16_t Ultrasonic_read(void);



 
#endif  /* _ULTRASONIC_H */

