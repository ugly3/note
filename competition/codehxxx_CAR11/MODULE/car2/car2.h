/**
 * @file car2.h
 * @brief 车2模块 - 替代从车功能，实现与主车1的通信
 * 
 * 车2作为从车功能的载体，运行在STM32平台上
 * 与主车1通过ZigBee无线通信
 */
#ifndef __CAR2_H
#define __CAR2_H

#include "all_module.h"

/*==================== 车2通信地址定义 ====================*/
#define CAR2_ADDR         0x02   /* 车2地址 */
#define CAR1_ADDR         0x01   /* 车1地址(主车) */

/*==================== 帧格式定义 ====================*/
/* 帧格式: {0x55, 目标地址, 主指令, 副指令, 数据1, 数据2, 数据3, 校验和, 0xBB} */
#define FRAME_HEADER      0x55
#define FRAME_TAIL        0xBB

/*==================== 车2接收命令 (车1->车2) ====================*/
#define CMD_CAR2_START       0xA0   /* 启动车2 */
#define CMD_CAR2_START_2     0xA1   /* 二次启动车2 */
#define CMD_CAR2_RFID        0xA2   /* 接收RFID数据 */
#define CMD_CAR2_QR_FRONT    0xA3   /* 报警台前三位 */
#define CMD_CAR2_QR_BACK     0xA4   /* 报警台后三位 */
#define CMD_CAR2_TFT_3D      0xA5   /* TFT 3D显示数据 */

/*==================== 车2发送命令 (车2->车1) ====================*/
#define CMD_CAR2_QR_SEND     0x03   /* 发送二维码数据给车1 */
#define CMD_CAR2_FINISH      0xAA   /* 车2任务完成 */
#define CMD_CAR2_STATUS      0xAB   /* 车2状态上报 */

/*==================== 车2状态定义 ====================*/
typedef struct {
    uint8_t Work_Mode;           /* 工作模式: 0=空闲, 1=启动, 2=运行中 */
    uint8_t Task_Status;         /* 任务状态 */
    uint8_t QR_Data[8];          /* 二维码数据 */
    uint8_t RFID_Data[4];        /* RFID数据 */
    uint8_t Track_Sensor;        /* 巡线传感器数据 */
    uint16_t Ultrasonic_Dist;    /* 超声波距离 */
    uint8_t Finish_Flag;         /* 完成任务标志 */
    uint8_t Start_Flag;          /* 启动标志 */
    uint8_t Barrier_State;       /* 道闸状态 */
} Car2_Status_TypeDef;

/*==================== 外部变量声明 ====================*/
extern Car2_Status_TypeDef Car2_Status;

/*==================== 函数声明 ====================*/

/* 初始化 */
void Car2_Init(void);

/* 任务处理 */
void Car2_Task(void);

/* 命令解析 - 从ZigBee接收数据中解析车2命令 */
void Car2_ParseCommand(uint8_t* rxBuf);

/* 校验和计算 */
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2);

/* 发送给车1 */
void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3);

/* 发送二维码数据给车1 */
void Car2_SendQRData(uint8_t* qrData);

/* 发送任务完成标志给车1 */
void Car2_SendFinish(void);

/* 发送状态给车1 */
void Car2_SendStatus(void);

/* 电机控制 */
void Car2_Motor_Init(void);
void Car2_Go(uint8_t speed, uint16_t time);
void Car2_Back(uint8_t speed, uint16_t time);
void Car2_TurnLeft(uint8_t speed, uint16_t time);
void Car2_TurnRight(uint8_t speed, uint16_t time);
void Car2_Stop(void);

/* 巡线 */
void Car2_Track(uint8_t speed);
void Car2_Track_Time(uint8_t speed, uint16_t time);

#endif /* __CAR2_H */
