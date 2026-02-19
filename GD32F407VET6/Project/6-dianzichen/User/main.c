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
#include "hx711.h"
#include "usart.h"

int main(void)
{
		char chdat[50];
		long  Weight;
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		IIC_config(&hiic2);
		OLED_Init(&hiic1);
	  HX711_init();
		OLED_PrintString(0, 0, "hello world!",1);
		OLED_Refresh();
    while(1) {
			Weight= Get_Weight();
			printf("重量:%ld\r\n",Weight);
			OLED_Clear();
			sprintf(chdat,"Weight=%ld",Weight);
			OLED_PrintString(0,0,chdat,1);
			OLED_Refresh();
			
			gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
			delay_1ms(50);
    }
}
