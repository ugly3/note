#ifndef __I2C_KEY_H
#define __I2C_KEY_H



#include "gd32f4xx.h"                   // Device header


#define u8 uint8_t


void I2C_Key_Init(void);
void I2c_key_scan(void);  
void process_password_input(void);// ¥¶¿Ì√‹¬Î ‰»Î








#endif



