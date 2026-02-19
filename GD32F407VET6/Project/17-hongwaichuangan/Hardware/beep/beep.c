#include "beep.h"


/************************************************
函数名称 ： Beep_config
功    能 ： 初始蜂鸣器引脚
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void Beep_config(void)
{
	rcu_periph_clock_enable(Beep_RTC);

	gpio_mode_set(Beep_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, Beep_Pin);
	gpio_output_options_set(Beep_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Beep_Pin);	
	gpio_bit_write(Beep_GPIO_Port, Beep_Pin, RESET);
}

uint8_t Beep_sw=0;
//当前函数Beep_pulse放在systick.c文件中的delay_decrement函数运行
void Beep_pulse(void){
	if(Beep_sw)gpio_bit_toggle(Beep_GPIO_Port,Beep_Pin);
}
void Beep_on(void){
	Beep_sw=1;
}
void Beep_off(void){
	Beep_sw=0;
	gpio_bit_write(Beep_GPIO_Port, Beep_Pin, RESET);
}

/************************************************
函数名称 ： Beep_control
功    能 ： 控制蜂鸣器发出声音
参    数 ： time_us 脉冲时间
						count 脉冲数量
返 回 值 ： 无
作    者 ： 
*************************************************/
void Beep_control(uint16_t time_us ,uint16_t count){
	for(int i=0 ;i<count;i++){
		gpio_bit_write(Beep_GPIO_Port, Beep_Pin, SET);
		delay_us(time_us);
		gpio_bit_write(Beep_GPIO_Port, Beep_Pin, RESET);
		delay_us(time_us);
	}
	gpio_bit_write(Beep_GPIO_Port, Beep_Pin, RESET);
}


