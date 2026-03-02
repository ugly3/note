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
uint8_t transmitter_buffer[] = "HELLOWORLD";//定义发送数组
uint8_t receiver_buffer[10];//定义接收数组
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE   (ARRAYNUM(transmitter_buffer) - 1)//计算大小
uint8_t transfersize = TRANSMIT_SIZE;
uint8_t receivesize = 10;
__IO uint8_t txcount = 0; 
__IO uint16_t rxcount = 0; 

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
    usart_baudrate_set(USART0,9600);
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
		//USART相关配置
		gd_eval_com_init();
		nvic_irq_enable(USART0_IRQn, 0, 0);//使能USART1中断
		usart_interrupt_enable(USART0, USART_INT_RBNE);//接收中断打开
    while(1)
    {
			if(rxcount >= receivesize)//接收满10个字节，在USART1_IRQHandler函数中计数
			{
				rxcount=0;
				txcount=0;
				usart_interrupt_enable(USART0, USART_INT_TBE);//发送中断打开
				while(txcount < transfersize);//等待发送完成，在USART1_IRQHandler函数中计数
				while (RESET == usart_flag_get(USART0, USART_FLAG_TC));//发送完成判断
				usart_interrupt_enable(USART0, USART_INT_RBNE);//接收中断打开
				rxcount=0;
			}
			
    }
}
