#ifndef _HX711_H
#define _HX711_H

#include "gd32f4xx.h"
#include "systick.h"

#define HX711_SCL_RTC 		  RCU_GPIOE
#define HX711_SCL_GPIO_Port	GPIOE
#define HX711_SCL_Pin 		  GPIO_PIN_0
				
#define HX711_SDA_RTC 		  RCU_GPIOE
#define HX711_SDA_GPIO_Port	GPIOE
#define HX711_SDA_Pin 		  GPIO_PIN_1
				
#define	HX711_SCK_LOW		gpio_bit_write(HX711_SCL_GPIO_Port, HX711_SCL_Pin,RESET)
#define	HX711_SCK_HIGH	gpio_bit_write(HX711_SCL_GPIO_Port, HX711_SCL_Pin,SET)

#define	HX711_DOUT_IN 	gpio_input_bit_get(HX711_SDA_GPIO_Port,HX711_SDA_Pin)

void HX711_init(void);
unsigned long HX711_Read(void);
void Get_Maopi(void);
long Get_Weight(void);



 
#endif  /* _BUTTON_H */

