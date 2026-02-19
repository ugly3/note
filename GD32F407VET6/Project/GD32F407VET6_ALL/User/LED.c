#include "LED.h"
#include "systick.h"

void LED_Init(void)
{
    rcu_periph_clock_enable(LED_RCU_GPIOX);//使能GPIOB时钟
    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);//配置成输出
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PIN);//配置成推挽输出，50M速度
    gpio_bit_set(LED_PORT, LED_PIN);//输出高电平（灭）
}


void set_led(uint8_t status)
{
    gpio_bit_write(LED_PORT, LED_PIN, status == LED_ON ? RESET : SET);	
}


void led_roll(uint8_t count)
{
    uint8_t i;
    for(i=0;i<count;i++)
    {
        gpio_bit_reset(LED_PORT, LED_PIN);
        delay_1ms(400);
        gpio_bit_set(LED_PORT, LED_PIN);//PB4输出高电平（灭）
        delay_1ms(400);
    }
}

