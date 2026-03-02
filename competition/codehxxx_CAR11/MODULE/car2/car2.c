/**
 * @file car2.c
 * @brief 车2模块实现 - 替代从车功能
 */
#include "car2.h"
#include "data_channel.h"

/*==================== 全局变量 ====================*/
Car2_Status_TypeDef Car2_Status;

/* 巡线参数 */
#define CAR2_TRACK_SPEED     50
#define CAR2_GO_SPEED        55
#define CAR2_TURN_SPEED       90
#define CAR2_TURN_TIME        670

/*==================== 内部函数声明 ====================*/
static void Car2_Process_Start(void);
static void Car2_Process_QR(void);
static void Car2_Process_RFID(void);
static void Car2_Process_3D(void);

/*==================== 初始化 ====================*/
/**
 * @brief 初始化车2模块
 */
void Car2_Init(void)
{
    /* 初始化状态 */
    Car2_Status.Work_Mode = 0;
    Car2_Status.Task_Status = 0;
    Car2_Status.Finish_Flag = 0;
    Car2_Status.Start_Flag = 0;
    Car2_Status.Barrier_State = 0;
    
    /* 清除数据缓冲区 */
    memset(Car2_Status.QR_Data, 0, sizeof(Car2_Status.QR_Data));
    memset(Car2_Status.RFID_Data, 0, sizeof(Car2_Status.RFID_Data));
    
    printf("Car2 Init OK!\r\n");
}

/**
 * @brief 初始化车2电机
 */
void Car2_Motor_Init(void)
{
    /* 使用主车的电机驱动模块 */
    /* 这里调用主车的电机初始化函数 */
}

/*==================== 任务处理 ====================*/
/**
 * @brief 车2任务处理主函数
 */
void Car2_Task(void)
{
    /* 检查ZigBee接收 */
    if (Zigbee_Rx_flag)
    {
        Car2_ParseCommand(Zigb_Rx_Buf);
        Zigbee_Rx_flag = 0;
        memset(Zigb_Rx_Buf, 0, sizeof(Zigb_Rx_Buf));
    }
    
    /* 根据工作模式处理任务 */
    switch (Car2_Status.Work_Mode)
    {
        case 0:  /* 空闲 */
            break;
            
        case 1:  /* 启动 */
            Car2_Process_Start();
            break;
            
        case 2:  /* 运行中 */
            /* 巡线任务 */
            Car2_Track(CAR2_TRACK_SPEED);
            break;
            
        default:
            Car2_Status.Work_Mode = 0;
            break;
    }
}

/**
 * @brief 解析来自车1的命令
 */
void Car2_ParseCommand(uint8_t* rxBuf)
{
    /* 验证帧头帧尾 */
    if (rxBuf[0] != FRAME_HEADER || rxBuf[7] != FRAME_TAIL)
        return;
    
    /* 验证目标地址 */
    if (rxBuf[1] != CAR2_ADDR)
        return;
    
    /* 解析命令 */
    uint8_t cmd1 = rxBuf[2];
    uint8_t cmd2 = rxBuf[3];
    
    switch (cmd1)
    {
        case CMD_CAR2_START:
            if (cmd2 == 0xA0)
            {
                /* 第一次启动 */
                Car2_Status.Start_Flag = 1;
                Car2_Status.Work_Mode = 1;
                printf("Car2 Start First!\r\n");
            }
            else if (cmd2 == 0xA1)
            {
                /* 第二次启动 */
                Car2_Status.Work_Mode = 2;
                printf("Car2 Start Second!\r\n");
            }
            /* 回复确认 */
            Car2_SendToCar1(0xAA, 0x00, 0x00, 0x00, 0x00);
            break;
            
        case CMD_CAR2_RFID:
            /* 接收RFID数据 */
            Car2_Status.RFID_Data[0] = rxBuf[3];
            Car2_Status.RFID_Data[1] = rxBuf[4];
            Car2_Status.RFID_Data[2] = rxBuf[5];
            printf("Car2 RFID: %02X %02X %02X\r\n", 
                   Car2_Status.RFID_Data[0], 
                   Car2_Status.RFID_Data[1], 
                   Car2_Status.RFID_Data[2]);
            break;
            
        case CMD_CAR2_QR_FRONT:
            /* 报警台前三位 */
            Car2_Status.QR_Data[0] = rxBuf[3];
            Car2_Status.QR_Data[1] = rxBuf[4];
            Car2_Status.QR_Data[2] = rxBuf[5];
            printf("Car2 QR Front: %02X %02X %02X\r\n",
                   Car2_Status.QR_Data[0],
                   Car2_Status.QR_Data[1],
                   Car2_Status.QR_Data[2]);
            break;
            
        case CMD_CAR2_QR_BACK:
            /* 报警台后三位 */
            Car2_Status.QR_Data[3] = rxBuf[3];
            Car2_Status.QR_Data[4] = rxBuf[4];
            Car2_Status.QR_Data[5] = rxBuf[5];
            printf("Car2 QR Back: %02X %02X %02X\r\n",
                   Car2_Status.QR_Data[3],
                   Car2_Status.QR_Data[4],
                   Car2_Status.QR_Data[5]);
            break;
            
        case CMD_CAR2_TFT_3D:
            /* 3D显示数据处理 */
            Car2_Process_3D();
            break;
            
        default:
            break;
    }
}

