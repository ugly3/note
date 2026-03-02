/**************************************************************************************
实验名称：LCD屏幕显示实验

注意事项：LCD屏幕需要提前下载工程目录下的test.HMI文件
				 
实验现象：点击LCD屏幕上的开关切换LED与蜂鸣器状态

***************************************************************************************/

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
	  USART_Init();
		LED_config();
		Btn_config();
	  Beep_config();
    while(1) {
		USART_Test();
    }
}
