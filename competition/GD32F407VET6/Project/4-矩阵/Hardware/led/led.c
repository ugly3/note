#include "led.h"

/************************************************
函数名称 ： LED_config
功    能 ： led灯gpio引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void LED_config(void)
{
	rcu_periph_clock_enable(LED1_RTC);
	rcu_periph_clock_enable(LED2_RTC);
	rcu_periph_clock_enable(LED3_RTC);
	rcu_periph_clock_enable(LED4_RTC);
	
	
	gpio_mode_set(LED1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED1_Pin);
	gpio_mode_set(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED2_Pin);
	gpio_mode_set(LED3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED3_Pin);
	gpio_mode_set(LED4_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED4_Pin);
	
	
	gpio_output_options_set(LED1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_Pin);	
	gpio_output_options_set(LED2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_Pin);
	gpio_output_options_set(LED3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_Pin);
	gpio_output_options_set(LED4_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED4_Pin);
	
	gpio_bit_write(LED1_GPIO_Port, LED1_Pin, RESET);
	gpio_bit_write(LED2_GPIO_Port, LED2_Pin, RESET);
	gpio_bit_write(LED3_GPIO_Port, LED3_Pin, RESET);
	gpio_bit_write(LED4_GPIO_Port, LED4_Pin, RESET);
}

