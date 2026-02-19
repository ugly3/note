#ifndef __FAN_H
#define __FAN_H


#include "gd32f4xx.h"                   // Device header
#include "Servo.h"

/*
 *接线：
 *  风扇信号线    PA11
 *  正极          5V
 *  负极          GND
*/

void Fan_motor_duty(uint8_t flag);
void Fan_ZD(uint8_t status);

extern uint32_t temp_yuzhi,humi_yuzhi;


#endif



