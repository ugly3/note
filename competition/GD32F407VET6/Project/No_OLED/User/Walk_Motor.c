#include "ALL.h"

uint8_t Walkmotor_last_status = 0;
uint8_t phasecw[4] ={GPIO_PIN_5,GPIO_PIN_4,GPIO_PIN_3,GPIO_PIN_2};//逆时针转电机导通相序D-C-B-A
uint8_t phaseccw[4]={GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5};//顺时针转电机导通相序A-B-C-D


void Walk_Motor_Init(void)
{
    rcu_periph_clock_enable(Motor_RCU_GPIOX);//使能GPIOE时钟
    gpio_mode_set(Motor_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, Motor_PIN);//PE2,PE3,PE4,PE5配置成输出
    gpio_output_options_set(Motor_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Motor_PIN);//配置成推挽输出，50M速度
}


void Set_Walkmotor(uint8_t status)
{
    uint8_t j;
    uint16_t i;
    if(status==1)
    {
        if(Walkmotor_last_status==0)//上一个状态为0,才执行，防止重复执行
        {
            Walkmotor_last_status=1;
            for(i=0; i<255*2+1; i++)//顺时针转动一圈
            {
                for(j=0;j<4;j++)
                {
                    gpio_bit_write(Motor_PORT,phaseccw[j],SET);//输出高
                     delay_1ms(5);//等待5ms
                    gpio_bit_write(Motor_PORT,phaseccw[j],RESET);//输出低
                }
            }
        }
    }
    else if(status==0)
    {
        if(Walkmotor_last_status==1)//上一个状态为1,才执行，防止重复执行
        {
            Walkmotor_last_status=0;
            for(i=0; i<255*2+1; i++)//逆时针转动一圈
            {
                for(j=0;j<4;j++)
                {
                    gpio_bit_write(Motor_PORT,phasecw[j],SET);//输出高
                    delay_1ms(5);//等待5ms
                    gpio_bit_write(Motor_PORT,phasecw[j],RESET);//输出低
                }
            }
        }
    }
}










