#include "ALL.h"


uint8_t Buf[30];
int i,j=0;


//MPU6050
short Accel[3];//加速度
short Gyro [3];//角速度
long AccelData[3];//单位mg
long GyroData[3];//单位mdps



uint16_t temp_value =0;
uint16_t humi_value =0;


//硬件控制标志位，0代表关，1代表开
uint8_t led_status = 0;
uint8_t door_status = 0;
uint8_t Walkmotor_status = 0;
uint8_t Fan_status=0;
uint8_t HC_status = 0;
uint8_t Weight_status = 0;
uint8_t Body_Hw_ZD_status=0;
uint8_t Fan_ZD_status=0;

//定时器1  计次变量
uint32_t TimeCounter;

//定时器2 定时执行任务
uint32_t index_5ms_flag=0;
uint8_t index_10ms_flag=0;
uint8_t index_50ms_flag=0;
uint8_t index_200ms_flag=0;
uint8_t index_500ms_flag=0;
uint8_t index_800ms_flag=0;
uint8_t index_1000ms_flag=0;
uint8_t index_1300ms_flag=0;

//任务开始标志
uint8_t task1_is_start = 0;  
uint8_t task2_is_start = 0;  
uint8_t task3_is_start = 0; 
uint8_t task4_is_start = 0;  
uint8_t task5_is_start = 0;
uint8_t task6_is_start = 0;
uint8_t task7_is_start = 0; 
uint8_t task8_is_start = 0;
uint8_t task9_is_start = 0;
uint8_t task10_is_start = 0;  
uint8_t task11_is_start = 0;  
uint8_t task12_is_start = 0;
uint8_t task9_1_is_start=0;
uint8_t task9_2_is_start=0;
uint8_t task9_3_is_start=0;


uint8_t RFID_Stop=0;    //0代表检测RFID，1代表不检测RFID
uint8_t select_card=0;  //选择删除想要的RFID卡
uint8_t DLED_view_flag=0;        //数码管、点阵执行标志位，1代表关闭一些中断任务，防止数码管、点阵闪烁不稳定

//三个独立按键
int OLED_SELECT=0;        //当前的选择参数
int OLED_SELECT_flag=-1;   //上一次选择参数
uint8_t OLED_CONFIRM=0;       //确认键



//阈值
uint32_t LightData_yuzhi = 100;
uint32_t temp_yuzhi=29,humi_yuzhi=50;
uint32_t MQ2_yuzhi = 100,Huoyan_yuzhi = 150;

//超声波距离
uint32_t Distance;

//光照强度（整数，单位lx）
uint32_t LightData_Hex;

//称重/g
float Weight_Shiwu;

//存储RFID卡号ID
uint8_t UI0[4]={0xFF,0xFF,0xFF,0xFF};		 	//卡0 ID号
uint8_t UI1[4]={0xFF,0xFF,0xFF,0xFF};	 		//卡1 ID号
uint8_t UI2[4]={0xFF,0xFF,0xFF,0xFF};			//卡2 ID号
uint8_t UI3[4]={0xFF,0xFF,0xFF,0xFF};			//卡3 ID号

//三个ADC通道转换值
uint16_t adc_value[3];

