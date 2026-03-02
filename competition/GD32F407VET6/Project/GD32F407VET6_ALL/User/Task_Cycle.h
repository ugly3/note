#ifndef __TASK_CYCLE_H
#define __TASK_CYCLE_H

#include "gd32f4xx.h"                   // Device header

void Task_cycle(void);

extern uint16_t temp_value;
extern uint16_t humi_value;

extern uint8_t led_status;
extern uint8_t door_status;
extern uint8_t Walkmotor_status;
extern uint8_t Fan_status;
extern uint8_t HC_status;
extern uint8_t Weight_status;
extern uint8_t Body_Hw_ZD_status;
extern uint8_t Fan_ZD_status;

extern uint32_t MQ2_yuzhi,Huoyan_yuzhi;

#endif


