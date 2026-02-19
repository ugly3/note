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
#include "oled.h"

int main(void)
{


    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
	OLED_Init();//OLED初始化  
	OLED_Clear();//OLED清屏 
	while(1)
	{
		OLED_Clear();//OLED清屏 
		OLED_ShowCHinese(18,0,0);//显示中文(光)
		OLED_ShowCHinese(36,0,1);//显示中文(子)
		OLED_ShowCHinese(54,0,2);//显示中文(物)
		OLED_ShowCHinese(72,0,3);//显示中文(联)
		OLED_ShowCHinese(90,0,4);//显示中文(网)
		OLED_ShowString(6,3,(u8 *)"GD32F407 TEST");//显示字符
		OLED_ShowString(0,6,(u8 *)"ASCII:");//显示字符  
		OLED_ShowString(63,6,(u8 *)"CODE:");//显示字符  
		delay_1ms(5000);//等待5秒
	}
 
}
