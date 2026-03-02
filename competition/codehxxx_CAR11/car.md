# STM32 车2（从车）模块移植手册

本手册详细介绍了将原有从车功能移植到 STM32 环境下的步骤，涉及目录结构、通信协议、电机控制及传感器整合。

## 1. 项目结构与模块创建

在 `codehxxx_CAR 11/MODULE/` 目录下创建新模块 `car2/`。

## 1.1 目录结构

Plaintext

```
MODULE/car2/
├── car2.h          // 模块主头文件
├── car2.c          // 任务逻辑实现
├── car2_motor.h    // 电机控制头文件
├── car2_motor.c    // 电机驱动实现
├── car2_sensor.h   // 传感器驱动头文件
├── car2_sensor.c   // 传感器逻辑实现
├── car2_cmd.h      // 通信协议定义
└── car2_cmd.c      // 协议解析与发送实现
```

## 1.2 核心状态结构体 (`car2.h`)

C

```
#ifndef __CAR2_H
#define __CAR2_H

#include "stm32f4xx.h"

// 车2状态结构体定义
typedef struct {
    uint8_t Work_Mode;          // 工作模式
    uint8_t Task_Status;        // 任务状态
    uint8_t QR_Data[8];         // 二维码数据
    uint8_t RFID_Data[4];       // RFID数据
    uint8_t Track_Sensor;       // 巡线传感器状态
    uint16_t Ultrasonic_Dist;   // 超声波距离
    uint8_t Finish_Flag;        // 任务完成标志
} Car2_Status_TypeDef;

extern Car2_Status_TypeDef Car2_Status;

// 函数声明
void Car2_Init(void);
void Car2_Task(void);
void Car2_Process_Command(uint8_t* cmd);

#endif
```

---

## 2. 通信协议移植

## 2.1 协议定义 (`car2_cmd.h`)

**帧格式：** `{0x55, 目标地址, 主指令, 副指令, 数据1, 数据2, 数据3, 校验和, 0xBB}`

|属性|值|描述|
|---|---|---|
|**Header**|`0x55`|帧头|
|**Tail**|`0xBB`|帧尾|
|**CAR1_ADDR**|`0x01`|车1（主车）地址|
|**CAR2_ADDR**|`0x02`|车2（从车）地址|

Export to Sheets

## 2.2 核心逻辑实现 (`car2_cmd.c`)

C

```
#include "car2_cmd.h"
#include "canp_hostcom.h" // 引用主车 ZigBee 发送函数

Car2_Status_TypeDef Car2_Status;

// 校验和计算：(cmd1 + cmd2 + d1 + d2) % 256
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2) {
    return (cmd1 + cmd2 + d1 + d2) % 256;
}

// 发送数据至车1
void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3) {
    uint8_t cmd[9] = {
        FRAME_HEADER,
        CAR1_ADDR,
        cmd1,
        cmd2,
        d1, d2, d3,
        0x00, // 校验和占位
        FRAME_TAIL
    };
    cmd[7] = Car2_GetChecksum(cmd[2], cmd[3], cmd[4], cmd[5]);
    Send_ZigbeeData_To_Fifo(cmd, 9);
}
```

---

## 3. 电机控制移植 (`car2_motor.c`)

直接复用主车的底层驱动模块。

C

```
#include "car2_motor.h"
#include "pwm.h"
#include "gpio.h"

extern void Motor_SetSpeed(int16_t left, int16_t right);

void Car2_SpeedCtr(int16_t leftSpeed, int16_t rightSpeed) {
    Motor_SetSpeed(leftSpeed, rightSpeed);
}

void Car2_Go(uint8_t speed)        { Car2_SpeedCtr(speed, speed); }
void Car2_Back(uint8_t speed)      { Car2_SpeedCtr(-speed, -speed); }
void Car2_Stop(void)               { Car2_SpeedCtr(0, 0); }
```

---

## 4. 主程序整合 (`main.c`)

在 STM32 主循环中添加初始化与任务调度。

C

```
#include "car2.h"
#include "car2_cmd.h"

int main(void) {
    Hardware_Init(); // 硬件基础初始化
    
    Car2_Motor_Init();
    Car2_Init();
    
    while(1) {
        // 处理通信
        if (Zigbee_Rx_flag) {
            Car2_ParseCommand(Zigb_Rx_Buf);
            Zigbee_Rx_flag = 0;
        }
        
        // 运行车2业务逻辑
        Car2_Task();
        
        delay_ms(10);
    }
}
```

---

## 5. 移植对照清单

| 原 Arduino 模块 (.ino)  | 移植后 STM32 文件     | 目标位置           |
| -------------------- | ---------------- | -------------- |
| `DCMotor.ino`        | `car2_motor.c/h` | `MODULE/car2/` |
| `Communication.ino`  | `car2_cmd.c/h`   | `MODULE/car2/` |
| `Ultrasonic.ino`     | `car2_sensor.c`  | `MODULE/car2/` |
| `QR.ino`             | `car2_sensor.c`  | `MODULE/car2/` |
| `DCMotor.ino` (巡线部分) | `car2_track.c`   | `MODULE/car2/` |
|                      |                  |                |
