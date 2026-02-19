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
#include "esp8266.h"
#include "tcp.h"
volatile uint8_t UartRxData;
uint8_t UartTxbuf[512]={1,2,3,4,5,6,7,8,9,10};
uint8_t UartRxbuf[512],UartIntRxbuf[512];
uint16_t UartRxIndex=0,UartRxFlag,UartRxLen=0,UartRxTimer,UartRxOKFlag,UartIntRxLen;

//串口清除
uint8_t UartRecv_Clear(void)
{
	UartRxOKFlag=0;
	UartRxLen=0;
	UartIntRxLen=0;
	UartRxIndex=0;
	return 1;
}

//接收标志函数，返回0说明没收据接收，返回1说明有数据收到
uint8_t Uart_RecvFlag(void)
{
		if(UartRxOKFlag==0x55)
		{
			UartRxOKFlag=0;
			UartRxLen=UartIntRxLen;
			memcpy(UartRxbuf,UartIntRxbuf,UartIntRxLen);//把缓冲区的数据，放入需要解析的数组
			UartIntRxLen=0;
			TcpClosedFlag = strstr (UartRxbuf, "CLOSED\r\n" ) ? 1 : 0;
			return 1;
		}
		return 0;
}
//串口2在1字节接收完成回调函数
void HAL_UART_RxCpltCallback(void)
{
	

		UartRxFlag=0x55;//接收标志置位
		UartIntRxbuf[UartRxIndex]=usart_data_receive(USART5);//数据写入缓冲区
		UartRxIndex++;//记载数目加1
		if(UartRxIndex>=512)//缓冲区是512字节，如果存满，归零
		{
			UartRxIndex=0;
		}
		usart_interrupt_enable(USART5, USART_INT_RBNE);//接收中断打开


}

//1ms调用一次，用来判断是否收完一帧
void UART_RecvDealwith(void)
{
	if(UartRxFlag==0x55)
	{
		if(UartIntRxLen<UartRxIndex)//UartIntRxLen小于UartRxIndex，说明有收到新的数据，把接收长度增加
		{
		UartIntRxLen=UartRxIndex;
		}else
		{
			UartRxTimer++;
			if(UartRxTimer>=50)//50ms,等待，没收到新数据，说明已经收完一帧
			{
				UartRxTimer=0;
				UartRxFlag=0;
				UartRxOKFlag=0x55;
				UartRxIndex=0;
			}
		}
	}
}

//USART0初始化，使用PC6(TX),PC7(RX)脚，115200波特率，无校验，8位数据，1位停止
void gd_eval_com_init(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOC);//使能GPIOC时钟

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART5);//使能USART5时钟
	//gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);//PB6,PB7需要重映射

	gpio_af_set(GPIOC,GPIO_AF_8,GPIO_PIN_6);//pc6作为USRAT5需要根据芯片手册，配置AF_8
	gpio_af_set(GPIOC,GPIO_AF_8,GPIO_PIN_7);//pc7作为USRAT5需要根据芯片手册，配置AF_8
	 gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);//PC6配置成输出
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);//PC6配置成推挽输出，50M速度
	
	/* configure USART Rx as alternate function push-pull */

	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);//PC7配置成输入
	  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);//PC7


	/* USART configure */
	usart_deinit(USART5);
	usart_baudrate_set(USART5,115200);
	usart_word_length_set(USART5, USART_WL_8BIT);
	usart_stop_bit_set(USART5, USART_STB_1BIT);
	usart_parity_config(USART5, USART_PM_NONE);

	usart_transmit_config(USART5, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART5, USART_RECEIVE_ENABLE);
	usart_enable(USART5);//使能USART5
}
int main(void)
{


	systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
	rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
	rcu_periph_clock_enable(RCU_GPIOC); //使能GPIOC时钟
	gd_eval_com_init();
	nvic_irq_enable(USART5_IRQn, 0, 0);//使能USART5中断
	usart_interrupt_enable(USART5, USART_INT_RBNE);//接收中断打开
	ESP8266_Init();
	while(1)
	{
	ESP8266_STA_TCPClient_Test();//测试TCP通讯
	}
}
