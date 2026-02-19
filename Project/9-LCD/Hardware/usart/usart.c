#include "usart.h"

uint8_t rxbuffer[3],txbuffer[3];

uint8_t rxbuffer1[26],txbuffer1[30];
uint8_t rx_date;
uint16_t rx_size = 26;
uint16_t rxcount = 0;
uint16_t USART_FLAG = 0x00;

//串口屏
void USART_Init(void)
{
	/* enable GPIO clock */
    rcu_periph_clock_enable(USART_TX_RTC);
	rcu_periph_clock_enable(USART_RX_RTC);
	
	/* enable USART clock */
    rcu_periph_clock_enable(RTC_USARTx);

    /* configure the USART5 TX pin and USART5 RX pin */
    gpio_af_set(USART_TX_GPIO_Port, GPIO_AF_x, USART_TX_Pin);
    gpio_af_set(USART_RX_GPIO_Port, GPIO_AF_x, USART_RX_Pin);

    /* configure USART1 TX as alternate function push-pull */
    gpio_mode_set(USART_TX_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_Pin);
    gpio_output_options_set(USART_TX_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_TX_Pin);

    /* configure USART1 RX as alternate function push-pull */
    gpio_mode_set(USART_RX_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_Pin);
    gpio_output_options_set(USART_RX_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_RX_Pin);
		
    /* USART configure */
	usart_deinit(USARTx);
    usart_baudrate_set(USARTx, 		baudrate);  					// 波特率115200
    usart_parity_config(USARTx, 	USART_PM_NONE);  				// 无校检
    usart_word_length_set(USARTx,	USART_WL_8BIT);  				// 8位数据位
    usart_stop_bit_set(USARTx, 		USART_STB_1BIT);  				// 1位停止位
    usart_transmit_config(USARTx, 	USART_TRANSMIT_ENABLE);			// 使能串口发送
    usart_receive_config(USARTx, 	USART_RECEIVE_ENABLE);			// 使能串口接收
	usart_hardware_flow_coherence_config(USARTx,USART_HCM_NONE);	//无硬件流控
	
	/* USART IRQn configure */
	nvic_irq_enable(USART1_IRQn,5,0);								//配置中断
	usart_interrupt_enable(USARTx,USART_INT_RBNE);					//使能中断	
	usart_data_first_config(USARTx,USART_MSBF_LSB);					//大端小端与MSB和LSB
	usart_enable(USARTx);  											// 使能串口
}


void USART_Send_Byte(uint32_t USART_ID,uint8_t data) 
{
    usart_data_transmit(USART_ID,data);
}


void USART_Send_Buff(uint32_t USART_ID,uint8_t *data,uint32_t len) 
{
    uint32_t i = 0;
	
	while(len--)
	{
		USART_Send_Byte(USART_ID,data[i]);
		
		while(usart_flag_get(USART_ID,USART_FLAG_TBE) == RESET);
		i++;		
    }
}

void USART_Test(void)
{
	if(USART_FLAG == 1)
	{
		USART_FLAG = 0;
		rxcount = 0;
		if(rxbuffer[1]==0x01)
		{
			gpio_bit_write(LED1_GPIO_Port, LED1_Pin, SET);
			gpio_bit_write(LED2_GPIO_Port, LED2_Pin, SET);
			gpio_bit_write(LED3_GPIO_Port, LED3_Pin, SET);
			gpio_bit_write(LED4_GPIO_Port, LED4_Pin, SET);
			Beep_control(500,200);
		}else if(rxbuffer[1]==0x02){
			gpio_bit_write(LED1_GPIO_Port, LED1_Pin, RESET);
			gpio_bit_write(LED2_GPIO_Port, LED2_Pin, RESET);
			gpio_bit_write(LED3_GPIO_Port, LED3_Pin, RESET);
			gpio_bit_write(LED4_GPIO_Port, LED4_Pin, RESET);
			Beep_control(500,200);
		}
	}
}

void USART1_IRQHandler(void)
{
	
    if((RESET != usart_interrupt_flag_get(USARTx, USART_INT_FLAG_RBNE)) && (RESET != usart_flag_get(USARTx, USART_FLAG_RBNE))){
        /* receive data */
        rxbuffer[rxcount++] = usart_data_receive(USARTx);
		
		if(rxbuffer[rxcount-2] == 0x0D && rxbuffer[rxcount-1] == 0x0A){	//检测到完整的数据
			USART_FLAG = 0x01;	
		}
		
		if(rxcount == rx_size){
				rxcount = 0;
			}
    }		
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USARTx, (uint8_t)ch);
    while(RESET == usart_flag_get(USARTx, USART_FLAG_TBE));
    return ch;
}

/* retarget the C library scanf/getchar function to the USART */
int fgetc(FILE *f)
{
	while (usart_flag_get(USART1, USART_FLAG_RBNE) == RESET);
	return (int)usart_data_receive(USART1);
}
