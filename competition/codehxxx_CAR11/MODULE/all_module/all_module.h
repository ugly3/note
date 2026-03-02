#ifndef __ALL_MODULE_H
#define __ALL_MODULE_H




/*System HeadFile*/
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"
#include "sys.h"

/*Original HeadFile*/
#include "infrared.h"  //红外发射
#include "cba.h"      //核心板
#include "ultrasonic.h"   //超声波
#include "canp_hostcom.h" //CAN数据交互
#include "hard_can.h"     //CAN初始化与检测
#include "bh1750.h"       //光照度传感器
#include "power_check.h"  //电量采集
#include "can_user.h"     //CAN用户数据监视（核心）
#include "data_base.h"    //各标志物命令集
#include "roadway_check.h"  // 主车循迹与路况检测
#include "tba.h"           //任务板
#include "swopt_drv.h"
#include "uart_a72.h"      //终端（主车无）
#include "Can_check.h"     //CAN总线检测
#include "delay.h"         //延时函数
#include "Timer.h"         //定时器10时间片轮询
#include "Rc522.h"         //RFID
#include "bkrc_voice.h"    //小创
#include "drive.h"         //主车电机驱动


/*My HeadFile*/
#include "mixture.h"
#include "follow_car.h"
#include "android.h"
#include "communication.h"
#include "motor_drive.h"
#include "barrier_gate.h"
#include "etc.h"
#include "led_display.h"
#include "rfid.h"
#include "smart_street_light.h"
#include "smart_traffic_light.h"
#include "smart_tft_display.h"
#include "special_landform.h"
#include "three_dim_carport.h"
#include "three_dim_display.h"
#include "voice_report.h"
#include "wireless_charge.h"
#include "smoke_tower.h"
#include "xiaochuang.h"

#if 0
void Car_Thread(void);
#endif

extern void xAuto_Run_Function(void);

/**********************************************roadway_check.c的全局变量**********************************************/
extern uint8_t Go_Flag;                    //前进标志位
extern uint8_t Back_Flag;                  //后退标志位
extern uint8_t Track_Flag;                 //循迹标志位
extern uint8_t Stop_Flag;                  //停止标志位
extern int LSpeed;                        //左轮速度
extern int RSpeed;                        //右轮速度
extern int Car_Spend;                     //车速度的临时变量
extern uint16_t count;
/*********************************************************************************************************************/



/*********************************************常用全局函数*********************************************************/
extern void USART6_Send_Byte(uint8_t byte);  //串口6发送一个字节（发送给小创一个字节命令来播放指定词句）
extern void USART6_Send_Length(uint8_t *buf,uint8_t length); //串口6发送一个数组











/*****************************************************************************************************************/
#endif















