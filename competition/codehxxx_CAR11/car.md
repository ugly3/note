详细移植步骤
阶段一：创建新模块（车2）
在 codehxxx_CAR 11/MODULE/ 下创建新模块 car2/
步骤1.1: 创建目录和文件
MODULE/car2/
├── car2.h
├── car2.c
├── car2_motor.h
├── car2_motor.c
├── car2_sensor.h
├── car2_sensor.c
├── car2_cmd.h
└── car2_cmd.c
步骤1.2: 创建头文件 car2.h
#ifndef __CAR2_H
#define __CAR2_H
#include "stm32f4xx.h"
// 车2状态
typedef struct {
    uint8_t Work_Mode;          // 工作模式
    uint8_t Task_Status;         // 任务状态
    uint8_t QR_Data[8];         // 二维码数据
    uint8_t RFID_Data[4];       // RFID数据
    uint8_t Track_Sensor;       // 巡线传感器
    uint16_t Ultrasonic_Dist;   // 超声波距离
    uint8_t Finish_Flag;        // 完成任务标志
} Car2_Status_TypeDef;
extern Car2_Status_TypeDef Car2_Status;
// 函数声明
void Car2_Init(void);
void Car2_Task(void);
void Car2_Process_Command(uint8_t* cmd);
#endif
---
阶段二：移植通信协议
步骤2.1: 创建 car2_cmd.h
#ifndef __CAR2_CMD_H
#define __CAR2_CMD_H
#include "stm32f4xx.h"
// 通信地址
#define CAR2_ADDR        0x02   // 车2地址
#define CAR1_ADDR        0x01   // 车1地址
// 帧格式: {0x55, 目标地址, 主指令, 副指令, 数据1, 数据2, 数据3, 校验和, 0xBB}
#define FRAME_HEADER    0x55
#define FRAME_TAIL      0xBB
// 车2接收命令 (车1发送给车2)
#define CMD_CAR2_START          0xA0  // 启动车2
#define CMD_CAR2_START_2       0xA1  // 二次启动
#define CMD_CAR2_RFID           0xA2  // 接收RFID
#define CMD_CAR2_QR_FRONT       0xA3  // 报警台前三位
#define CMD_CAR2_QR_BACK        0xA4  // 报警台后三位
#define CMD_CAR2_TFT_3D         0xA5  // TFT 3D显示
// 车2发送命令 (车2发送给车1)
#define CMD_CAR2_QR_SEND        0x03  // 发送二维码给车1
#define CMD_CAR2_FINISH         0xAA  // 任务完成
// 校验和计算
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2);
// 接收处理
void Car2_ParseCommand(uint8_t* rxBuf);
// 发送给车1
void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3);
#endif
步骤2.2: 创建 car2_cmd.c
#include "car2_cmd.h"
#include "canp_hostcom.h"  // 使用主车的ZigBee发送函数
Car2_Status_TypeDef Car2_Status;
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2)
{
    return (cmd1 + cmd2 + d1 + d2) % 256;
}
// 发送给车1
void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3)
{
    uint8_t cmd[8] = {
        FRAME_HEADER,
        CAR1_ADDR,    // 目标地址 = 车1
        cmd1,
        cmd2,
        d1, d2, d3,
        0x00,        // 校验和
        FRAME_TAIL
    };
    
    cmd[6] = Car2_GetChecksum(cmd[2], cmd[3], cmd[4], cmd[5]);
    Send_ZigbeeData_To_Fifo(cmd, 8);
}
// 解析来自车1的命令
void Car2_ParseCommand(uint8_t* rxBuf)
{
    if (rxBuf[0] != FRAME_HEADER || rxBuf[7] != FRAME_TAIL) return;
    if (rxBuf[1] != CAR2_ADDR) return;  // 不是发给车2的
    
    uint8_t cmd1 = rxBuf[2];
    uint8_t cmd2 = rxBuf[3];
    
    switch (cmd1) {
        case CMD_CAR2_START:
            if (cmd2 == 0xA0) {
                Car2_Status.Work_Mode = 1;  // 启动
            } else if (cmd2 == 0xA1) {
                Car2_Status.Work_Mode = 2;  // 二次启动
            }
            break;
            
        case CMD_CAR2_RFID:
            Car2_Status.RFID_Data[0] = rxBuf[3];
            Car2_Status.RFID_Data[1] = rxBuf[4];
            Car2_Status.RFID_Data[2] = rxBuf[5];
            break;
            
        case CMD_CAR2_QR_FRONT:
            // 处理报警台前三位
            break;
            
        case CMD_CAR2_QR_BACK:
            // 处理报警台后三位
            break;
            
        case CMD_CAR2_TFT_3D:
            // 处理3D显示数据
            break;
    }
}
---
阶段三：移植电机控制
步骤3.1: 创建 car2_motor.h
#ifndef __CAR2_MOTOR_H
#define __CAR2_MOTOR_H
#include "stm32f4xx.h"
// 电机引脚定义 (根据实际硬件修改)
#define MOTOR_PWM_FREQ    10000   // 10KHz
// 函数声明
void Car2_Motor_Init(void);
void Car2_Go(uint8_t speed);
void Car2_Back(uint8_t speed);
void Car2_TurnLeft(uint8_t speed);
void Car2_TurnRight(uint8_t speed);
void Car2_Stop(void);
void Car2_SpeedCtr(int16_t leftSpeed, int16_t rightSpeed);
#endif
步骤3.2: 创建 car2_motor.c
#include "car2_motor.h"
#include "pwm.h"           // 主车的PWM驱动
#include "gpio.h"          // 主车的GPIO驱动
// 使用主车的电机驱动模块
extern void Motor_SetSpeed(int16_t left, int16_t right);
void Car2_Motor_Init(void)
{
    // 初始化PWM引脚 (使用主车的电机驱动)
    // 根据实际硬件配置
}
void Car2_Go(uint8_t speed)
{
    Car2_SpeedCtr(speed, speed);
}
void Car2_Back(uint8_t speed)
{
    Car2_SpeedCtr(-speed, -speed);
}
void Car2_TurnLeft(uint8_t speed)
{
    Car2_SpeedCtr(-speed, speed);
}
void Car2_TurnRight(uint8_t speed)
{
    Car2_SpeedCtr(speed, -speed);
}
void Car2_Stop(void)
{
    Car2_SpeedCtr(0, 0);
}
void Car2_SpeedCtr(int16_t leftSpeed, int16_t rightSpeed)
{
    Motor_SetSpeed(leftSpeed, rightSpeed);
}
---
阶段四：移植传感器
步骤4.1: 创建 car2_sensor.h
#ifndef __CAR2_SENSOR_H
#define __CAR2_SENSOR_H
#include "stm32f4xx.h"
// 巡线传感器
uint8_t Car2_Read_TrackSensor(void);
// 超声波测距
uint16_t Car2_Read_Ultrasonic(void);
// RFID读取
uint8_t Car2_Read_RFID(uint8_t* rfidData);
// 二维码读取
uint8_t Car2_Read_QR(uint8_t* qrData);
#endif
---
阶段五：整合到主程序
步骤5.1: 在 main.c 中添加
// main.c
#include "car2.h"
#include "car2_cmd.h"
#include "car2_motor.h"
#include "car2_sensor.h"
extern uint8_t Zigb_Rx_Buf[16];  // ZigBee接收缓冲区
int main(void)
{
    Hardware_Init();
    
    // 初始化车2模块
    Car2_Motor_Init();
    Car2_Init();
    
    while(1)
    {
        // 检查ZigBee接收数据
        if (Zigbee_Rx_flag)
        {
            Car2_ParseCommand(Zigb_Rx_Buf);
            Zigbee_Rx_flag = 0;
        }
        
        // 车2任务处理
        Car2_Task();
        
        delay_ms(10);
    }
}
---
需要修改的文件清单
| 原从车模块 | 移植到STM32 | 位置 |
|-----------|------------|------|
| DCMotor.ino | car2_motor.c,h | MODULE/car2/ |
| Communication.ino | car2_cmd.c,h | MODULE/car2/ |
| Ultrasonic.ino | car2_sensor.c | MODULE/car2/ |
| QR.ino | car2_sensor.c | MODULE/car2/ |
| DCMotor.ino 巡线 | car2_track.c | MODULE/car2/ |
---
关键点总结
1. 通信: 使用 Send_ZigbeeData_To_Fifo() 发送，解析 Zigb_Rx_Buf 接收
2. 电机: 复用主车的 motor_drive 模块
3. 校验和: checksum = (cmd1 + cmd2 + d1 + d2) % 256
4. 帧格式: {0x55, 目标地址, 主指令, 副指令, 数据1, 数据2, 数据3, 校验和, 0xBB}
---
