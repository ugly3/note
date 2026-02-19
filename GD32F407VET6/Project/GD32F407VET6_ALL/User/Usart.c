#include "Usart.h"
#include "Task_Cycle.h"
#include "Fan.h"
#include "Key.h"   


//USART1初始化，使用PD5(TX),PD6(RX)脚，9600波特率，无校验，8位数据，1位停止
void USART_Init(void)
{
	/* enable GPIO clock */
    rcu_periph_clock_enable(USART_TX_RTC);
	rcu_periph_clock_enable(USART_RX_RTC);
	
	/* enable USART clock */
    rcu_periph_clock_enable(RTC_USARTx);
	
	/* enable USART clock */
    rcu_periph_clock_enable(RTC_USARTx);

    /* configure the USART1 TX pin and USART1 RX pin */
    gpio_af_set(USART_TX_GPIO_Port, GPIO_AF_x, USART_TX_Pin);
    gpio_af_set(USART_RX_GPIO_Port, GPIO_AF_x, USART_RX_Pin);

    /* configure USART1 TX as alternate function push-pull */
    gpio_mode_set(USART_TX_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_Pin);
    gpio_output_options_set(USART_TX_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_TX_Pin);

    /* configure USART1 RX as alternate function push-pull */
    gpio_mode_set(USART_RX_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_Pin);
    gpio_output_options_set(USART_RX_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_RX_Pin);
	
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1,baudrate);
	usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
	usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1);//使能USART1
	
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

void Send_TO_Screen(uint8_t *data)
{
	static const uint32_t USART_ID = USARTx;
    static const uint8_t test[3] = {0xFF, 0xFF, 0xFF};
    uint32_t len = strlen((const char *)data); // 动态计算data的长度
    uint32_t i = 0;

    // 发送数据
    while(len--)
    {
        USART_Send_Byte(USART_ID, data[i]);
        while(usart_flag_get(USART_ID, USART_FLAG_TBE) == RESET);
        i++;
    }

    // 发送包尾
    for (i = 0; i < sizeof(test); i++)
    {
        USART_Send_Byte(USART_ID, test[i]);
        while(usart_flag_get(USART_ID, USART_FLAG_TBE) == RESET);
    }
}


uint8_t Serial_RxFlag;

char Serial_RxPacket[100];	

volatile uint8_t pRxPacket = 0;

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0; // 接收状态机

	if((RESET != usart_interrupt_flag_get(USARTx, USART_INT_FLAG_RBNE)) && (RESET != usart_flag_get(USARTx, USART_FLAG_RBNE)))
	{
		uint8_t RxData = usart_data_receive(USARTx);
		
		switch (RxState)
		{
			case 0: // 等待包头
				if (RxData == 0x55)
				{
					pRxPacket = 0; // 重置包索引
					RxState = 1; // 进入下一状态
				}
				break;
			case 1: // 接收数据直到包尾
				if (RxData == 0xBB) // 检测到包尾
				{
					Serial_RxPacket[pRxPacket] = '\0'; // 添加字符串结束符
					Serial_RxFlag = 1; // 设置接收完成标志
					RxState = 0; // 重置状态机
				}
				else
				{
					if (pRxPacket < RX_PACKET_SIZE - 1) // 防止缓冲区溢出
					{
						Serial_RxPacket[pRxPacket++] = RxData; // 存储接收到的数据
					}
				}
				break;
			default:
				RxState = 0; // 出现未知状态，重置状态机
				break;
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
	while (usart_flag_get(USART0, USART_FLAG_RBNE) == RESET);
	return (int)usart_data_receive(USART0);
}


void Usart_Send_and_Receive_Data(void)
{
    if(Serial_RxFlag==1)
    {
        if(Serial_RxPacket[0]==0x01)//温度阈值
            temp_yuzhi = Serial_RxPacket[1];
        else if(Serial_RxPacket[0]==0x02)//湿度阈值
            humi_yuzhi = Serial_RxPacket[1];
        else if(Serial_RxPacket[0]==0x03)//烟雾阈值
            MQ2_yuzhi= Serial_RxPacket[1];
        else if(Serial_RxPacket[0]==0x04)//窗帘开关
        {
            if(Serial_RxPacket[1]==0x01)
            {
                Walkmotor_status=0;
                task3_is_start=0;
            }
            else if(Serial_RxPacket[1]==0x02)
            {
                Walkmotor_status=1;
                task3_is_start=1;
            }
        }
        else if(Serial_RxPacket[0]==0x05)
            LightData_yuzhi=Serial_RxPacket[1];
        else if(Serial_RxPacket[0]==0x06)
            Huoyan_yuzhi = Serial_RxPacket[1];
        else if(Serial_RxPacket[0]==0x07)
        {
            if(Serial_RxPacket[1]==0x01)
            {
                led_status=0;
                task1_is_start=0;
            }
            else if(Serial_RxPacket[1]==0x02)
            {
                led_status=1;
                task1_is_start=1;
            }
        }
            
        else if(Serial_RxPacket[0]==0x08)
        {
            if(Serial_RxPacket[1]==0x01)
            {
                Fan_status=0;
                task4_is_start=0;
            }
            else if(Serial_RxPacket[1]==0x02)
            {
                Fan_status=1;
                task4_is_start=1;
            }
            else if(Serial_RxPacket[1]==0x03)
            {
                Fan_status=2;
                task4_is_start=1;
            }
            
        }
    }
    
}



