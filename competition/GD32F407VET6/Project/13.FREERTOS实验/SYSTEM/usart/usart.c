
#include "sys.h"
#include "usart.h"	 
#include "myqueue.h"
#include "command_line.h"


void Debug_USART_Init(uint32_t baud){
	/* USART interrupt configuration */
	nvic_irq_enable(DEBUG_USART_IRQ, 0, 0);
	
	/* enable GPIO clock */
	rcu_periph_clock_enable( DEBUG_USART_GPIO_CLK );

	/* enable USART clock */
	rcu_periph_clock_enable( DEBUG_USART_CLK );

	/* connect port to USARTx_Tx */
	gpio_af_set(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_AF, DEBUG_USART_TX_GPIO_PIN);

	/* connect port to USARTx_Rx */
	gpio_af_set(DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_AF, DEBUG_USART_RX_GPIO_PIN);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(DEBUG_USART_TX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,DEBUG_USART_TX_GPIO_PIN);
	gpio_output_options_set(DEBUG_USART_TX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,DEBUG_USART_TX_GPIO_PIN);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(DEBUG_USART_RX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,DEBUG_USART_RX_GPIO_PIN);
	gpio_output_options_set(DEBUG_USART_RX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,DEBUG_USART_RX_GPIO_PIN);


	/* USART configure */
	usart_deinit(DEBUG_USARTx);
	usart_baudrate_set(DEBUG_USARTx,baud);
	usart_receive_config(DEBUG_USARTx, USART_RECEIVE_ENABLE);
	usart_transmit_config(DEBUG_USARTx, USART_TRANSMIT_ENABLE);
	usart_enable(DEBUG_USARTx);
	
	/* enable USART1 receive interrupt */
	usart_interrupt_enable(DEBUG_USARTx, USART_INTEN_RBNEIE);
	
//	/* enable USART1 transmit interrupt */
//	usart_interrupt_enable(DEBUG_USARTx, USART_INTEN_TBEIE);
}

/**
 * @brief  UART interrupt func.
 * @param  null
 * @retval null
 */
void DEBUG_USART_TX_IRQHandler(void)
{
    /* not used for now */
}

/**
 * @brief  UART RX interrupt func.
 * @param  null
 * @retval null
 */
void DEBUG_USART_RX_IRQHandler(void)
{
	uint8_t ucData;
	
	if((RESET != usart_interrupt_flag_get(DEBUG_USARTx, USART_INT_RBNEIE)) && 
		 (RESET != usart_flag_get(DEBUG_USARTx, USART_FLAG_RBNE))){
			/* Read one byte from the receive data register */
			ucData = (uint8_t)usart_data_receive(DEBUG_USARTx);

			/* save char */
			QueueIn(CLI_RX_Buff, ucData);		

			/* display char in terminal */
			usart_data_transmit(DEBUG_USARTx, ucData);		
	}
		 
//	if((RESET != usart_flag_get(DEBUG_USARTx, USART_FLAG_TBE)) && 
//		 (RESET != usart_interrupt_flag_get(DEBUG_USARTx, USART_INT_TBEIE))){
//			/* Write one byte to the transmit data register */
//			usart_data_transmit(DEBUG_USARTx, tx_buffer[tx_counter++]);

//			if(tx_counter >= nbr_data_to_send)
//			{
//					/* disable the USART0 transmit interrupt */
//					usart_interrupt_disable(DEBUG_USARTx, USART_INTEN_TBEIE);
//			}
//	}

} 



//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

void _ttywrch(int ch)
{
	ch = ch;
} 

//重定义fputc函数 
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(DEBUG_USARTx, (uint8_t)ch);
    while(RESET == usart_flag_get(DEBUG_USARTx, USART_FLAG_TBE));
    return ch;
}

int fgetc(FILE *f)
{
    /* get a char */
		while(usart_flag_get(DEBUG_USARTx, USART_FLAG_RBNE) == RESET);
		/* Read one byte from the receive data register */
		return (int)usart_data_receive(DEBUG_USARTx);	
}

#endif 

