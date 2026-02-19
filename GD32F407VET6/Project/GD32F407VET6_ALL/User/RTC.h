#ifndef __RTC_H
#define __RTC_H


#include "gd32f4xx.h"                   // Device header


void MyRTC_Init(void);
void RTC_Show_Time(void);


extern rtc_parameter_struct   rtc_initpara;












#endif



