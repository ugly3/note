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
#define CAR2_TURN_SPEED      90
#define CAR2_TURN_TIME       670

/*==================== 内部函数声明 ====================*/
static void Car2_Process_Start(void);
static void Car2_Process_QR(void);
static void Car2_Process_RFID(void);
static void Car2_Process_3D(void);

/*==================== 初始化 ====================*/
void Car2_Init(void)
{
    Car2_Status.Work_Mode = 0;
    Car2_Status.Task_Status = 0;
    Car2_Status.Finish_Flag = 0;
    Car2_Status.Start_Flag = 0;
    Car2_Status.Barrier_State = 0;
    
    memset(Car2_Status.QR_Data, 0, sizeof(Car2_Status.QR_Data));
    memset(Car2_Status.RFID_Data, 0, sizeof(Car2_Status.RFID_Data));
    
    printf("Car2 Init OK!\r\n");
}

void Car2_Motor_Init(void)
{
}

/*==================== 任务处理 ====================*/
void Car2_Task(void)
{
    if (Zigbee_Rx_flag)
    {
        Car2_ParseCommand(Zigb_Rx_Buf);
        Zigbee_Rx_flag = 0;
        memset(Zigb_Rx_Buf, 0, sizeof(Zigb_Rx_Buf));
    }
    
    switch (Car2_Status.Work_Mode)
    {
        case 0:
            break;
        case 1:
            Car2_Process_Start();
            break;
        case 2:
            Car2_Track(CAR2_TRACK_SPEED);
            break;
        default:
            Car2_Status.Work_Mode = 0;
            break;
    }
}

void Car2_ParseCommand(uint8_t* rxBuf)
{
    if (rxBuf[0] != FRAME_HEADER || rxBuf[7] != FRAME_TAIL)
        return;
    
    if (rxBuf[1] != CAR2_ADDR)
        return;
    
    uint8_t cmd1 = rxBuf[2];
    uint8_t cmd2 = rxBuf[3];
    
    switch (cmd1)
    {
        case CMD_CAR2_START:
            if (cmd2 == 0xA0)
            {
                Car2_Status.Start_Flag = 1;
                Car2_Status.Work_Mode = 1;
                printf("Car2 Start First!\r\n");
            }
            else if (cmd2 == 0xA1)
            {
                Car2_Status.Work_Mode = 2;
                printf("Car2 Start Second!\r\n");
            }
            Car2_SendToCar1(0xAA, 0x00, 0x00, 0x00, 0x00);
            break;
            
        case CMD_CAR2_RFID:
            Car2_Status.RFID_Data[0] = rxBuf[3];
            Car2_Status.RFID_Data[1] = rxBuf[4];
            Car2_Status.RFID_Data[2] = rxBuf[5];
            printf("Car2 RFID: %02X %02X %02X\r\n", 
                   Car2_Status.RFID_Data[0], 
                   Car2_Status.RFID_Data[1], 
                   Car2_Status.RFID_Data[2]);
            break;
            
        case CMD_CAR2_QR_FRONT:
            Car2_Status.QR_Data[0] = rxBuf[3];
            Car2_Status.QR_Data[1] = rxBuf[4];
            Car2_Status.QR_Data[2] = rxBuf[5];
            printf("Car2 QR Front: %02X %02X %02X\r\n",
                   Car2_Status.QR_Data[0],
                   Car2_Status.QR_Data[1],
                   Car2_Status.QR_Data[2]);
            break;
            
        case CMD_CAR2_QR_BACK:
            Car2_Status.QR_Data[3] = rxBuf[3];
            Car2_Status.QR_Data[4] = rxBuf[4];
            Car2_Status.QR_Data[5] = rxBuf[5];
            printf("Car2 QR Back: %02X %02X %02X\r\n",
                   Car2_Status.QR_Data[3],
                   Car2_Status.QR_Data[4],
                   Car2_Status.QR_Data[5]);
            break;
            
        case CMD_CAR2_TFT_3D:
            Car2_Process_3D();
            break;
    }
}

/*==================== 命令发送 ====================*/
uint8_t Car2_GetChecksum(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2)
{
    return (cmd1 + cmd2 + d1 + d2) % 256;
}

void Car2_SendToCar1(uint8_t cmd1, uint8_t cmd2, uint8_t d1, uint8_t d2, uint8_t d3)
{
    uint8_t cmd[8];
    cmd[0] = FRAME_HEADER;
    cmd[1] = CAR1_ADDR;
    cmd[2] = cmd1;
    cmd[3] = cmd2;
    cmd[4] = d1;
    cmd[5] = d2;
    cmd[6] = 0x00;
    cmd[7] = FRAME_TAIL;
    
    cmd[6] = Car2_GetChecksum(cmd[2], cmd[3], cmd[4], cmd[5]);
    
    Send_ZigbeeData_To_Fifo(cmd, 8);
}

void Car2_SendQRData(uint8_t* qrData)
{
    Car2_SendToCar1(CMD_CAR2_QR_SEND, 0x00, qrData[0], qrData[1], qrData[2]);
}

void Car2_SendFinish(void)
{
    Car2_SendToCar1(CMD_CAR2_FINISH, 0x00, 0x00, 0x00, 0x00);
    Car2_Status.Finish_Flag = 1;
}

void Car2_SendStatus(void)
{
    Car2_SendToCar1(CMD_CAR2_STATUS, 
                    Car2_Status.Work_Mode,
                    Car2_Status.Track_Sensor,
                    (uint8_t)(Car2_Status.Ultrasonic_Dist >> 8),
                    (uint8_t)(Car2_Status.Ultrasonic_Dist & 0xFF));
}

/*==================== 电机控制 ====================*/
void Car2_Go(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Go(speed, time);
}

void Car2_Back(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Back(speed, time);
}

void Car2_TurnLeft(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_L90(speed, time);
}

void Car2_TurnRight(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_R90(speed, time);
}

void Car2_Stop(void)
{
    Motor_Data.xCAR_Go(0, 0);
}

/*==================== 巡线功能 ====================*/
void Car2_Track(uint8_t speed)
{
    Motor_Data.xCAR_Track(speed);
}

void Car2_Track_Time(uint8_t speed, uint16_t time)
{
    Motor_Data.xCAR_Track_Time(speed, time);
}

/*==================== 内部处理函数 ====================*/
static void Car2_Process_Start(void)
{
    printf("Car2 Processing Start...\r\n");
}

static void Car2_Process_QR(void)
{
    Car2_SendQRData(Car2_Status.QR_Data);
}

static void Car2_Process_RFID(void)
{
    printf("Car2 RFID Data: %02X %02X %02X %02X\r\n",
           Car2_Status.RFID_Data[0],
           Car2_Status.RFID_Data[1],
           Car2_Status.RFID_Data[2],
           Car2_Status.RFID_Data[3]);
}

static void Car2_Process_3D(void)
{
    printf("Car2 3D Display Process\r\n");
}
