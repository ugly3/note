#include "ALL.h"

uint32_t HalTime1,HalTime2;


void HC_Init(void)
{
    rcu_periph_clock_enable(HC_RCU_GPIOX);//使能GPIOA时钟	
    gpio_mode_set(HC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, HC_PIN_Trig);//PA0配置成输出
    gpio_output_options_set(HC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, HC_PIN_Trig);//配置成推挽输出，50M速度
    gpio_mode_set(HC_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, HC_PIN_Echo);//PA1配置成输入
    TIMER1_config();
}

uint32_t Distance_Calculate(uint32_t count)//传入时间单位100us
{
    uint32_t Distance = 0;
    Distance = (uint32_t)(((float)count *17)/10);//距离单位cm,声速340M/S，时间*速度/2=距离
    return Distance;
}


void HC_Get_Data(uint8_t status)
{
    if(status)
    {
        gpio_bit_write(HC_PORT, GPIO_PIN_0,RESET);//预先拉低Trig引脚
		delay_1ms(5);
		gpio_bit_write(HC_PORT, GPIO_PIN_0,SET);//拉高Trig引脚
		delay_1us(20);
		gpio_bit_write(HC_PORT, GPIO_PIN_0,RESET);//拉低Trig引脚
		delay_1us(20);
		while(gpio_input_bit_get(HC_PORT, HC_PIN_Echo) == 0);//如果是低电平，一直等
		HalTime1= TimeCounter;
		while(gpio_input_bit_get(HC_PORT, HC_PIN_Echo) == 1);//如果是高电平一直等
		if(TimeCounter>HalTime1)
		{
            HalTime2 = TimeCounter-HalTime1;
            if(HalTime2<0x300)
            {
                Distance = Distance_Calculate(HalTime2);//计算距离值
                if(Distance<30)
                    door_status=1;
                else
                    door_status=0;
            }
		} 
    }        
}
