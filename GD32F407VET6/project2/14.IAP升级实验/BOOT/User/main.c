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
//USART0初始化，使用PA9(TX),PA10(RX)脚，115200波特率，无校验，8位数据，1位停止
void gd_eval_com_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOA时钟

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);//使能USART0时钟
		gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);//复用功能7
		gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);//PA9配置成串口输出
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
		gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);//复用功能7
		gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);//PA10配置成串口输入
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	
    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0,115200);
		usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
	  usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);//使能USART0
		
}



int main(void)
{


  systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
	gd_eval_com_init();//串口初始化
	while(1)
	{
	   Main_Menu();//实现程序升级并跳转，Ymodem协议
	}
}
