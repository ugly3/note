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
		gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);//PA0配置成输输入
		OLED_Init();//OLED初始化  
		OLED_Clear();//OLED清屏 

	while(1) 
	{		
		OLED_Clear();
    OLED_ShowCHinese(18,0,0);//光
		OLED_ShowCHinese(36,0,1);//子
		OLED_ShowCHinese(54,0,2);//物
		OLED_ShowCHinese(72,0,3);//联
		OLED_ShowCHinese(90,0,4);//网
		OLED_ShowCHinese(10,3,5);//障
		OLED_ShowCHinese(28,3,6);//碍
		OLED_ShowCHinese(46,3,7);//物
	  OLED_ShowString(80,3,"TEST");

		if(gpio_input_bit_get(GPIOA,GPIO_PIN_0)==0)//读取PA0引脚电平，如果是低电平，说明有障碍物
		{
				OLED_ShowCHinese(46,6,8);//有
		}else
		{	
				OLED_ShowCHinese(46,6,9);//无
		}
		delay_1ms(1000);//等待1秒
	}	  
 
}
