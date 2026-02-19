#include "ALL.h"


void Body_Hw_Init(void)
{
    rcu_periph_clock_enable(Body_Hw_GPIOX);//使能GPIOB时钟
    gpio_mode_set(Body_Hw_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, Body_Hw_PIN);//配置成下拉输入
}

void Body_Hw_ZD(uint8_t status)
{
    if(status==1)
    {
        if(gpio_input_bit_get(Body_Hw_PORT,Body_Hw_PIN)==1)
        {
                task1_is_start=1;
                led_status=1;//开灯
        }    
        else
        {
                task1_is_start=0;
                led_status=0;//关灯
        }

     }
    
}
