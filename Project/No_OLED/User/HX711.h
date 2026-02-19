#ifndef __HX711_H
#define __HX711_H



#include "gd32f4xx.h"                   // Device header

#define HX711_RCU_GPIOX       RCU_GPIOB
#define HX711_PORT            GPIOB
#define HX711_PIN_SCK         GPIO_PIN_12
#define HX711_PIN_DOUT        GPIO_PIN_13

#define HX711_SCK PBout(12)
#define HX711_DOUT PBin(13)

void HX711_Init(void);
void Get_Maopi(void);
void Get_Weight(uint8_t status);

extern float Weight_Shiwu;



#endif


