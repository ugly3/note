#include "fan.h"


/************************************************
函数名称 ： FAN_config
功    能 ： 初始蜂鸣器引脚
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void FAN_config(void)
{
	rcu_periph_clock_enable(FAN_RTC);

	gpio_mode_set(FAN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, FAN_Pin);
	gpio_output_options_set(FAN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FAN_Pin);	
	gpio_bit_write(FAN_GPIO_Port, FAN_Pin, RESET);
}


void FAN_on(void){
	gpio_bit_write(FAN_GPIO_Port, FAN_Pin, SET);
}


void FAN_off(void){
	gpio_bit_write(FAN_GPIO_Port, FAN_Pin, RESET);
}


