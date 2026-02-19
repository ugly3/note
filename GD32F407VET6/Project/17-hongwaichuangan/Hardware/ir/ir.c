#include "ir.h"



void IR_Init(void)
{
	rcu_periph_clock_enable(IR_RTC);

	gpio_mode_set(IR_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, IR_Pin);
}

void IR_Test(void)
{
	if(IR){
		OLED_Clear();
		OLED_PrintString(0,0, "somebody", 1);
    OLED_Refresh();
		printf("有人\r\n");
	}else{
		OLED_Clear();
		OLED_PrintString(0,0, "nobody ", 1);
    OLED_Refresh();
		printf("无人\r\n");printf("有人\r\n");
	}
	
}


