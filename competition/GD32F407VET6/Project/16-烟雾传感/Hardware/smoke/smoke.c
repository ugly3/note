#include "smoke.h"


/************************************************
函数名称 ： Smoke_config
功    能 ： 
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void smoke_config(void)
{
	rcu_periph_clock_enable(Smoke_RTC);
	gpio_mode_set(Smoke_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Smoke_Pin);
}



uint8_t get_smoke_state(void){
	
	return (uint8_t)gpio_input_bit_get(Smoke_GPIO_Port,Smoke_Pin); 
}



