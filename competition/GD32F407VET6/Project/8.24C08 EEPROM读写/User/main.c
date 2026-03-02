/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "systick.h"
#include "24C08.h"
unsigned char EEDATA;
extern void EE_IIC_Init(void);
extern uint8_t EE_EE_IIC_SendByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);
extern uint8_t EE_IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
int main(void)
{


    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
		EE_IIC_Init();//EEPROM管脚初始化
		EE_EE_IIC_SendByteToSlave(0xA0,0x00,0x33);//存储数据
		delay_1ms(50);
		EE_IIC_ReadByteFromSlave(0xA0,0x00,&EEDATA);//读取数据
    while(1)
		{
					EE_EE_IIC_SendByteToSlave(0xA0,0x00,0x33);//存储数据
					delay_1ms(1000);
					EE_IIC_ReadByteFromSlave(0xA0,0x00,&EEDATA);//读取数据

    }
}
