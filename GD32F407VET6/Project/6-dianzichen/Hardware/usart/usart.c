#include "usart.h"



// 初始化 USART0
void USART0_init(uint32_t Baud) {
    // 使能 GPIOA 和 USART0 时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    // 配置 PA9 为复用推挽输出（USART0 TX）
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    // 配置 PA10 为浮空输入（USART0 RX）
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);

    // 配置 USART0
    usart_deinit(USART0);
    usart_baudrate_set(USART0, Baud);  // 波特率 
    usart_word_length_set(USART0, USART_WL_8BIT);  // 8 位数据位
    usart_stop_bit_set(USART0, USART_STB_1BIT);  // 1 位停止位
    usart_parity_config(USART0, USART_PM_NONE);  // 无校验位
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);  // 禁用 RTS
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);  // 禁用 CTS
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);  // 使能接收
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);  // 使能发送
    usart_enable(USART0);  // 使能 USART0
}

// 重定向 printf 到 USART0
int fputc(int ch,FILE *p){
	
	usart_data_transmit(USART0, (uint8_t)ch);  // 发送一个字符
	while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));  // 等待发送完成
	
	return ch;
}



