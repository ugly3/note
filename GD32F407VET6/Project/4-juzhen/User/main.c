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
#include "ch455g.h"
#include "usart.h"

int main(void)
{
	  char chdata[20];
		uint8_t key_val;
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		IIC_config(&hiic2);
		OLED_Init(&hiic1);
	  CH455G_Init(&hiic2);
		OLED_PrintString(0, 0, "hello world!",1);
		OLED_Refresh();
    while(1) {
			key_val=Keyboard_proc_f();
		//**************************************************
			if(key_val){
				OLED_Clear();
				sprintf(chdata,"key:%d",key_val);
				OLED_PrintString(0, 0, chdata,1);
		    OLED_Refresh();
				printf("key:%d\n",key_val);
			}
			gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
			delay_1ms(50);
    }
}
