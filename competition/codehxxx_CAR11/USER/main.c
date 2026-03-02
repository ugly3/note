#include "all_module.h"

/*USER CODE BEGIN*/
#define Shield_A72 0	//A72屏蔽选择
#define Debug_USART1 0	//电脑串口调试选择
#define WIFI_Data_UpLoad 0	//WIFI上传数据选择
/*USER CODE END*/


/*****************全局静态变量**********************/
static uint32_t Power_check_times;          // 电量检测周期
static uint32_t RFID_Init_Check_times;      // RFID初始化检测时间周期
#if WIFI_Data_UpLoad
static uint32_t WIFI_Upload_data_times;     // 通过Wifi上传数据周期
#endif

/*****************全局变量**************************/
uint8_t RFID_Flag = 0;          	           // RFID检测标志位
uint8_t coordinate;                         // 随机坐标点
uint8_t run_state = 0;  //任务运行

/****************全局静态函数************************/
static void Hardware_Init(void);            // 硬件初始化函数


int main(void)
{
    Hardware_Init();                        // 硬件初始化
#if WIFI_Data_UpLoad     //将wifi上传数据先暂时屏蔽    
    WIFI_Upload_data_times = gt_get() + 200;
    uint16_t Light_Value = 0;               // 光强度值
    uint16_t CodedDisk_Value = 0;           // 码盘值   
    Principal_Tab[0] = 0x55;                // 主车数据上传指令包头
    Principal_Tab[1] = 0xAA;
    Follower_Tab[0] = 0x55;                 // 智能运输车数据上传指令包头
    Follower_Tab[1] = 0x02;
#endif	
	Power_check_times = gt_get() + 200;        //电量检测周期
	RFID_Init_Check_times = gt_get() + 200;    //RFID检测初始化周期
    Send_UpMotor(0, 0);
	
    while(1)
    {
			Mixture_Data.xCAR_KeyRun_Function();            //按键运行小车
			xAuto_Run_Function();			
        if(gt_get_sub(Power_check_times) == 0)          // 电池电量检测
        {
            Power_check_times = gt_get() + 200;
            Power_Check();
        }
#if 1
        if(gt_get_sub(RFID_Init_Check_times) == 0)      // RFID初始化检测
        {
            RFID_Init_Check_times =  gt_get() + 200;
            if(Rc522_GetLinkFlag() == 0)             //未能初始化，核心板的蜂鸣器间断鸣响
            {
                Readcard_daivce_Init();
                MP_SPK = !MP_SPK;
            }
            else                                     //RFID初始化通过，蜂鸣器关闭，LED1闪烁
            {
				MP_SPK = 0;   
				LED1 = !LED1;
				Rc522_LinkTest();
            }
        }
#endif
#if WIFI_Data_UpLoad    //wifi上传数据屏蔽
        if(gt_get_sub(WIFI_Upload_data_times) == 0)         // 数据上传
        {
            WIFI_Upload_data_times =  gt_get() + 500;
            if(Host_AGV_Return_Flag == RESET)               // 主车数据上传
            {
                Principal_Tab[2] = Stop_Flag;               // 运行状态
                Principal_Tab[3] = Get_tba_phsis_value();   // 光敏状态

                Ultrasonic_Ranging();                       // 超声波数据采集
                Principal_Tab[4] = dis % 256;               // 超声波数据低八位
                Principal_Tab[5] = dis / 256;               // 超声波数据高八位

                Light_Value = Get_Bh_Value();               // 光强度传感器数据采集
                Principal_Tab[6] = Light_Value % 256;       // 光强度数据低八位
                Principal_Tab[7] = Light_Value / 256;       // 光强度数据高八位

                CodedDisk_Value = CanHost_Mp;               // 码盘值
                Principal_Tab[8] = CodedDisk_Value % 256;
                Principal_Tab[9] = CodedDisk_Value / 256;

                Principal_Tab[10] = coordinate;             //返回随机救援坐标点
//              Send_WifiData_To_Fifo(Principal_Tab, 13);   // 通过Wifi上传主车数据
                UartA72_TxClear();
                UartA72_TxAddStr(Principal_Tab, 13);        // 通过串口上传主车数据
                UartA72_TxStart();
				coordinate = 0 ;
            }
            else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
            {

                UartA72_TxClear();
                UartA72_TxAddStr(Follower_Tab, 50);         // 通过串口上传从车数据
                UartA72_TxStart();
                Send_WifiData_To_Fifo(Follower_Tab, 50);    // 通过Wifi上传从车数据
                AGV_data_Falg = 0;
            }
        }
#endif
    }
}

/* 硬件初始化函数 */
//不需要用到
void Hardware_Init(void)
{
	uint8_t test_buf[8]={0};                            // 初始化发送数据
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     // 中断分组
    delay_init(168);                                    // 延时初始化
    Tba_Init();                                         // 任务板初始化
    Infrared_Init();                                    // 红外初始化
    Cba_Init();                                         // 核心板初始化
    Ultrasonic_Init();                                  // 超声波初始化
    Hard_Can_Init();                                    // CAN总线初始化
    BH1750_Configure();                                 // BH1750初始化配置
    BKRC_Voice_Init();									// 小创语音模块初始化
    Electricity_Init();                                 // 电量检测初始化
	
#if Shield_A72    //A72终端屏蔽
    UartA72_Init();                                     // A72硬件串口通讯初始化
#endif
	
    Can_check_Init(7, 83);                              // CAN总线定时器初始化
    roadway_check_TimInit(999, 167);                   	// 路况检测
    Timer_Init(999, 167);                               // 串行数据通讯时间帧
    Readcard_daivce_Init();                         	// RFID初始化
	
	/* Mycode BEGIN */
	Mixture_Data.xTIM3_Init();							// 定时器3初始化
	Mixture_Data.xTIM2_Init();                          // 定时器2初始化
	Ultrasonic_Ranging();                               // 先获取一次超声波（保证后面的准确度）
	Infrared_Send(test_buf,8);                          // 红外发送数据0
	Send_ZigbeeData_To_Fifo(test_buf,8);                // ZigBee发送数据0
	Send_WifiData_To_Fifo(test_buf,8);                  // WIFI发送数据0
	/* Mycode END*/
}











