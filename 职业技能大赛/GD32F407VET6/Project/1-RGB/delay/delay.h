#ifndef __DELAY_H
#define __DELAY_H


#include "main.h"
#include <rtthread.h>
                  
void delay_us(uint32_t nus);

void delay_ms(rt_tick_t ms);

#endif 