/*==================== 命令发送 ====================*/
/**
 * @brief 计算校验和
 */
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2)
{
    return (cmd1 + cmd2 + d1 + d2) % 256;
}

/**
 * @brief 发送数据给车1
 */
void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3)
{
    uint8_t cmd[8] = {
        FRAME_HEADER,           /* 帧头 */
        CAR1_ADDR,              /* 目标地址 = 车1 */
        cmd1,                   /* 主指令 */
        cmd2,                   /* 副指令 */
        d1, d2, d3,             /* 数据 */
        0x00,                   /* 校验和(待计算) */
        FRAME_TAIL              /* 帧尾 */
    };
    
    /* 计算校验和 */
    cmd[6] = Car2_GetChecksum(cmd[2], cmd[3], cmd[4], cmd[5]);
    
    /* 发送 */
    Send_ZigbeeData_To_Fifo(cmd, 8);
}

/**
 * @brief 发送二维码数据给车1
 */
void Car2_SendQRData(uint8_t* qrData)
{
    Car2_SendToCar1(CMD_CAR2_QR_SEND, 0x00, 
                    qrData[0], qrData[1], qrData[2]);
}

/**
 * @brief 发送任务完成标志给车1
 */
void Car2_SendFinish(void)
{
    Car2_SendToCar1(CMD_CAR2_FINISH, 0x00, 0x00, 0x00, 0x00);
    Car2_Status.Finish_Flag = 1;
}

/**
 * @brief 发送状态给车1
 */
void Car2_SendStatus(void)
{
    Car2_SendToCar1(CMD_CAR2_STATUS, 
                    Car2_Status.Work_Mode,
                    Car2_Status.Track_Sensor,
                    (uint8_t)(Car2_Status.Ultrasonic_Dist >> 8),
                    (uint8_t)(Car2_Status.Ultrasonic_Dist & 0xFF));
}

/*==================== 电机控制 ====================*/
/**
 * @brief 车2前进
 */
void Car2_Go(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Go(speed, time);
}

/**
 * @brief 车2后退
 */
void Car2_Back(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Back(speed, time);
}

/**
 * @brief 车2左转
 */
void Car2_TurnLeft(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_L90(speed, time);
}

/**
 * @brief 车2右转
 */
void Car2_TurnRight(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_R90(speed, time);
}

/**
 * @brief 车2停止
 */
void Car2_Stop(void)
{
    Motor_Data.xCAR_Go(0, 0);
}

/*==================== 巡线功能 ====================*/
/**
 * @brief 巡线函数
 */
void Car2_Track(uint8_t speed)
{
    Motor_Data.xCAR_Track(speed);
}

/**
 * @brief 按时间巡线
 */
void Car2_Track_Time(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Track_Time(speed, time);
}

/*==================== 内部处理函数 ====================*/
/**
 * @brief 处理启动命令
 */
static void Car2_Process_Start(void)
{
    /* 执行启动后的初始化 */
    printf("Car2 Processing Start...\r\n");
}

/**
 * @brief 处理二维码数据
 */
static void Car2_Process_QR(void)
{
    /* 发送二维码数据给车1 */
    Car2_SendQRData(Car2_Status.QR_Data);
}

/**
 * @brief 处理RFID数据
 */
static void Car2_Process_RFID(void)
{
    /* 处理RFID数据 */
    printf("Car2 RFID Data: %02X %02X %02X %02X\r\n",
           Car2_Status.RFID_Data[0],
           Car2_Status.RFID_Data[1],
           Car2_Status.RFID_Data[2],
           Car2_Status.RFID_Data[3]);
}

/**
 * @brief 处理3D显示数据
 */
static void Car2_Process_3D(void)
{
    /* 3D显示处理 */
    printf("Car2 3D Display Process\r\n");
}
