#ifndef __BH1750_H
#define __BH1750_H


#include "gd32f4xx.h"                   // Device header


#define BH1750_RCU_GPIOX     RCU_GPIOB
#define BH1750_PORT          GPIOB                  
#define BH1750_PIN_SCL       GPIO_PIN_6     
#define BH1750_PIN_SDA       GPIO_PIN_7 

#define BH1750_RCU_I2CX      RCU_I2C0
#define I2CX                 I2C0

void BH1750_Init(void);
void BH1750_Get_Data(void);





extern uint32_t LightData_Hex;//光照强度（整数，单位lx）














#endif

