/********************************************************************************
  * 文 件 名: main.c
  * 功能介绍:          
  ******************************************************************************
  * 注意事项:
*********************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

#include "led.h"
#include "button.h"
#include "beep.h"
#include "usart.h"


int main(void)
{
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
    while(1) {
			usart_data_transmit(USART0, 'a'); 
			printf("aa");
			gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
			delay_1ms(100);
    }
}
