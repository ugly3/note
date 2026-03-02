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
#include "infrared.h"  //���ⷢ��
#include "cba.h"      //���İ�
#include "ultrasonic.h"   //������
#include "canp_hostcom.h" //CAN���ݽ���
#include "hard_can.h"     //CAN��ʼ������
#include "bh1750.h"       //���նȴ�����
#include "power_check.h"  //�����ɼ�
#include "can_user.h"     //CAN�û����ݼ��ӣ����ģ�
#include "data_base.h"    //����־�����
#include "roadway_check.h"  // ����ѭ����·�����
#include "tba.h"           //�����
#include "swopt_drv.h"
#include "uart_a72.h"      //�նˣ������ޣ�
#include "Can_check.h"     //CAN���߼��
#include "delay.h"         //��ʱ����
#include "Timer.h"         //��ʱ��10ʱ��Ƭ��ѯ
#include "Rc522.h"         //RFID
#include "bkrc_voice.h"    //С��
#include "drive.h"         //�����������


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
//#include "car2.h"              /* 车2模块 - 替代从车功能 */

#if 0
void Car_Thread(void);
#endif

extern void xAuto_Run_Function(void);

/**********************************************roadway_check.c��ȫ�ֱ���**********************************************/
extern uint8_t Go_Flag;                    //ǰ����־λ
extern uint8_t Back_Flag;                  //���˱�־λ
extern uint8_t Track_Flag;                 //ѭ����־λ
extern uint8_t Stop_Flag;                  //ֹͣ��־λ
extern int LSpeed;                        //�����ٶ�
extern int RSpeed;                        //�����ٶ�
extern int Car_Spend;                     //���ٶȵ���ʱ����
extern uint16_t count;
/*********************************************************************************************************************/



/*********************************************����ȫ�ֺ���*********************************************************/
extern void USART6_Send_Byte(uint8_t byte);  //����6����һ���ֽڣ����͸�С��һ���ֽ�����������ָ���ʾ䣩
extern void USART6_Send_Length(uint8_t *buf,uint8_t length); //����6����һ������











/*****************************************************************************************************************/
#endif















