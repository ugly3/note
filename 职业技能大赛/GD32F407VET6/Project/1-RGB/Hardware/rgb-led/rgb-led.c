#include "rgb-led.h"

/************************************************
函数名称 ： RGBled_config
功    能 ： led灯gpio引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 
*************************************************/

uint32_t RGB_colors[RGBnum];

void RGBled_config(void)
{
	rcu_periph_clock_enable(RGBled_RTC);

	gpio_mode_set(RGBled_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, RGBled_Pin);
	gpio_output_options_set(RGBled_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RGBled_Pin);	
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, RESET);
}




void RGB_set_color(uint8_t num,uint8_t r ,uint8_t g ,uint8_t b , uint8_t brightness){
	
	if(brightness==0)RGB_colors[num]=0;
	else{
		brightness+=1;
		if(brightness){
		 r = ((r * brightness) >> 8);
		 g = ((g * brightness) >> 8);
		 b = ((b * brightness) >> 8);
		}
		
		RGB_colors[num]=((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
}

void RGB_set_color_32(uint8_t num,uint32_t c,uint8_t brightness){
	uint8_t  r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
  if(brightness==0)RGB_colors[num]=0;
	else{
		brightness+=1;
		if(brightness){
		 r = ((r * brightness) >> 8);
		 g = ((g * brightness) >> 8);
		 b = ((b * brightness) >> 8);
		}
		
		RGB_colors[num]=((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
}

void RGBsendT1(void){
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, SET);
	delay_100ns(6);
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, RESET);
	delay_100ns(2);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void RGBsenT0(void){
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, SET);
	delay_100ns(2);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, RESET);
	delay_100ns(6);
}
void RGBsenRESET(){
	gpio_bit_write(RGBled_GPIO_Port, RGBled_Pin, RESET);
	delay_us(300);
}

void RGB_WriteByte(uint8_t dat){
	uint8_t i;
	for(i=0;i<8;i++){
		if(dat&0x80)RGBsendT1();
		else RGBsenT0();
		dat<<=1;
	}
}

void RGB_Show(void){
	int i=0;
	for(i=0;i<RGBnum;i++){
		RGB_WriteByte((uint8_t)(RGB_colors[i]>>8));
		RGB_WriteByte((uint8_t)(RGB_colors[i]>>16));
		RGB_WriteByte((uint8_t)(RGB_colors[i]));
	}
	RGBsenRESET();
}
