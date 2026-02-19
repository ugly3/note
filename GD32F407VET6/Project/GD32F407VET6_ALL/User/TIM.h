#ifndef __TIM_H
#define __TIM_H


#include "gd32f4xx.h"                   // Device header
#define PWM_RTC 				RCU_GPIOA
#define PWM_GPIO_Port		GPIOA
#define PWM_Pin 				 GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11//GPIO_PIN_8 |OLED”√µΩPA8

void TIMER0_config(void);
void TIMER1_config(void);
void TIMER2_config(void);

void SteerGear_PWM_CH1(uint16_t duty);
void SteerGear_PWM_CH2(uint16_t duty);
void SteerGear_PWM_CH3(uint16_t duty);

void pwm_gpio_config(void);


extern uint32_t TimeCounter;
extern uint32_t index_5ms_flag;
extern uint8_t index_10ms_flag;
extern uint8_t index_50ms_flag;
extern uint8_t index_200ms_flag;
extern uint8_t index_500ms_flag;
extern uint8_t index_800ms_flag;
extern uint8_t index_1000ms_flag;
extern uint8_t index_1300ms_flag;













#endif

