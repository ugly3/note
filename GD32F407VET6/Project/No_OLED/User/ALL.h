#ifndef __ALL_H
#define __ALL_H

#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "systick.h"
#include "Task_Cycle.h"
#include "TIM.h"          
#include "iic.h"
#include "oled.h"
#include "LED.H"
#include "Buzz.h"
#include "Walk_Motor.h"
#include "Key.h"
#include "Fan.h"
#include "Servo.h"
#include "ShuMaGuan.h" 
#include "HC.h"
#include "BH1750.h"
#include "DHT11.h"
#include "Mpu6050.h"
#include "HX711.h"
#include "Hc595.h"
#include "RC522.h"
#include "My_RFID_Check.h"
#include "RTC.h"
#include "ADC.h"
#include "Body_Hw.h"
#include "Usart.h"
#include "main.h"



extern uint8_t Buf[30];
extern int i,j;


//MPU6050
extern short Accel[3];//加速度
extern short Gyro [3];//角速度
extern long AccelData[3];//单位mg
extern long GyroData[3];//单位mdps


extern uint16_t temp_value;
extern uint16_t humi_value;
extern uint32_t MQ2_yuzhi,Huoyan_yuzhi;
//硬件控制标志位，0代表关，1代表开
extern uint8_t led_status;
extern uint8_t door_status;
extern uint8_t Walkmotor_status;
extern uint8_t Fan_status;
extern uint8_t HC_status;
extern uint8_t Weight_status;
extern uint8_t Body_Hw_ZD_status;
extern uint8_t Fan_ZD_status;


//定时器2 定时执行任务
extern uint32_t TimeCounter;
extern uint32_t index_5ms_flag;
extern uint8_t index_10ms_flag;
extern uint8_t index_50ms_flag;
extern uint8_t index_200ms_flag;
extern uint8_t index_500ms_flag;
extern uint8_t index_800ms_flag;
extern uint8_t index_1000ms_flag;
extern uint8_t index_1300ms_flag;

//任务开始标志
extern uint8_t task1_is_start;
extern uint8_t task2_is_start;
extern uint8_t task3_is_start;
extern uint8_t task4_is_start;
extern uint8_t task5_is_start;
extern uint8_t task6_is_start;
extern uint8_t task7_is_start;
extern uint8_t task8_is_start;
extern uint8_t task9_is_start;
extern uint8_t task10_is_start;
extern uint8_t task11_is_start;
extern uint8_t task12_is_start;
extern uint8_t task9_1_is_start;
extern uint8_t task9_2_is_start;
extern uint8_t task9_3_is_start;


extern uint8_t RFID_Stop;      //0代表检测RFID，1代表不检测RFID
extern uint8_t select_card;    //选择删除想要的RFID卡
extern uint8_t DLED_view_flag;       //数码管、点阵执行标志位，1代表关闭一些中断任务，防止数码管、点阵闪烁不稳定


//三个独立按键
extern int OLED_SELECT_flag;   //上一次选择参数
extern int OLED_SELECT;      
extern uint8_t OLED_CONFIRM;

//阈值
extern uint32_t LightData_yuzhi;
extern uint32_t temp_yuzhi,humi_yuzhi;

//超声波距离
extern uint32_t Distance;

//光照强度（整数，单位lx）
extern uint32_t LightData_Hex;

//存储RFID卡号ID
extern uint8_t UI0[4];		 	//卡0ID号
extern uint8_t UI1[4];	 		//卡1ID号
extern uint8_t UI2[4];			//卡2ID号
extern uint8_t UI3[4];			//卡3ID号

//三个ADC通道转换值
extern uint16_t adc_value[3];




#endif


