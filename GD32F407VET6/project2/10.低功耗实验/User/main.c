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

int main(void)
{
		int i;
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4配置成输出
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4配置成推挽输出，50M速度
		pmu_wakeup_pin_enable();//使能唤醒引脚
		for( i=0;i<50;i++)
		{
				delay_1ms(100);//等待100ms
				gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4输出高
				delay_1ms(100);
				gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4输出低
		}
		 
		pmu_to_standbymode(WFI_CMD);//进入待机模式
}
