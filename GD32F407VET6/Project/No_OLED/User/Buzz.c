#include "ALL.h"

void Buzze_Init(void)
{
    rcu_periph_clock_enable(Buzze_RCU_GPIOX);//使能GPIOB时钟
    gpio_mode_set(Buzze_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, Buzze_PIN);//输出
    gpio_output_options_set(Buzze_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Buzze_PIN);//推挽输出，50M速度
    gpio_bit_reset(Buzze_PORT, Buzze_PIN);//输出低电平（不叫）
}


void Set_Buzze(uint8_t status)
{
    gpio_bit_write(Buzze_PORT, Buzze_PIN, status == Buzze_ON ? SET : RESET);
}


void Buzze_roll(uint8_t count)
{
    uint8_t i;
    for(i=0;i<count;i++)
    {
        gpio_bit_set(Buzze_PORT, Buzze_PIN);
        delay_1ms(90);
        gpio_bit_reset(Buzze_PORT, Buzze_PIN);
        delay_1ms(90);
    }
}

