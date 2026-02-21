#include "TIM.h"                   
#include "Mpu6050.h"

void pwm_gpio_config(void)
{
    rcu_periph_clock_enable(PWM_RTC);
    
    gpio_mode_set(PWM_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_Pin);
    gpio_output_options_set(PWM_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_Pin);

    gpio_af_set(PWM_GPIO_Port, GPIO_AF_1, PWM_Pin);
}

void TIMER0_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    
    pwm_gpio_config();          //PWM引脚初始化

    rcu_periph_clock_enable(RCU_TIMER0);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    
    timer_struct_para_init(&timer_initpara);
    timer_deinit(TIMER0);
	timer_primary_output_config(TIMER0,ENABLE);//高级定时器0,7需要使能
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 168-1;//168分频，配合后面的9999，产生50HZ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 20000-1;//计数值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

//    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);

    /* CH1 configuration in PWM mode1,duty cycle 0% */
//    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);//占空比0
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);//占空比
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);//占空比0
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,0);//占空比0

    
//    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    
    
//    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);
    
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    /* auto-reload preload enable */
    timer_enable(TIMER0);
}
/**************************************************************
*功  能：	调节PWM
*参  数：	无
*返回值：	无 
**************************************************************/
void SteerGear_PWM_CH1(uint16_t duty)
{
	if(duty > 20000)
	{
		duty = 20000;
	}
	timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,duty);
}

void SteerGear_PWM_CH2(uint16_t duty)
{
	if(duty > 20000)
	{
		duty = 20000;
	}
	timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,duty);
}

void SteerGear_PWM_CH3(uint16_t duty)
{
	if(duty > 20000)
	{
		duty = 20000;
	}
	timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,duty);
}




//定时器配置
void TIMER1_config(void)
{
    /* -----------------------------------------------------------------------
	  系统主频168MHZ,timer_initpara.prescaler为167，timer_initpara.period为99，频率就为10kHZ
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER1);
	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//AP1总线最高42MHZ,所以TIME1到168M需要4倍频
    timer_deinit(TIMER1);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 167;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 99;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
		nvic_irq_enable(TIMER1_IRQn, 0, 1);
		timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}

void TIMER2_config(void)
{
    /* -----------------------------------------------------------------------
	  系统主频168MHZ,timer_initpara.prescaler为167，timer_initpara.period为999，频率就为1KHZ
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//AP1总线最高42MHZ,所以TIME1到168M需要4倍频
    timer_deinit(TIMER2);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 167;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);
    
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER2_IRQn, 1, 1);
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* auto-reload preload enable */
    timer_enable(TIMER2);
}







uint32_t TimeCounter;
void TIMER1_IRQHandler(void)//(0.1ms中断一次)
{
	timer_flag_clear(TIMER1,TIMER_FLAG_UP);
    TimeCounter++;
}

uint32_t index_1ms = 0;
uint32_t index_5ms_flag=0;
uint8_t index_10ms_flag=0;
uint8_t index_50ms_flag=0;
uint8_t index_200ms_flag=0;
uint8_t index_500ms_flag=0;
uint8_t index_800ms_flag=0;
uint8_t index_1000ms_flag=0;
uint8_t index_1300ms_flag=0;

void TIMER2_IRQHandler(void)//(1ms中断一次)
{
	timer_flag_clear(TIMER2,TIMER_FLAG_UP);
    index_1ms++;
    if(index_1ms%1==0)
    {
        index_5ms_flag=1;
    }
    if(index_1ms%10==0)//每10ms执行一次 
    {
        index_10ms_flag =1;
    }
    if(index_1ms%50==0)//每20ms执行一次 
    {
        index_50ms_flag =1;
    }
    if(index_1ms%200==0)//每200ms执行一次 
    {   
        index_200ms_flag=1;
    }
    if(index_1ms%500==0)//每500ms执行一次 
    {
        index_500ms_flag = 1;
    }
    if(index_1ms%800==0)//每800ms执行一次 
    {
        index_800ms_flag = 1;
    }
    if(index_1ms%1000==0)//每800ms执行一次
    {
        index_1000ms_flag = 1;
    }
    if(index_1ms%1500==0)//每1500ms执行一次 
    {   
        index_1300ms_flag=1;
    }
}




