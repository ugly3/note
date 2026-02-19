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
#include "74hc595.h"
#include "matrixpixel.h"
#include "usart.h"




int main(void)
{
	int16_t count=0;
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		IIC_config(&hiic2);
		OLED_Init(&hiic1);
		OLED_PrintString(0, 0, "hello world!",1);
		OLED_Refresh();
		hc595_init();
		printf("run\r\n");
	
		
    while(1) {
			
			matrixSendData(count,0XFF);
			count++;
			if(count>7)count=0;
			 //read_RFID_all();
//		LED595SendData(0,Tab[0]);
//		delay_1ms(1);
//		LED595SendData(1,Tab[1]);
//		delay_1ms(1);
//		LED595SendData(2,Tab[2]);
//		delay_1ms(1);
//		LED595SendData(3,Tab[3]);
//		delay_1ms(1);

			gpio_bit_toggle(LED1_GPIO_Port, LED1_Pin);
			delay_1ms(100);
			
    }
}
