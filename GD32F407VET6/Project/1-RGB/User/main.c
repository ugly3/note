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
#include "rgb-led.h"

//53-11
int main(void)
{

	systick_config();   // 滴答定时器初始化
	LED_config();
	Btn_config();
	Beep_config();
	RGBled_config();
	
	while(1) {
		gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
		delay_1ms(100);
		RGB_set_color(0,255,255,255,20);
		RGB_Show();
		delay_1ms(100);
		RGB_set_color_32(0,0x0000ff,20);
		RGB_Show();
    }
}
