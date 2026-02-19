#include "servo.h"




void GPIO_Config(void) {
    // 使能 GPIOC 时钟
    rcu_periph_clock_enable(SERVO_RTC);

    // 配置 PC9 为复用推挽输出（TIMER2_CH3）
    gpio_af_set(SERVO_GPIO_Port, GPIO_AF_2, SERVO_Pin);  // AF2 是 TIMER2 的复用功能
    gpio_mode_set(SERVO_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, SERVO_Pin);
    gpio_output_options_set(SERVO_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SERVO_Pin);
}

void TIMER2_PWM_Config(void) {
    // 使能 TIMER2 时钟
    rcu_periph_clock_enable(RCU_TIMER2);

    // 配置 TIMER2
    timer_deinit(TIMER2);

    // 设置 TIMER2 为向上计数模式
    timer_counter_value_config(TIMER2, 0);
    timer_prescaler_config(TIMER2, 83, TIMER_PSC_RELOAD_NOW);  // 84MHz / (83 + 1) = 1MHz
    timer_autoreload_value_config(TIMER2, 19999);  // 1MHz / 20000 = 50Hz（20ms 周期）

    // 配置 TIMER2_CH3 为 PWM 模式 1
    timer_channel_output_mode_config(TIMER2, TIMER_CH_3, TIMER_OC_MODE_PWM1);
    timer_channel_output_polarity_config(TIMER2, TIMER_CH_3, TIMER_OC_POLARITY_LOW);
    timer_channel_output_fast_config(TIMER2, TIMER_CH_3, TIMER_OC_FAST_DISABLE);

    // 设置初始占空比（1.5ms 脉冲宽度）
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 1500);  // 1.5ms / 20ms * 20000 = 1500

    // 使能 TIMER2_CH3 输出
    timer_channel_output_state_config(TIMER2, TIMER_CH_3, TIMER_CCX_ENABLE);

    // 使能 TIMER2
    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);
}

void servo_init(void){
	GPIO_Config();
	TIMER2_PWM_Config();
}

void Set_Servo_Angle(uint16_t angle) {
    // 限制脉冲宽度在 500 到 2500 之间（0.5ms 到 2.5ms）
		uint16_t pulse_width;
		pulse_width=angle*11.11+500;
    if (pulse_width < 500) pulse_width = 500;
    if (pulse_width > 2500) pulse_width = 2500;

    // 设置 TIMER2_CH3 的脉冲宽度
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, pulse_width);
}

