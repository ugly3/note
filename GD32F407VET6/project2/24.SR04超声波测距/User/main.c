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
#include "oled.h"
uint32_t HalTime1,HalTime2;
//等待时间，us级别
void Delay_us(unsigned long i)
{
	unsigned long j;
	for(;i>0;i--)
	{
			for(j=32;j>0;j--);
	}
}
uint32_t Distance_Calculate(uint32_t count)//传入时间单位100us
{
    uint32_t Distance = 0;
    Distance = (uint32_t)(((float)count *17)/10);//距离单位cm,声速340M/S，时间*速度/2=距离
    return Distance;
}

uint32_t Distance;
uint32_t TimeCounter;
//定时器配置
void timer_config(void)
{
    /* -----------------------------------------------------------------------
	  系统主频168MHZ,timer_initpara.prescaler为167，timer_initpara.period为99，频率就为100HZ
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER1);
	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//AP1总线最高42MHZ,所以TIME1到168M需要4倍频
    timer_deinit(TIMER1);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 167;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 99;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
		nvic_irq_enable(TIMER1_IRQn, 0, 1);
		timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}
int main(void)
{
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOA时钟	
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);//PA0配置成输出
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);//配置成推挽输出，50M速度
		gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_1);//PA1配置成输入
		timer_config();  
		OLED_Init();//OLED初始化  
		OLED_Clear();//OLED清屏 

	while(1) 
	{		
		gpio_bit_write(GPIOA, GPIO_PIN_0,RESET);//预先拉低Trig引脚
		delay_1ms(5);
		gpio_bit_write(GPIOA, GPIO_PIN_0,SET);//拉高Trig引脚
		Delay_us(20);
		gpio_bit_write(GPIOA, GPIO_PIN_0,RESET);//拉低Trig引脚
		Delay_us(20);
		while(gpio_input_bit_get(GPIOA, GPIO_PIN_1) == 0);//如果是低电平，一直等
		HalTime1= TimeCounter;
		while(gpio_input_bit_get(GPIOA, GPIO_PIN_1) == 1);//如果是高电平一直等
		if(TimeCounter>HalTime1)
		{
		HalTime2 = TimeCounter-HalTime1;
		if(HalTime2<0x300)
		{
		Distance = Distance_Calculate(HalTime2);//计算距离值
		}
		}
	  OLED_Clear();
    OLED_ShowCHinese(18,0,0);//光
		OLED_ShowCHinese(36,0,1);//子
		OLED_ShowCHinese(54,0,2);//物
		OLED_ShowCHinese(72,0,3);//联
		OLED_ShowCHinese(90,0,4);//网
		OLED_ShowString(24,3,"SR04 TEST");
		OLED_ShowString(0,6,"Dis: ");  
		OLED_ShowNum(30,6,Distance,3,16);//显示距离，单位cm  
		OLED_ShowString(60,6,"cm"); 
    delay_1ms(200);		
	

	}  
 
}
