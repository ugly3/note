#include "button.h"

/************************************************
函数名称 ： Btn_config
功    能 ： led灯gpio引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

void Btn_config(void)
{
	rcu_periph_clock_enable(Btn1_RTC);
	rcu_periph_clock_enable(Btn2_RTC);
	rcu_periph_clock_enable(Btn3_RTC);
	rcu_periph_clock_enable(Btn4_RTC);
	
	gpio_mode_set(Btn1_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Btn1_Pin);
	gpio_mode_set(Btn2_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Btn2_Pin);
	gpio_mode_set(Btn3_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Btn3_Pin);
	gpio_mode_set(Btn4_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Btn4_Pin);
}

uint8_t Btn_Scan(uint8_t mode)  			
{
	static uint8_t Btn_up = 1;
	
	if(mode){
		Btn_up = 1; 	
	}
	
	if(Btn_up&&(Btn1 == 1 || Btn2 == 0 || Btn3 == 0 || Btn4 == 0)){
		delay_1ms(10);
		
		Btn_up=0;
		if(Btn1==1)return 1;
		else if(Btn2==0)return 2;
		else if(Btn3==0)return 3;
		else if(Btn4==0)return 4;
	}
	else if(Btn1 == 0 && Btn2 == 1 && Btn3 == 1 && Btn4 == 1){
		Btn_up = 1; 
	}
 	return 0;
}



