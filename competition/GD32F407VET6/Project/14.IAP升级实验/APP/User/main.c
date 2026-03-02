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
#define CLI() __set_PRIMASK(1)//关闭总中断  
#define SEI() __set_PRIMASK(0)//打开总中断
int main(void)
{

		SEI();
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		 nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);//中断向量地址偏移0x4000
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4配置成输出
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4配置成推挽输出，50M速度
    while(1)
		{
        gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4输出高电平
        delay_1ms(100);//等待100ms
        gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4输出低电平
        delay_1ms(100);
    }
}
