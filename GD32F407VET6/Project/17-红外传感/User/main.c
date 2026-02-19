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
#include "ir.h"
int main(void)
{
		uint8_t flag=0;
    systick_config();   // 滴答定时器初始化
		LED_config();
		Btn_config();
	  Beep_config();
		IIC_config(&hiic1);
		OLED_Init(&hiic1);
		IR_Init();
		
    while(1) {
			 
      IR_Test();
			delay_1ms(10);
    }
}
