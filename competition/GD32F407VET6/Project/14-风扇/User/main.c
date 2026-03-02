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
#include "fan.h"

int main(void)
{
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
	  FAN_config();
    while(1) {
			uint8_t key= Btn_Scan(0);
			switch(key){
				case 1:
					FAN_on();
					break;
				case 2:
					FAN_off();
					break;
				case 3:
					gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
					break;
				case 4:
					break;
			}
			
			delay_1ms(10);
    }
}
