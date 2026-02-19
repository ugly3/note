#include "74hc595.h"


void hc595_init(void ){

	
	rcu_periph_clock_enable(SI_RTC);
	rcu_periph_clock_enable(RCK_RTC);
	rcu_periph_clock_enable(SCK_RTC);

	
	
	gpio_mode_set(SI_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, SI_PIN);
	gpio_mode_set(RCK_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, RCK_PIN);
	gpio_mode_set(SCK_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, SCK_PIN);
	
	
	gpio_output_options_set(SI_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SI_PIN);	
	gpio_output_options_set(RCK_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCK_PIN);
	gpio_output_options_set(SCK_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SCK_PIN);
	
	gpio_bit_write(SI_Port, SI_PIN, RESET);
	gpio_bit_write(RCK_Port, RCK_PIN, RESET);
	gpio_bit_write(SCK_Port, SCK_PIN, RESET);
	
}


void hc595_WriteByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        // 设置 SER 引脚（发送数据的当前位）
        if ((data & 0x80) == 0X80) {
            gpio_bit_write(SI_Port,SI_PIN,SET);
        } else {
            gpio_bit_write(SI_Port,SI_PIN,RESET);
        }
				data <<=1;
        // 产生 SRCLK 上升沿，将数据移入移位寄存器
        gpio_bit_write(SCK_Port, SCK_PIN,SET);
        delay_us(10);  // 短暂延时
        gpio_bit_write(SCK_Port, SCK_PIN,RESET);
				delay_us(10);
    }

    // 产生 RCLK 上升沿，将数据输出到并行端口
    gpio_bit_write(RCK_Port, SCK_PIN,SET);
    delay_us(10);  // 短暂延时
    gpio_bit_write(RCK_Port, SCK_PIN,RESET);
}

void hc595_WriteByte16(uint16_t data) {
    for (int i = 0; i < 16; i++) {
        // 设置 SER 引脚（发送数据的当前位）
        if ((data & 0x8000) == 0X8000) {
            gpio_bit_write(SI_Port, SI_PIN,SET);
        } else {
            gpio_bit_write(SI_Port, SI_PIN,RESET);
        }
				data <<=1;
        // 产生 SRCLK 上升沿，将数据移入移位寄存器
        gpio_bit_write(SCK_Port, SCK_PIN,SET);
        delay_us(10);  // 短暂延时
        gpio_bit_write(SCK_Port, SCK_PIN,RESET);
				delay_us(10);
    }

    // 产生 RCLK 上升沿，将数据输出到并行端口
    gpio_bit_write(RCK_Port, RCK_PIN,SET);
    delay_us(10);  // 短暂延时
    gpio_bit_write(RCK_Port, RCK_PIN,RESET);
}



//void HC595_Send_16Bit(hc595_bus_typ *hbus,uint16_t data) 
//{  
//	uint16_t i = 0;
//	for(i=0;i<16;i++)           	
//	{ 
//	    if((data & 0x8000) == 0X8000)
//		{
//			gpio_bit_write(hbus->SI_Port, hbus->SI_PIN,SET);
//		}
//		else
//		{
//			gpio_bit_write(hbus->SI_Port, hbus->SI_PIN,RESET); 
//		}
//		delay_us(10);
//		gpio_bit_write(hbus->RCK_Port, hbus->RCK_PIN,RESET);
//		delay_us(10);
//		gpio_bit_write(hbus->RCK_Port, hbus->RCK_PIN,SET);	                //移位输入时钟，上升沿输入 
//		delay_us(10);
//		data <<=1;
// 	} 
//	gpio_bit_write(hbus->RCK_Port, hbus->RCK_PIN,RESET);        
//	delay_us(10);            			//并行输出时钟 对消隐会起作用     
//	gpio_bit_write(hbus->RCK_Port, hbus->RCK_PIN,SET);
//}



