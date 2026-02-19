#include "fire.h"


/************************************************
函数名称 ： Fire_config
功    能 ： 
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void Fire_config(void)
{
	rcu_periph_clock_enable(Fire_RTC);
	gpio_mode_set(Fire_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Fire_Pin);
}

uint8_t get_fire_state(void){
	
	return (uint8_t)gpio_input_bit_get(Fire_GPIO_Port,Fire_Pin); 
}



