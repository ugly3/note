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
#include "iic.h"
#include "oled.h"
#include "fire.h"
int main(void)
{
		uint8_t flag=0;
    systick_config();   // 滴答定时器初始化
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		OLED_Init(&hiic1);
	  Fire_config();
//		OLED_PrintString(0, 0, "hello world!",1);
//		OLED_Refresh();
    while(1) {
			if(get_fire_state()==0){
				if(flag==0){
					OLED_PrintString(0, 0, "fire alarm",1);
					OLED_Refresh();
					Beep_on();
				}
				gpio_bit_toggle(LED1_GPIO_Port, LED1_Pin);
				flag=1;
			}else{
				if(flag==1){
					OLED_Clear();
					OLED_Refresh();
					gpio_bit_write(LED1_GPIO_Port, LED1_Pin,RESET);
					Beep_off();
				}
				flag=0;
			}
			
			delay_1ms(100);
    }
}
