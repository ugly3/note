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
#define uchar unsigned char
#define uint  unsigned int
uchar phasecw[4] ={GPIO_PIN_5,GPIO_PIN_4,GPIO_PIN_3,GPIO_PIN_2};//正转电机导通相序D-C-B-A
uchar phaseccw[4]={GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5};//反转电机导通相序A-B-C-D

//正转
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {

		gpio_bit_write(GPIOE,phasecw[i],SET);//输出高
    delay_1ms(10);//等待5ms
		gpio_bit_write(GPIOE,phasecw[i],RESET);//输出低
  }
}
//反转
void MotorCCW(void)
{
 uchar i;
	
 for(i=0;i<4;i++)
  {

		gpio_bit_write(GPIOE,phaseccw[i],SET);//输出高
    delay_1ms(10);//等待5ms
		gpio_bit_write(GPIOE,phaseccw[i],RESET);//输出低
  }
}
int main(void)
{
		int i;
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOE);//使能GPIOE时钟
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);//PE2,PE3,PE4,PE5配置成输出
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);//配置成推挽输出，50M速度
		while(1)
    {
			
				delay_1ms(50);//等待50ms			
				for(i=0;i<500;i++)
				{
				MotorCW();   //正转
				} 
				delay_1ms(500);
				for(i=0;i<500;i++)
				{
				MotorCCW();  //反转
				} 
			delay_1ms(500); //等待500ms
    }
}
