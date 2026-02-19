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
#include "dht11.h"
#include "oled.h"
#include "iic.h"

int main(void)
{
		uint16_t temp,humi;
		char chdat[30];
    systick_config();   // 滴答定时器初始化
	  USART0_init(115200);
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		OLED_Init(&hiic1);
		DHT11_Init();
    while(1) {
			DHT11_Read_Data(&temp,&humi);
			printf("温度=%d,湿度=%d\r\n",temp,humi);
			OLED_Clear();
			sprintf(chdat,"temp=%d",temp);
			OLED_PrintString(0,0,chdat,1);
			sprintf(chdat,"humi=%d",humi);
			OLED_PrintString(0,17,chdat,1);
			OLED_Refresh();
			gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
			delay_1ms(200);
    }
}
