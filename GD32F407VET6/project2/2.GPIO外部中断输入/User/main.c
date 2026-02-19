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
	//配置PA0外部中断
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);//PA0配置成下拉输入
	/* enable and set key EXTI interrupt to the lowest priority */
	nvic_irq_enable(EXTI0_IRQn, 2U, 0U);//中断使能
	/* configure key EXTI line */
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);//上升沿触发
	exti_interrupt_flag_clear(EXTI_0);
	while(1)
	{

	}
}
