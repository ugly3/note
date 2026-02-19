#ifndef __BH1750_H
#define __BH1750_H

#include "gd32f4xx.h"
#include "systick.h"
#include "iic.h"

void BH1750_init(void);


#define BH1750_Addr 0x46				//0100 0110   即地址位 = 0；
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07


void BH1750_Hardware_Init(void);
void BH1750_Software_Init(void);

void Start_BH1750(void);				//上电，设置清除数据寄存器
void Read_BH1750(void);					//连续的读取内部寄存器数据
void Convert_BH1750(void);
uint16_t BH1750_Read(void); 

#endif 

