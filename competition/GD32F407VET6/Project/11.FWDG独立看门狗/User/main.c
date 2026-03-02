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


	systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
	rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4配置成输出
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4配置成推挽输出，50M速度
  /* enable IRC40K */
  rcu_osci_on(RCU_IRC32K);//IRC32K时钟打开
	/* confiure FWDGT counter clock: 32KHz(IRC32K) / 64 = 0.5KHz */
	fwdgt_config(2*500, FWDGT_PSC_DIV64);//看门狗时间设置,Clock为500hz,1000个计数就是2秒，看门狗复位时间就是2秒
	fwdgt_enable();//看门狗使能
	fwdgt_counter_reload();//喂狗
	while (1)
	{
		delay_1ms(500);//等待500ms
		gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4输出低
		delay_1ms(500);
		gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4输出高
		fwdgt_counter_reload();//喂狗
	}
}
