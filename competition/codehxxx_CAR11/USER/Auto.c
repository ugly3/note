/**
************************************************************************
 *    文件名：all_module 
 *      说明：全自动运行（所用头文件+任务）
************************************************************************
**/

#include "all_module.h"
#define All_Test_1 0	
#define All_Test_2 0	
#define All_Test_3 0	
#define All_Test_4 0	
#define All_Test_5 0
#define YanShi     0
#define Android_Traffic_Light_Test   0   //安卓红绿灯测试
#define Two_Code_Test   0 //安卓二维码测试
#define Paging_Test     0 //安卓控制主车向下翻页测试
#define TFT_Licence_Test  0 // 安卓识别车牌测试
#define zhucong_shengsai_Test   0
#define light_daozha_tft_Test  0  //红绿灯车牌识别
#define di_liu_tao_Test   0  //样题第六套测试
#define di_san_tao_Test   0  //样题第三套测试
#define di_yi_lun    0  //第一轮比赛
#define di_er_lun    0  //第二轮比赛


#define HC 0
#define YT1 1
#define YT2 0
#define YT3 0
#define YT4 0
#define YT5 0



//uint8_t make = 0;                           // 全自动驾驶标志位

uint8_t  Go_Speed  = 50;                    // 全局行进速度值
uint8_t  wheel_Speed = 90;                  // 全局转弯速度值
uint8_t  RFID_Go_Speed  = 35;               // RFID路段的速度根据新车、旧车自行调节
uint16_t Go_Temp = 260;                     // 全局前进码盘值
uint16_t wheel_Time = 330;                  // 全局转45°时间
uint16_t distance = 0;                      // 记录超声波数据
uint8_t gear_init = 0;                      // 智能路灯初始档位
//static uint32_t task_times;                 // 任务时间检测时间周期


uint8_t MODE1 = 0;
uint8_t MODE2 = 0;
uint8_t MODE3 = 0;
uint8_t MODE4 = 0;

 
//  读RFID卡的速度设为30比较好

/*   
   //将数据打印至Debug区域
				uint8_t Buf[50];
				sprintf((char*)Buf,"data is : %x \r\n",XiaoChuang_Rx_Byte);  
				Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  
*/

uint8_t Run_State = 0; //运行位置状态

#if  All_Test_1
void xAuto_Run_Function(void)
{
	switch(Run_State)
	{
      case 1:   //主车启动
      {		
				
				Run_State = 2;				
				break;
      }
      case 2:   // 主车交通灯识别
      {

				Run_State = 3;
				break;
      }
      case 3:   //主车识别车牌
      {
				
			 Run_State = 4;
			 break;
      }
      case 4:   //主车测距
      {
           Run_State = 5;
         break;
      }
      case 5:   //主车识别二维码
      {  
           Run_State = 6;
         break;
      }
      case 6:   //主车通过道闸
      {
           Run_State = 7;
         break;
      }
      case 7:   //主车控制立体显示标志物显示
      {
           Run_State = 8;
         break;
      }
      case 8:   //主车获取RFID数据
      {
           Run_State = 9;
         break;
      }
      case 9:  //主车调光
      {
           Run_State = 10;
         break;
      }
      case 10:  //主车开启烽火报警
      {
				
				Run_State = 11;
				break;
      }
      case 11:  //主车经过特殊地形
      {
				
				Run_State = 12;
				break;
      }
      case 12:  //主车倒车入库
      {
				
				Run_State = 13;
				break;
      }
	}
}

#endif



#if  All_Test_2
void xAuto_Run_Function(void)
{
	switch(Run_State)
	{
      case 1:   
      {	
		Motor_Data.xCAR_Track_Go();
	    delay_ms(100);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
	    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		delay_ms(100);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
	    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		delay_ms(100);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
	    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Run_State = 2;
		break;
      }
      case 2:   
      {
	    ETC_Data.xETC_Pass();
		Run_State = 3;
		break;
      }
      case 3:  
      {
			
		 Run_State = 4;
		 break;
      }
	  default:break;
	}
}

#endif


#if  YanShi
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   //主车启动
      {
		LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x01);  //主车启动
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		Run_State = 2;
		break;
      }
      case 2:   // 主车测距
      {
		delay_ms(500);
		delay_ms(500);
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x02);  //开始测距	
		Ultrasonic_Ranging();
		delay_ms(500);
		LED_Display_Data.xLED_Display_Distance(123);   //将距离显示在 LED显示标志物里面
		delay_ms(500);
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);	
		delay_ms(500);
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2 + 30);
		delay_ms(500);
		Run_State = 3;
		break;
      }
      case 3:   //主车经过特殊地形
      {
		Special_LanForm_Data.xSpecial_LandForm_Pass();
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x03);   //通过特殊地形
		Motor_Data.xCAR_Track_Go();
		delay_ms(500);
		Run_State = 4;
		break;
      }
      case 4:   //主车调光
      {
			
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x04);
		delay_ms(500);
		Smart_Light_Data.xSmart_Light_Get_Init_Level();
		Smart_Light_Data.xSmart_Light_Appoint_Level(2); //将灯调节为2裆 
		delay_ms(700);				
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);	
		delay_ms(500);				
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);				
		delay_ms(500);				
		Run_State = 5;
		break;
      }
      case 5:   //主车语音交互识别
      {  
		 Motor_Data.xCAR_Track_Go();
		 delay_ms(500);
		 Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		 delay_ms(500);
		 XiaoChuang_Data.xSend_To_XiaoChuang_Rouse();
		 delay_ms(500);
		 Voice_Report_Data.xVoice_Report_Random_Command();
		 for(uint8_t i = 0; i < 6; i++)
		 {
			delay_ms(500);
			delay_ms(500);	
		 }
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);	
		 delay_ms(1000);
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);				
		 delay_ms(500);				
         Run_State = 6;
         break;
      }
      case 6:   //主车通过ETC系统
      {
		  Motor_Data.xCAR_Track_Time(60, 300);
		 ETC_Data.xETC_Pass();				
         Run_State = 7;
         break;
      }
      case 7:    //主车获取RFID数据
      { 
		 XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x05);
		 delay_ms(200);
		 RFID_Data.xRFID_Track_Read(15, 6);
		 delay_ms(500);
		 Motor_Data.xCAR_Track_Go();
         Run_State = 20;
         break;
      }
      case 8:       //主车避让
      {
		 XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x06);
		 delay_ms(200);
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);	
		 delay_ms(500);
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2 + 25);				
		 delay_ms(1000);
         Motor_Data.xCAR_Back(30, 740);	
		 delay_ms(1000);
         Run_State = 9;
         break;
      }
      case 9:   //主车让从车启动，并且到达B4时，开启左右双闪灯
      {
		 Mixture_Data.xTba_Both_Led(SET);    //开启双向灯
		 uint8_t fromcar_start[8] = {0x55,0x02,0xA0,0x00,0x00,0x00,0x00,0xBB};
		 XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x07);
		 delay_ms(200);
		 FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start);	
		 Run_State = 10;
		 break;
      }
      case 10:      //从车需要开启道闸
      {
		if(FollowCar_Data.FollowCar_Need_Barrier_Gate)  
		{
			FollowCar_Data.FollowCar_Need_Barrier_Gate = 0;
			Barrier_Data.xBarrier_Open_Close(Barrier_Data.Barrier_Gate_OPEN);
			Barrier_Data.xBarrier_Open_Close(Barrier_Data.Barrier_Gate_OPEN);
			uint8_t barrier_gate_start[8] = {0x55,0x02,0xA1,0x00,0x00,0x00,0x00,0xBB};
			FollowCar_Data.xSend_Command_TO_FollowCar(barrier_gate_start);
			FollowCar_Data.xSend_Command_TO_FollowCar(barrier_gate_start);
			Mixture_Data.xTba_Both_Led(RESET);    //关闭双向灯 
			Run_State = 11;			
		}
		break;
      }
      case 11:  
      { 				
		if(FollowCar_Data.FollowCar_Arrive_CarPort)    //从车到达车库，控制车库到3层
		{					
			FollowCar_Data.FollowCar_Arrive_CarPort = 0;
			CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 3);
			CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 3);
			Run_State = 12;			
		}
		break;
      }
      case 12:     //主车通过道闸
      {
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		delay_ms(200);
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x08);
		delay_ms(200);
		Barrier_Data.xBarrier_Open_Close(Barrier_Data.Barrier_Gate_OPEN);
		Motor_Data.xCAR_Track_Go();
		Run_State = 13;
		break;
      }
      case 13:     //主车开启烽火台
      {
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x09);
		SmokeTower_Data.SmokeTower_Infrared_Open();
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		Run_State = 14;
		break;
      }
      case 14:    //主车倒车入库
      {
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_Track_Time(25,700);   //倒车入库前先调整车的位置，保证车位于中间
		Motor_Data.xCAR_Back(20,1200);
		XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x10);
		Run_State = 15;
		break;
      }
	}
}







#endif



#if Android_Traffic_Light_Test
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
	    // 发送识别红绿灯识别
		Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);
		Run_State = 2;
		break;
      }
      case 2:   
      {
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);
			Run_State = 3;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			Run_State = 3;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			Run_State = 3;
		}	
		break;
      }
      case 3:  
      {
			
		 Run_State = 4;
		 break;
      }
	  default:break;
	}	
}

#endif 



#if Two_Code_Test
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
		uint8_t two_code_arr[] = {0x55,0xFF,0x02,0x00,0x00,0x00,0x00,0xBB};
		Android_Data.xMainCar_Send_Android(two_code_arr);   //发送请求识别二维码
		Run_State = 2;
		break;
      }
      case 2:   
      {
		if(Android_Data.Two_Code_State == 1)
		{
			uint8_t Buf[50];
			sprintf((char*)Buf,"data is %x  ",Two_Code_Data_Store);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  
			Run_State = 3;
		}
		break;
      }
      case 3:  
      {

		Run_State = 4;
		break;
      }
	  default:break;
	}	
}

#endif 



#if Paging_Test
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;
			if(Android_Data.TFT_Paging_Over == 1)
			{
				Android_Data.TFT_Paging_Over = 0;
				Run_State = 2;
				break;
			}
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}
		break;
      }
      case 2:   
      {
		uint8_t Buf[50];
		sprintf((char*)Buf,"successfully \r\n");  
		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  		
		break;
      }
	  default:break;
	}	
}

#endif 



#if TFT_Licence_Test
void xAuto_Run_Function(void)
{
#if 0
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
		uint8_t tft_license_arr[] = {0x55,0xFF,0x03,0x00,0x00,0x00,0x00,0xBB};
		Android_Data.xMainCar_Send_Android(tft_license_arr);   //发送请求识别TFT车牌
		Run_State = 2;
		break;
      }
      case 2:   
      {
		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		  
		if(Android_Data.Android_TFT_Licence_Flag == 1)   //识别成功后，打印车牌
		{
			Android_Data.Android_TFT_Licence_Flag = 0;
			uint8_t Buf[50];
			sprintf((char*)Buf,"license:%s \r\n",TFT_License_Data_Store);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  
			Run_State = 3;
		}			
		break;
      }
	  default:break;
	}	
#endif

		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		
}

#endif 



#if zhucong_shengsai_Test
void xAuto_Run_Function(void)
{
	switch(Run_State)
	{
      case 1:   //主车启动
      {
		LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2); 
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_Track_Go();   
		Run_State = 2;
		break;
      }
      case 2:   // 主车测距
      {
		delay_ms(200);
		Ultrasonic_Ranging();
		delay_ms(100);
		LED_Display_Data.xLED_Display_Distance(dis);   //将距离显示在 LED显示标志物里面
		delay_ms(100);
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);	
		delay_ms(100);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
		Run_State = 3;
		break;
      }
      case 3:   //主车经过特殊地形
      {
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);  
		Special_LanForm_Data.xSpecial_LandForm_Pass();
		Motor_Data.xCAR_Track_Time(45,350);
		delay_ms(200);
		Run_State = 4;
		break;
      }
      case 4:   //主车经过ETC
      {
		ETC_Data.xETC_Pass();
		delay_ms(100);
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);	
		Motor_Data.xCAR_Track_Go();
					
		Run_State = 5;
		break;
      }
      case 5:   //主车智能路灯感知
      {  
		 Smart_Light_Data.xSmart_Light_Get_Init_Level(); 
		 Smart_Light_Data.xSmart_Light_Appoint_Level(2);
		 Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2); 			
         Run_State = 6;
         break;
      }
      case 6:   //主车智能语音交互
      {
		 XiaoChuang_Data.xSend_To_XiaoChuang_Rouse();
		 delay_ms(200);
		 Voice_Report_Data.xVoice_Report_Random_Command();
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2); 
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
         Run_State = 7;
         break;
      }
      case 7:    //主车获取RFID数据（无线射频识别任务）
      { 
		 delay_ms(100);
		 RFID_Data.xRFID_Track_Read(30, 6);
		 Motor_Data.xCAR_Track_Go();
		 delay_ms(200);
		 RFID_Data.xRFID_Track_Read(30, 6);
		 Motor_Data.xCAR_Track_Go();
		 delay_ms(200);
		 
         Run_State = 8;
         break;
      }
      case 8:    //从车启动
      {
		  uint8_t fromcar[]={0x55,0x02,0xA0,0x00,0x00,0x00,0x00,0xBB};
		 FollowCar_Data.xStart_Command_To_FollowCar(fromcar);
		 delay_ms(200);
		  
		  uint8_t fromcar_dis[]={0x55,0x02,0xA2,0x01,0x02,0x03,0x00,0xBB};
		 FollowCar_Data.xSend_Command_TO_FollowCar(fromcar_dis);
		 delay_ms(200);
		  
		  
		 Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2); 
         Run_State = 9;
         break;
      }
      case 9:   //主车开启烽火台
      {
		 SmokeTower_Data.SmokeTower_Infrared_Open();
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2); 
		 Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2); 
		 Run_State = 10;
		 break;
      }
      case 10:      //主车开启道闸
      {
		Barrier_Data.xBarrier_Open_Close(Barrier_Data.Barrier_Gate_OPEN);
		delay_ms(500);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
		Motor_Data.xCAR_Track_Go(); 
	    Run_State = 11;
		break;
      }
      case 11:  
      { 
		Run_State = 12;
		break;
      }
      case 12:     //主车通过道闸
      {
		Run_State = 13;
		break;
      }
      case 13:     
      {
		Run_State = 14;
		break;
      }
      case 14:    //主车倒车入库
      {
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2+100);
		CarPort_Data.xCarPort_CarBack_Into(CarPort_Data.Device_A);
		Run_State = 15;
		break;
      }
	  case 15:
	  {
		  uint8_t fromcar[]={0x55,0x02,0xA0,0x00,0x00,0x00,0x00,0xBB};
		 FollowCar_Data.xStart_Command_To_FollowCar(fromcar);
		 delay_ms(200);
		  
		  uint8_t fromcar_dis[]={0x55,0x02,0xA2,0x05,0x00,0x00,0x00,0xBB};
		 FollowCar_Data.xSend_Command_TO_FollowCar(fromcar_dis);
		 delay_ms(200);
		  Run_State = 16;
		  break;
	  }
  }
}
#endif


#if light_daozha_tft_Test
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   //发送请求红绿灯识别   用的设备A
      {	
		Motor_Data.xCAR_Track_Go();
		delay_ms(500);
		Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);
		Run_State = 2;
		break;
      }
      case 2:   
      {	  
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			delay_ms(500);
			Run_State = 3;
		}
		break;
      }
	  case 3:
	  {
		Motor_Data.xCAR_Track_Go();
		delay_ms(500);
		Motor_Data.xCAR_Go(40,400);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		uint8_t tft_license_arr[] = {0x55,0xFF,0x03,0x00,0x00,0x00,0x00,0xBB};
		Android_Data.xMainCar_Send_Android(tft_license_arr);   //发送请求识别TFT车牌	
		Run_State = 4;
		break;
	  }
	  case 4:
	  {
		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		  
		if(Android_Data.TFT_Licence_Flag == 1)   //识别成功后，打印车牌
		{
			Android_Data.TFT_Licence_Flag = 0;
			uint8_t Buf[50];
			sprintf((char*)Buf,"license:%s \r\n",TFT_License_Data_Store);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			Motor_Data.xCAR_Back(40,400);
			Run_State = 20;
		}
		break;
	  }
	  case 5:
	  {
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Barrier_Data.xBarrier_Licence_Tx((char*)TFT_License_Data_Store);
		  delay_ms(500);
		  Motor_Data.xCAR_Track_Go();
		  Run_State = 6;
		  break;
	  }
	  case 6:
	  {
		  Motor_Data.xCAR_Track_Go();
		  RFID_Data.xRFID_Track_Read(30,57,1);
		  Motor_Data.xCAR_Go(40,300);
		  Run_State = 7;
		  break;
	  }
	  case 7:
	  {
		  RFID_Data.xRFID_Track_Read(30,57,2);	
		  Motor_Data.xCAR_Go(40,300);
		  Run_State = 8;
		  break;
	  }
	  case 8:
	  {
		  RFID_Data.xRFID_Track_Read(30,57,3);
		  Motor_Data.xCAR_Go(40,300);
		  Run_State = 9;
		  break;
	  }
	  default:break;
	}		
}

#endif 


#if di_liu_tao_Test
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
		LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		delay_ms(500);
		Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯
		Run_State = 2;
		break;
      }
      case 2:      //识别好红绿灯后发送数据给红绿灯暂停时间倒计时
      {
		if(Android_Data.Red_State == 1)   
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			delay_ms(500);
			Run_State = 3;
		}		  
		break;
      }
	  case 3:
	  {
		Motor_Data.xCAR_Track_Go();
		Ultrasonic_Ranging();   // 主车测距，变量为 dis
		LED_Display_Data.xLED_Display_Distance(dis);   //将距离显示在 LED显示标志物里面
		Run_State = 4;
		break;
	  }
	  case 4:
	  {
		Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码			
		Run_State = 5;
		break;
	  }
	  case 5:
	  {
		  if(Android_Data.Two_Code_State == 1)    //识别二维码成功
		  {
			Android_Data.Two_Code_State = 0;
			uint8_t buf[50];
			sprintf((char*)buf,"two_code data : %s \r\n",Two_Code_Data_Store);  
			Send_InfoData_To_Fifo((char*)buf,strlen((char*)buf)); 
			Run_State = 6;
		  }
		  break;
	  }
	  case 6:
	  {
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		  ETC_Data.xETC_Pass();    //通过ETC
		  Special_LanForm_Data.xSpecial_LandForm_Pass();   //通过特殊地形
		  Motor_Data.xCAR_Track_Time(35,300);
		  Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);   //发送请求识别TFT车牌
		  Run_State = 7;
		  break;
	  }
	  case 7:   // 车牌识别成功后
	  {
		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;   //TFT A设备
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		  
		if(Android_Data.TFT_Licence_Flag == 1)   //识别成功后，打印车牌
		{
			Android_Data.TFT_Licence_Flag = 0;
			uint8_t Buf[30];
			sprintf((char*)Buf,"license :%s \r\n",TFT_License_Data_Store);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			Motor_Data.xCAR_Back(35,350);
			Run_State = 8;
		}
		break;
	  }
	  case 8:
	  {
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_Track_Time(35,300);
		  Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr); //发送请求识别TFT交通标志
		  Run_State = 9;
		  break;
	  }
	  case 9:  
	  {
		if(Android_Data.Control_TFT_Paging == 1)   //翻到交通标志
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		  
		if(Android_Data.TFT_Traffic_Flag == 1)   //交通标志识别成功后
		{
			Android_Data.TFT_Traffic_Flag = 0;
			uint8_t Buf[30];
			sprintf((char*)Buf,"Traffic:%x \r\n",TFT_Traffic_Data_Store[0]);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			delay_ms(500);
			delay_ms(500);
			Android_Data.xMainCar_Send_Android(Identify_TFT_GraphCount_Arr); //发送请求识别TFT图形数量
			Run_State = 10;
		}	  
		  break;
	  }
	  case 10: //图形数量识别成功后
	  {
		if(Android_Data.Control_TFT_Paging == 1)   //翻到图形
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}			  
		if(Android_Data.TFT_GraphCount_Flag == 1)   //图形数量识别成功后
		{
			Android_Data.TFT_GraphCount_Flag = 0;
			uint8_t Buf[30];
			sprintf((char*)Buf,"graphcount :%x-%x-%x \r\n",TFT_GraphCount_Data_Store[0],
			TFT_GraphCount_Data_Store[1],
			TFT_GraphCount_Data_Store[2]);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(Smart_TFT_Data.Device_A,(char*)TFT_GraphCount_Data_Store);
			delay_ms(500);
			delay_ms(500);
			Android_Data.xMainCar_Send_Android(Identify_TFT_GraphColour_Arr); //发送请求识别TFT图形颜色
			delay_ms(500);
			delay_ms(500);
			Android_Data.xMainCar_Send_Android(Identify_TFT_GraphColour_Arr); //发送请求识别TFT图形颜色
			Run_State = 11;
		}	
		  break;
	  }
	  case 11:
	  {
		if(Android_Data.TFT_GraphColour_Flag == 1)   //图形颜色识别成功后
		{
			Android_Data.TFT_GraphColour_Flag = 0;
			uint8_t Buf[30];
			sprintf((char*)Buf,"graph colour is:%x-%x-%x \r\n",TFT_GraphColour_Data_Store[0],
				TFT_GraphColour_Data_Store[1],TFT_GraphColour_Data_Store[2]);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			LED_Display_Data.xLED_Display_Data(TFT_GraphColour_Data_Store[0], 
			TFT_GraphColour_Data_Store[1],
			TFT_GraphColour_Data_Store[2],
			2);   //将得到的图形颜色数据发送到LED显示的第二排
			Run_State = 12;
		}
		  break;
	  }
	  case 12:   //智能语音交互任务
	  { 
		  XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
		  Motor_Data.xCAR_Back(35,350);
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_Track_Time(35,300);
		  Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
		  for(uint8_t i = 0;i<12;i++)  // 延时3秒
		  {
			  delay_ms(500);
		  }
		  Motor_Data.xCAR_Back(35,300);
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Run_State = 13;
		  break;
	  }
	  case 13:
	  {
		  RFID_Data.xRFID_Track_Read(25,18,1);  //读取第一张卡 ，第五扇区第三数据块
		  uint8_t i = 0,j = 0,num[3],addr;
		  while(READ_RFID1[i] >= '0' && READ_RFID1[i] <= '9')
		  {
			 num[j++] =  READ_RFID1[i] - 0x30;
			 i++;
			  if(i == 6)
			  {
				  break;
			  }
		  }		  
		  Motor_Data.xCAR_Go(40,300);
		  addr = ((num[0] - 1)*4)+num[1];
		  RFID_Data.xRFID_Track_Read(25,addr,2);  //读取第二张卡
		  //Motor_Data.xCAR_Track_Go();
		  Run_State = 14;
		  break;
	  }
	  case 14:  //智能路灯感知
	  {	
		  uint8_t num = 0;
		  num = ((dis/60) * (dis/60))%4 + 1;
		  Smart_Light_Data.xSmart_Light_Appoint_Level(num);
		  Run_State = 15;
		  break;
	  }
	  case 15:   //通过道闸
	  { 
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  delay_ms(100);
		  Three_Dim_Display_Data.xThree_Dim_Display_Distance(TFT_Traffic_Data_Store[0]);
		  delay_ms(100);
		  Barrier_Data.xBarrier_Licence_Tx("AAA123");
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_Back(35,600);
		  Motor_Data.xCAR_Track_Time(35,750);
		  Motor_Data.xCAR_Back(35,350);
		  Motor_Data.xCAR_Track_Time(35,750);
		  Motor_Data.xCAR_Back(35,350);
		  Motor_Data.xCAR_Track_Time(35,750);
		  Motor_Data.xCAR_Back(35,500);
		  //从车启动
		  uint8_t fromcar_start[8] = {0x55,0x02,0xA0,0x00,0x00,0x00,0x00,0xBB};
		  FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start);
		  delay_ms(500);
		  
		  uint8_t fromcar_send[8] = {0x55,0x02,0xA1,0x01,0x00,0x00,0x00,0xBB};		  
		  FollowCar_Data.xSend_Command_TO_FollowCar(fromcar_send);
		  delay_ms(500);
		  
		  uint8_t fromcar_feng1[8] = {0x55,0x02,0xA2,0x03,0x05,0x14,0x00,0xBB};
		  FollowCar_Data.xSend_Command_TO_FollowCar(fromcar_feng1);
		  delay_ms(500);
		  uint8_t fromcar_feng2[8] = {0x55,0x02,0xA3,0x45,0xDE,0x92,0x00,0xBB};
		  FollowCar_Data.xSend_Command_TO_FollowCar(fromcar_feng2);
		  delay_ms(500);
		  Run_State = 16;
		  break;
	  }
	  case 16:
	  { 
		  if(FollowCar_Data.FollowCar_Finish_Flag == 1)   //从车完成了任务
		  {
			  LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeClose);
			  Run_State = 17;
		  }
		  break;
	  }
	  default:break;
	}	
}

#endif





#if di_san_tao_Test

uint8_t mask_data = 0;
void xAuto_Run_Function(void)
{
	if(Android_Data.Android_Main_Car_Start_Flag)
	{
		Android_Data.Android_Main_Car_Start_Flag = 0;
		Run_State = 1;	
	}
	switch(Run_State)
	{
      case 1:   
      {	
		LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
		delay_ms(200);
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_Track_Go();
		delay_ms(100);
		Ultrasonic_Ranging();   // 主车测距，变量为 dis
		delay_ms(100);
		LED_Display_Data.xLED_Display_Distance(dis);   //将距离显示在 LED显示标志物里面
		delay_ms(500);
        //Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码	
		Run_State = 2;
		break;
      }
      case 2:      
      {
//		  if(Android_Data.Two_Code_State == 1)    //识别二维码成功
//		  {
//			Android_Data.Two_Code_State = 0;
//			uint8_t buf[50];
//			sprintf((char*)buf,"two_code data : %s \r\n",Two_Code_Data_Store);  
//			Send_InfoData_To_Fifo((char*)buf,strlen((char*)buf));
//			delay_ms(500);
//			Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
//		    delay_ms(500);
//		    delay_ms(500);
//			Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯	
//		  }
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		delay_ms(500);
		delay_ms(500);
		Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯	
        Run_State = 3;		  
		break;
      }
	  case 3:
	  {
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);
			delay_ms(500);
			Run_State = 4;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			delay_ms(500);
			Run_State = 4;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			delay_ms(500);
			Run_State = 4;
		}
		break;
	  }
	  case 4:
	  {
		Motor_Data.xCAR_Track_Go();	
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Special_LanForm_Data.xSpecial_LandForm_Pass();   //通过特殊地形
		RFID_Data.xRFID_Track_Read(20,24,1);  //读取第一张卡 ，第五扇区第三数据块
		Motor_Data.xCAR_Go(40,300);
		delay_ms(200);
		RFID_Data.xRFID_Track_Read(20,7,2);  //读取第二张卡
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_Track_Time(30,300);
		delay_ms(500);
		Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);   //发送请求识别TFT车牌
		Run_State = 5;
		break;
	  }
	  case 5:  //车牌识别成功后
	  {
		if(Android_Data.Control_TFT_Paging == 1)
		{
			Android_Data.Control_TFT_Paging = 0;   //TFT A设备
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}		  
		if(Android_Data.TFT_Licence_Flag == 1)   //识别成功后，打印车牌
		{
			Android_Data.TFT_Licence_Flag = 0;
			uint8_t Buf[30];
			sprintf((char*)Buf,"license :%s \r\n",TFT_License_Data_Store);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
		    Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//发送请求识别TFT图形（数量+颜色）
			Run_State = 6;
		}		  
		break;
	  }
	  case 6:
	  {
		if(Android_Data.Control_TFT_Paging == 1)   //翻到图形
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}			  
		if(Android_Data.TFT_Graph_CountAndColour_Flag == 1)  //图形颜色+数量识别成功后
		{
			Android_Data.TFT_Graph_CountAndColour_Flag = 0;
			Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(Smart_TFT_Data.Device_A,(char*)TFT_GraphAndColour_Data_Store);
			delay_ms(500);
			Motor_Data.xCAR_Back(35,350);  //退出TFTA
			Run_State = 7;
		}
		  break;
	  }
	  case 7:   
	  {
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Motor_Data.xCAR_Track_Time(35,300);
		Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr); //发送请求识别TFT口罩行人	
		Run_State = 8;
		break;
	  }
	  case 8:
	  {
		if(Android_Data.Control_TFT_Paging == 1)   //翻到口罩行人
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}			  
		if(Android_Data.TFT_Mask_Flag == 1)  //TFT口罩行人识别成功后
		{
			Android_Data.TFT_Mask_Flag = 0;
			// 存储安卓发过来口罩的数量
			mask_data = (TFT_Mask_Data_Store[0]*10) + (TFT_Mask_Data_Store[1]*1);  //存储口罩行人数量
			uint8_t Buf[30];
			sprintf((char*)Buf,"mask count :%d \r\n",mask_data);  
			Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
			delay_ms(500);
			delay_ms(500);
		    Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr); //发送请求识别TFT交通标志	
			Run_State = 9;     
		}		  
		break;
	  }
	  case 9:  
	  {
		if(Android_Data.Control_TFT_Paging == 1)   //翻到交通标志
		{
			Android_Data.Control_TFT_Paging = 0;
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
		}			  
		if(Android_Data.TFT_Traffic_Flag == 1)  //TFT交通标志识别成功后
		{
			Android_Data.TFT_Traffic_Flag = 0;
			delay_ms(500);
			Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(Smart_TFT_Data.Device_A,(char*)TFT_Traffic_Data_Store);			
			Run_State = 10;
		}	
		break;
	  }
	  case 10: 
	  {
		  Motor_Data.xCAR_Back(35,350);  //退出TFTB
		  delay_ms(100);
		  XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
		  delay_ms(100);
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  delay_ms(100);
		  Motor_Data.xCAR_Track_Time(35,300);
		  Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
		  for(uint8_t i = 0;i<12;i++)  // 延时3秒
		  {
			  delay_ms(500);
		  }
		  Motor_Data.xCAR_Back(35,300);
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Communication_Data.ETC_Open_Flag = 0;
		  ETC_Data.xETC_Pass();    //通过ETC
		  delay_ms(100);
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		  uint8_t num = 0;
		  num = (mask_data^mask_data)%4 + 1;
		  Smart_Light_Data.xSmart_Light_Appoint_Level(num);	   //智能路灯
          delay_ms(500);
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Run_State = 11;
		  break;
	  }
	  case 11:
	  {
		  //从车启动
		  uint8_t fromcar_start[8] = {0x55,0x02,0xA0,0x00,0x00,0x00,0x00,0xBB};
		  FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start);
		  delay_ms(500);
		  
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  // 到达道闸
		  Barrier_Data.xBarrier_Licence_Tx((char*)TFT_License_Data_Store);
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2+100);
		  
		  // 发送从车数据
		  uint8_t dat1,dat2,diss = 125;
		  
		  dat1 = diss/10/10;
		  dat2 = diss/10%10;
		  uint8_t fromcar_data[8] = {0x55,0x02,0xA4,dat1,dat2,0x00,0x00,0xBB};
		  FollowCar_Data.xStart_Command_To_FollowCar(fromcar_data);		  
		  
		  //倒车入库
		  CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,1);  //让车库先回到一层
	      CarPort_Data.xCarPort_CarBack_Into(CarPort_Data.Device_A); //入库
		  CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,3);
		  for(uint8_t i=0;i<9;i++)     //停车后报警10次
		  {			
			 MP_SPK = 1;             //打开蜂鸣器
			 delay_ms(100); 
			 MP_SPK = 0;             //关闭蜂鸣器
			 delay_ms(100);
		  }		  
		  Run_State = 12;
		  break;
	  }
	  case 12:  
	  { 
		  if(FollowCar_Data.FollowCar_Finish_Flag == 1)   //从车完成了任务
		  {
			  LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeClose);
			  Run_State = 13;
		  }
		  break;
	  }
	  case 13:
	  {

		  Run_State = 14;
		  break;
	  }
	  case 14:  
	  {	
		  uint8_t i = 0,j = 0,k=0,num1[3],num2[2],data;   // RFID获取公式数据算法
		  for(i = 0; i<8;i++)
		  {
			  if(READ_RFID1[i] >= '0' && READ_RFID1[i] <= '9')
			  {
				 num1[j++] =  READ_RFID1[i] - 0x30;
			  }
			  if(READ_RFID1[i] == '*'||READ_RFID1[i] == '/'||READ_RFID1[i] == '+'||READ_RFID1[i] == '-') 
			  {
				 num2[k++] =  READ_RFID1[i];
			  }
		  }
		  for(i = 0; i<2;i++)
		  {
			  if(i == 0)
			  {
				  if(num2[i] == '*')
				  {
					 data =  num1[1] * num1[2];
				  }
				  else if(num2[i] == '/')
				  {
					 data =  num1[1] / num1[2]; 
				  }
				  else if(num2[i] == '+')
				  {
					 data =  num1[1] + num1[2]; 
				  }
				  else if(num2[i] == '-')
				  {
					 data =  num1[1] - num1[2]; 
				  }
		      }
			  if(i == 1)
			  {
				  if(num2[i] == '*')
				  {
					 data =  data * num1[2];
				  }
				  else if(num2[i] == '/')
				  {
					 data =  data / num1[2]; 
				  }
				  else if(num2[i] == '+')
				  {
					 data =  data + num1[2]; 
				  }
				  else if(num2[i] == '-')
				  {
					 data =  data - num1[2]; 
				  }				  
			  }
		  }
	  
		  Run_State = 15;
		  break;
	  }
	  default:break;
	}	
}

#endif


     
#if di_yi_lun

 uint8_t timeout = 0;
 //Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]  温度数据

uint8_t mask_data = 0;
void xAuto_Run_Function(void)
{
//	if(Android_Data.Android_Main_Car_Start_Flag)
//	{
//		Android_Data.Android_Main_Car_Start_Flag = 0;
//		Run_State = 1;	
//	}
	switch(Run_State)                         
	{
      case 1:
      {	
		LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
		CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 1);
		delay_ms(200);
        //D7->D6
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
        //D6->F6
		ETC_Data.xETC_Pass();    //通过ETC  
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		// 智能语音交互
		XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
	    Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
	   for(uint8_t i = 0;i<6;i++)  // 延时6秒
	   {
		  delay_ms(500);
		  delay_ms(500);
	   }
	   Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
	   delay_ms(300);
	   Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
	   delay_ms(100);
	   Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
	   delay_ms(100);
       //F6->F5
	   Motor_Data.xCAR_Track_Time(40,400);  
	   Motor_Data.xCAR_Track_Time(40,400);  // 走到静态标志来测距
	   Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);  // 面向静态标志来测距  
	   //Ultrasonic_Ranging();   // 主车测距
	   delay_ms(100);
	   LED_Display_Data.xLED_Display_Distance(185);   //将距离显示在LED显示标志物第二行
        //识别二维码获取6位红外开启码，并将其发送给从车
       delay_ms(100);
       Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
       delay_ms(100);
       //F5->F4
       Motor_Data.xCAR_Track_Go();

       delay_ms(200);
       Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2); 
       delay_ms(600);  
       delay_ms(600);       
	   Run_State = 2;
		break;
      }
      case 2:      
      {
//		for(uint8_t i = 0; i<8; i++)  //翻页8次，要识别含有戴口罩的行人数量，识别含有RFID的图片获取A密匙
//		{
//           //摄像头识别行人戴口罩的数量和文字识别，识别到RFID图
//			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_B,2);  // 向下翻页
//			delay_ms(500);
//			delay_ms(500);
//		}
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
        delay_ms(200);
        //F4->F2
        Roadway_Flag_clean();//清除所以标志位
		RFID_Data.xRFID_Track_Read(25,22);   //寻卡
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
        delay_ms(200);
        Roadway_Flag_clean();//清除所以标志位
        
		//Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//发送请求识别TFT图形中的指定车型与车牌的车牌信息（数量+颜色发送给多功能显示B）
		Run_State = 3;	
		break;
      }
	  case 3:
	  {
		for(uint8_t i = 0; i<8; i++)  //翻页8次
		{
			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
			delay_ms(700);
			delay_ms(700);
			delay_ms(200);
		}
          
//		if(Android_Data.Control_TFT_Paging == 1)   //翻到图形
//		{
//			Android_Data.Control_TFT_Paging = 0;
//			Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
//		}			  
//		if(Android_Data.TFT_Graph_CountAndColour_Flag == 1)  //图形颜色+数量识别成功后
//		{
//			Android_Data.TFT_Graph_CountAndColour_Flag = 0;
//			Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(Smart_TFT_Data.Device_B,(char*)Identify_TFT_Graph_CountAndColour_Arr);
//			delay_ms(500);
//			Motor_Data.xCAR_Back(35,350);  //退出TFTA
//			Run_State = 4;
//		}
		Run_State = 4;
		break;
	  }
	  case 4:
	  { 
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		delay_ms(100);
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);

        //F2->D2
	    Special_LanForm_Data.xSpecial_LandForm_Pass();   //通过特殊地形
        delay_ms(200);
        Roadway_Flag_clean();//清除所有标志位
        RFID_Data.xRFID_Track_Read(25,22);  //寻卡
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
        delay_ms(200);  
        Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止重复识别十字路口的卡片
        Roadway_Flag_clean();//清除所有标志位
        delay_ms(500); 
        //D2->D4
		Barrier_Data.xBarrier_Licence_Tx("B8542D");  //开启道闸
        Roadway_Flag_clean();//清除所有标志位
		RFID_Data.xRFID_Track_Read(25,22);  //寻卡
        Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止重复识别十字路口的卡片
        Roadway_Flag_clean();//清除所以标志位        
        delay_ms(300); 
		// 从车第一次启动
		for(uint8_t i = 0; i<3; i++)
		{   
			FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start1);
			delay_ms(100);
		}
        //D4->D6
        Roadway_Flag_clean();//清除所以标志位
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
        //D6->B6
		Motor_Data.xCAR_Track_Go();
		Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
		Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B);  //发送请求识别红绿灯
		Run_State = 5; 
		break;
	  }
	  case 5:  
	  {
		  // 安卓识别交通灯
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,1);//发送给交通灯标志物请求确认
			delay_ms(500);
			Run_State = 6;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
			delay_ms(500);
			Run_State = 6;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
			delay_ms(500);
			Run_State = 6;
		}
        if(timeout >= 14)//蒙一个
		{
			delay_ms(500);
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
			Run_State = 6;
		}
       timeout++;
		break;
	  }
	  case 6:
	  {
          //B6->B4
		  Motor_Data.xCAR_Track_Go();
          //B4->B2
		  Motor_Data.xCAR_Track_Go();
		  Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		  Smart_Light_Data.xSmart_Light_Appoint_Level(3);	//智能路灯
		  Run_State = 7;
		  break;
	  }
	  case 7:   
	  {
		Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
		 // B2->B1
		Motor_Data.xCAR_Back(20,500);
        delay_ms(300); //延时
        xCAR_Track_Time(20,800);
        delay_ms(300); //延时
        Motor_Data.xCAR_Back(20,700);
        Motor_Data.xCAR_Back(20,700);
        delay_ms(500); //延时 
		Run_State = 8;
		break;
	  }
	  case 8:
	  {
		// 从车第二次启动
		uint8_t fromcar_start2[8] = {0x55,0x02,0xA0,0xA1,0x00,0x00,0x00,0xBB};
		for(uint8_t i = 0; i<3; i++)
		{   
			FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start2);
			delay_ms(100);
		}		
		Run_State = 9;  
		break;
	  }
	  case 9:  
	  {
//		  if(FollowCar_Data.FollowCar_Finish_Flag == 1)   //从车完成了任务
//		  {
//			  FollowCar_Data.FollowCar_Finish_Flag  =  0;
//			  Run_State = 10;
//		  }
           Run_State = 10;
		  break;
	  }
	  case 10: 
	  {
		  Run_State = 11;
		  break;
	  }
	  default:
            break;
	}	
}

#endif

#if di_er_lun
    void xAuto_Run_Function(void)
    {
        switch(Run_State)                         
	    {
          case 1:
          {	
                RFID_Data.xRFID_Track_Read(25,22);
                RFID_Data.xRFID_Track_Read(25,22);//十字路口有卡时读卡且不循迹，单纯的十字路口则再向前走一点，使车身中央对准十字路口
                Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
                Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止重复识别十字路口的卡片

                RFID_Data.xRFID_Track_Read(25,22);//继续沿直线寻找RFID卡
                RFID_Data.xRFID_Track_Read(25,22);//十字路口有卡时读卡且不循迹，单纯的十字路口则再向前走一点
                Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
                Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止重复识别十字路口的卡片
                RFID_Data.xRFID_Track_Read(25,22);
                Roadway_Flag_clean();//清除所以标志位

                Run_State=3;
                break;
          }
          case 2:
          {
              xCAR_Track_Go();
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
                Special_LanForm_Data.xSpecial_LandForm_Pass();   //通过特殊地形
//                RFID_Data.xRFID_Track_Read(25,22);  //寻到白卡
//                Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
//                Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止重复识别十字路口的卡片
//                Roadway_Flag_clean();//清除所以标志位
//                Barrier_Data.xBarrier_Licence_Tx("B8542D");  //开启道闸
                Run_State=3;
              break;
          }
          case 3:
          {
                Run_State=4;
                break;
          }
          case 4:  
          {	
                Run_State = 5;
                break;
          }	
          case 5:  
          {	
                Run_State = 6;
                break;
          }
          default:break;
              
        }
    }
#endif



#if HC
uint8_t mask_data = 0;
uint8_t timeout = 0;
uint8_t Buf[30];
void xAuto_Run_Function(void)
{
    //Ultrasonic_Ranging();
//    if(Android_Data.Control_TFT_Paging == 1)
//    {
//        Run_State=1;
//        Android_Data.Control_TFT_Paging = 0;
//    }
    switch(Run_State)                         
	{
          case 1:
          {
              
   //************************************************* 识别口罩数量 *******************************************************************
/*             
                Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr);
                delay_ms(500);
                if(Android_Data.TFT_Mask_Flag == 1)  //TFT口罩行人识别成功后
                {
                        // 存储安卓发过来口罩的数量
//                        mask_data = (TFT_Mask_Data_Store[0]*10) + (TFT_Mask_Data_Store[1]*1);  //存储口罩行人数量
//                        sprintf((char*)Buf,"mask count :%d \r\n",mask_data);  
//                        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
                    Android_Data.TFT_Mask_Flag = 0;      
                }
                              
*/
              
   //**************************************************  交通灯识别 *********************************************************************                      
/*         Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯 
         for(uint8_t i =0;i<3;i++)
         {
             delay_ms(500);
             delay_ms(500);
         }              
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			delay_ms(500);
			Run_State = 3;
		}
*/        
 
  //*********************************语音播报当前获取到的温度***********************************
/*              XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
              delay_ms(500);
              Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
              delay_ms(500);   
              XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x01);//播报“当前温度为”
              delay_ms(200);
              XiaoChuang_Data.xSend_Command_To_XiaoChuang((Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]/10)+'0');//播报十位数
              delay_ms(200);
              XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x02);//播报”十“
              delay_ms(200);
              XiaoChuang_Data.xSend_Command_To_XiaoChuang((Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]%10)+'0');//播报个位数
              delay_ms(200);
              XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x03);//播报”度“
              
              
//              mask_data=0x16;
//              XiaoChuang_Data.xSend_Command_To_XiaoChuang(1);//当前温度为
//              delay_ms(200);
//              XiaoChuang_Data.xSend_Command_To_XiaoChuang((mask_data>>4)+'0');//播报十位数
//              delay_ms(200);      
//              XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x02);//十
//              delay_ms(200);
//              XiaoChuang_Data.xSend_Command_To_XiaoChuang((mask_data&0x0F)+'0');//播报个位数
//              delay_ms(200);
//              XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x03);//度
*/

    //********************************************* 车牌识别 ******************************************
/*
                Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
                for(uint8_t i =0;i<3;i++)
                {
                    delay_ms(500);
                    delay_ms(500);
                }
                Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);   //发送请求识别TFT车牌
                for(uint8_t i =0;i<3;i++)
                {
                    delay_ms(500);
                    delay_ms(500);
                }
//            if(Android_Data.Control_TFT_Paging == 1)
//            {
//                Android_Data.Control_TFT_Paging = 0;
               
//            }		  
            if(Android_Data.TFT_Licence_Flag == 1)   //识别成功后，打印车牌
            {
                Android_Data.TFT_Licence_Flag = 0;
                uint8_t Buf[50];
                sprintf((char*)Buf,"%.6s ",TFT_License_Data_Store);  
                Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf)); 

                Run_State = 3;
            }		
*/

    //************************************** 识别二维码 ***********************************************
/*              Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);
              for(uint8_t i =0;i<2;i++)
                {
                    delay_ms(500);
                    delay_ms(500);
                }
              if(Android_Data.Two_Code_State == 1)  //TFT口罩行人识别成功后
              {
                        // 打印安卓发过来二维码的信息
                    mask_data = (Two_Code_Data_Store[0]*10) + (Two_Code_Data_Store[1]*1);  
                    sprintf((char*)Buf,"Two_Code :%d \r\n",mask_data);  
                    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
                    Android_Data.Two_Code_State = 0;      
              }
*/    

    //************************************** 交通标志识别 ***********************************************
/*
//                Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
              for(uint8_t i =0;i<3;i++)
              {
                   delay_ms(500);
                   delay_ms(500);
              }
              Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr);
              for(uint8_t i =0;i<2;i++)
              {
                  delay_ms(500);
                  delay_ms(500);
              }
              if(Android_Data.TFT_Traffic_Flag == 1)  
              {
                        // 打印安卓发过来交通标识的信息
                    sprintf((char*)Buf,"Traffic:%02x \r\n",TFT_Traffic_Data_Store[0]);  
                    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
                    Android_Data.TFT_Traffic_Flag = 0;      
              }
*/

/*
                Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
              for(uint8_t i =0;i<3;i++)
              {
                   delay_ms(500);
                   delay_ms(500);
              }
              Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);
              for(uint8_t i =0;i<2;i++)
              {
                  delay_ms(500);
                  delay_ms(500);
              }
              if(Android_Data.TFT_Graph_CountAndColour_Flag == 1)  
              {
                        // 打印安卓发过来二维码的信息
                    sprintf((char*)Buf,"%.6s \r\n",TFT_GraphAndColour_Data_Store);  
                    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
                    Android_Data.TFT_Graph_CountAndColour_Flag = 0;      
              }

*/
            
//          RFID_Data.xRFID_Write(17);
//          delay_ms(500);
//            RFID_Data.xRFID_Read(17);
//            delay_ms(500);
//            
//           YT2_Handle_Card1_Data(READ_RFID1);

//        Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
//        for(uint8_t i=0;i<2;i++)
//        {
//            delay_ms(500);
//            delay_ms(500);
//        }

//        Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);

//        Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);
//        delay_ms(500);
//        timeout=10;
//        while(timeout)
//        {
//            if(Android_Data.TFT_Graph_CountAndColour_Flag==1)
//            {
//                Android_Data.TFT_Graph_CountAndColour_Flag=0;
//                timeout=0;
//                break;
//            }
//            else
//            {
//                if(jixun!=1)
//                {
//                    Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
//                    for(uint8_t i=0;i<2;i++)
//                    {
//                        delay_ms(500);
//                        delay_ms(500);
//                    }
//                    timeout--;
//                }
//                else 
//                    jixun=0;
//                
//                Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);
//                delay_ms(500);
//            }
//        }
        
        
        
/*
        Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
        delay_ms(500);
        timeout=10;
        while(timeout)
        {
            if(Android_Data.TFT_CheXin_sucess_flag==1)
            {
                Android_Data.TFT_CheXin_sucess_flag=0;
                timeout=0;
                break;
            }
            else
            {
                if(jixun!=1)
                {
                    Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
                    for(uint8_t i=0;i<2;i++)
                    {
                        delay_ms(500);
                        delay_ms(500);
                    }
                    timeout--;
                }
                else 
                    jixun=0;
                
                Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
                delay_ms(500);
            }
        }
        
        Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr);//下一组不同类型的图片(车牌)
        delay_ms(500);
        timeout=10;
        while(timeout)
        {
            if(Android_Data.TFT_ChePai_sucess_flag==1)
            {
                Android_Data.TFT_ChePai_sucess_flag=0;
                timeout=0;
                break;
            }
            else
            {
                if(jixun!=1)//包头没有出错，继续翻页
                {
                    Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
                    for(uint8_t i=0;i<2;i++)
                    {
                        delay_ms(500);
                        delay_ms(500);
                    }
                    timeout--;
                }
                else 
                    jixun=0;
                
                Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr);
                delay_ms(500);
            }
        }
        
  */      
        

        
//        sprintf((char*)Buf,"%.6s \r\n",TFT_GraphAndColour_Data_Store);  
//        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));


            Run_State=0;
                break;
           }
          default:break;
        }
                  
          
}

#endif


#if  YT1 

#define Card1_Block  14                    //填写卡1地址块
#define Card2_Block  card2_sector_block    //卡2地址块
#define Card2_Place  card2_position        //卡2的坐标

uint8_t timeout = 0;
uint8_t Get_Num_Cheku;
uint8_t Get_Num_LuDeng;
void xAuto_Run_Function(void)
{
    switch(Run_State)                         
	{
      case 1:
      {	
          LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
          LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200);
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
          for(uint8_t i=0;i<3;i++) //等待安卓回传
          {
              delay_ms(500);
              delay_ms(500);
          }
          
          Run_State=2;
          break;
      }
      case 2:
      {
           // 安卓识别交通灯
		if(Android_Data.Red_State == 1)
		{
			Android_Data.Red_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Yellow_State == 1)
		{
			Android_Data.Yellow_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
			delay_ms(500);
			Run_State = 3;
		}
		else if(Android_Data.Green_State == 1)
		{
			Android_Data.Green_State = 0;
			Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
			delay_ms(500);
			Run_State = 3;
		}
        else//蒙一个
        {
            Android_Data.Green_State = 0;
            Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
            delay_ms(500);
            Run_State = 3;
        }
        
        Run_State = 3;
          break;
      }
      case 3:
      {
          //D6->B6
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          //B6->B4
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          Get_Num_LuDeng = Smart_Light_Data.xSmart_Light_Get_Init_Level();         //获得路灯初始挡位
          
          Get_Num_Cheku = CarPort_Data.xCarPort_Ack_State(CarPort_Data.Device_A);//获得车库A的初始层数
          Get_Num_Cheku = CarPort_Data.xCarPort_Ack_State(CarPort_Data.Device_A);

          delay_ms(200);
          Smart_Light_Data.xSmart_Light_Appoint_Level(Get_Num_Cheku);            //调节路灯到相应的挡位
          
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          
          Run_State=4;
          break;
      }
      case 4:
      {
          //B4->D4
            RFID_Data.xRFID_Track_Read_L(25,450,Card1_Block);//对ETC前面一小段进行识卡
            ETC_Data.xETC_Pass_RFID(Card1_Block,0,0);
            Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止可能重复识别十字路口的卡片
          //D4->F4
            //对特殊标志物前面一小段进行识卡
            RFID_Data.xRFID_Track_Read(25,Card1_Block,0,0);//识别到特殊路段停
//            delay_ms(200);
//            RFID_Data.xRFID_Read(Card1_Block);
//            delay_ms(200);  
//            Motor_Data.xCAR_Go(40,200);
//            RFID_Data.xRFID_Read(Card1_Block);
//            delay_ms(200);

            //过特殊标志物
            Motor_Data.xCAR_Go(25,500);
            Motor_Data.xCAR_Go(25,300);
            delay_ms(500);
            //特殊标志物后一小段进行寻卡
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡 
            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
            delay_ms(200); 
            YT1_Handle_Card1_Data(READ_RFID1);//************************************解析处理卡1内的信息，得到卡2的位置和读取的地址块
          
          Run_State=5;
          break;
      }
      case 5:
      {
          //安卓识别图片（一个交通标志和一个图形），回传图形种类的个数和图形颜色的个数
            Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr); 
            for(uint8_t i=0;i<5;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
                Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
            }
            for(uint8_t i=0;i<3;i++)//发送三次
            {
                Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,"A1D2E3");//多功能信息显示标志物B显示A1D2E3（还不行）
                delay_ms(200);
                LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
                delay_ms(200);
            }
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转
          Run_State=6;
          break;
      }
      case 6:
      {
          //F4->F2
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);//右转
            
            //安卓识别车型和行人口罩数量
            Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr);//主车发给安卓请求识别TFT口罩行人
            for(uint8_t i=0;i<8;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
            }
            Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,"FF0205");//多功能信息显示标志物A显示FF0105（还不行）
            delay_ms(200);
            
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转
            delay_ms(200);
          
          Run_State=7;
          break;
      }
      case 7:
      {
          //公交站
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
            for(uint8_t i = 0;i<8;i++)  // 延时8秒等待小创识别并重复播报
            {
                delay_ms(500);
                delay_ms(500);
            }
            Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
            Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
            FollowCar_Data.XSend_temperature_To_fromcar();
//            Voice_Report_Data.xVoice_Report_Speak_Weather();//播报天气状况
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转
 
           //F2->D2
            Motor_Data.xCAR_Track_Go();
            //启动从车
//            for(uint8_t i = 0; i<3; i++)//启动从车
//            {   
//              FollowCar_Data.xStart_Command_To_FollowCar();
//              delay_ms(100);
//            }        
          
          Run_State=8;
          break;
      }
      case 8:
      {
          if(strcmp(Card2_Place,"D1")==0)
          {    
              //D2->D1->D2
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);//右转
            RFID_Data.xRFID_Track_Read_L(25,1100,Card2_Block);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          }
           else if(strcmp(Card2_Place,"D3")==0)
          {   
            //D2->D3->D2
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转  
            RFID_Data.xRFID_Track_Read_L(25,1400,Card2_Block);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          }
              Run_State=9;
          break;
      }
      case 9:
      {
          //D2->B2
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            
//            if(FollowCar_Data.Follow_Send_Licence_Data_Flag == 1)
//            {
//                FollowCar_Data.Follow_Send_Licence_Data_Flag = 0;
//            sprintf((char*)Buf,"%.6s\r\n",TFT_GraphAndColour_Data_Store); 
//            Barrier_Data.xBarrier_Licence_Tx((char*)Buf);//车牌
//            }
            Barrier_Data.xBarrier_Licence_Tx("B8542D");//车牌
            delay_ms(300);
            //B2->B4
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_Go(25,250);//走开十字路口，防止重复识别十字路口的卡片
            
          Run_State=10;
          break;
      }
      case 10:
      {
            //B4->B6
            RFID_Data.xRFID_Track_Read(27,Card1_Block,Card2_Block,0);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(200);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(200);
            Motor_Data.xCAR_Go(20,150);
            delay_ms(200);
          Run_State=11;
          break;
      }
      case 11:
      {
          
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x01);//
//            for(uint8_t i=0;i<2;i++)//等待降到一层
//            {
//                delay_ms(500);
//                delay_ms(500);
//            }
            //B6->A6(倒车)
            Motor_Data.xCAR_Track_Time(20,1300);
            delay_ms(300); //延时
            Motor_Data.xCAR_Back(20,850);//短
            Motor_Data.xCAR_Back(20,800);//短
            
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x02);
            delay_ms(500); //延时 
          Run_State=12;
          break;
      }
      
      default:break;           
    }

}

#endif


#if YT2

#define Card1_Block  17                    //填写卡1地址块
#define Card2_Block  2                     //****************卡2地址块需修改为0~2其中一个数，具体看安卓的识别结果
#define Card3_Block  0                     //卡3地址块

#define Card2_Place  card2_position        //卡2的坐标

uint8_t timeout = 0;

void xAuto_Run_Function(void)
{
    switch(Run_State)                         
	{
      case 1:
      {	
            LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(300);
            //F7→F6
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          
            //公交站随机播报
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
            for(uint8_t i = 0;i<8;i++)  // 延时8秒等待小创识别并重复播报
            {
                delay_ms(500);
                delay_ms(500);
            }
            Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
            Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
//            Voice_Report_Data.xVoice_Report_Speak_Weather();//播报天气状况
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(200);
          
            //F6→F4
            ETC_Data.xETC_Pass();//ETC位于中间（无寻卡）
            delay_ms(300);
            
          Run_State=2;
          break;
      }
      case 2:
      {
            //F4→F2
            Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
            for(uint8_t i=0;i<3;i++) //等待安卓回传
            {
              delay_ms(500);
              delay_ms(500);
            }
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
                delay_ms(500);
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
                delay_ms(500);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
                delay_ms(500);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
                delay_ms(500);
            }
            Motor_Data.xCAR_Track_Go();
            delay_ms(200);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            RFID_Data.xRFID_Read(Card1_Block); //定点读一下有没有卡
            delay_ms(500);
        Run_State = 3;
          break;
      }
      case 3:
      {
          //安卓识别TFTA（文字图片与图形颜色图片）
            Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//颜色+数量
            Android_Data.xMainCar_Send_Android(Identify_TFT_Words_Arr);//文字
            for(uint8_t i=0;i<8;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
            }
            //************************************************************（还未写安坐传来的数据）
            
            
            Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,"123404");//多功能信息显示标志物A显示123404（还不行）
            delay_ms(200);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(200);
            
          Run_State=4;
          break;
      }
      case 4:
      {
            //F2→B2
            Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止可能重复识别十字路口的卡片
          
           //对特殊标志物前面一小段进行识卡
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//识别到特殊路段停
            //过特殊标志物
            Motor_Data.xCAR_Go(25,500);
            Motor_Data.xCAR_Go(25,300);
            delay_ms(500);
            //特殊标志物后一小段进行寻卡
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡 
            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
            delay_ms(200); 
            Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
            
            Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);//识别蓝色车牌和车型
            for(uint8_t i=0;i<8;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
            }
            Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
            //启动从车
//            for(uint8_t i = 0; i<3; i++)//启动从车
//            {   
//              FollowCar_Data.xStart_Command_To_FollowCar();
//              delay_ms(100);
//            }   
            
            
       
//            for(uint8_t i=0;i<100;i++)
//            {
//                delay_ms(500);
//                delay_ms(500);
//            }
            //*******************************************这部分等待从车开启路灯后再执行              
            //B2->B4
//            Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);//车牌
            Barrier_Data.xBarrier_Licence_Tx("B8542D");//车牌号

            delay_ms(300);
            //B2->B4
            Motor_Data.xCAR_Track_Go();
            delay_ms(200);
            Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
            delay_ms(500);
            Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("富强民主");
            delay_ms(500);
            Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
            delay_ms(200);
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            delay_ms(200);
            YT2_Handle_Card1_Data(READ_RFID1);//处理数据
            delay_ms(500);
            
            if(strcmp(Card2_Place,"E4")==0)
            {
                //B4->D4
                 RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                 RFID_Data.xRFID_Track_Read_L(25,1400,Card2_Block);
                 Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                 Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                //D4->B4
                 Motor_Data.xCAR_Track_Go();
                 Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
                //B4->D6->B6
                 Motor_Data.xCAR_Track_Go();
                 Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                 Motor_Data.xCAR_Track_Go();
                 Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
            }
            else
            {
                //B4->D4->D6
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                
                if(strcmp(Card2_Place,"E6")==0)
                {
                    //D6->E6->B6
                    Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
                    RFID_Data.xRFID_Track_Read_L(25,1400,Card2_Block);
                    Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                    Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                    Motor_Data.xCAR_Track_Go();
                    Motor_Data.xCAR_Track_Go();
                    Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                }
                else
                {
                    //D6->B6
                    Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                    Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                    RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                    Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                    Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                }
            }
            
            //B6->B7(倒车)
            Motor_Data.xCAR_Go(30,140);//往前一点防止车库降一时压到车尾
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x01);
            Motor_Data.xCAR_Track_Time(20,1000);
            delay_ms(300); //延时
            Motor_Data.xCAR_Back(20,850);//长
            Motor_Data.xCAR_Back(20,850);//长
            delay_ms(500); //延时 
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x03);//需修改层数
            unsigned int Kai_Qi_Ma[3]={0x00,0x00,0x00}
            Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码
            delay_ms(200);
            LED_Display_Data.xLED_Display_Time(0x00);//停止计时
          Run_State=5;  
          break;
      }
      case 5:
      {
          

          Run_State=6;
          break;
      }
      case 6:
      {

          
          Run_State=7;
          break;
      }
      case 7:
      {

          
          Run_State=8;
          break;
      }
      case 8:
      {

            
          Run_State=9;
          break;
      }
      case 9:
      {

            
          Run_State=10;
          break;
      }
      case 10:
      {

            
          Run_State=11;
          break;
      }
      case 11:
      {

          Run_State=12;
          break;
      }
      
      default:break;           
    }

}

#endif


#if YT3

#define Card1_Block  17                    //填写卡1地址块
#define Card2_Block  2                     //****************卡2地址块需修改为0~2其中一个数，具体看安卓的识别结果
#define Card3_Block  0                     //卡3地址块

#define Card2_Place  card2_position        //卡2的坐标


uint8_t Get_ChuShi_Num_LuDeng;//路灯初始挡位
uint8_t Get_MuBiao_Num_LuDeng;//路灯目标挡位


void xAuto_Run_Function(void)
{
    switch(Run_State)                         
	{
      case 1:
      {	
            LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(300);
            
           //G6→F6
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card1_Block,0);//寻卡
            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
            Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
          
          //F6→F4 交通灯识别
            Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
            for(uint8_t i=0;i<3;i++) //等待安卓回传
            {
              delay_ms(500);
              delay_ms(500);
            }
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
                delay_ms(500);
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
                delay_ms(500);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
                delay_ms(500);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
                delay_ms(500);
            }
            Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card1_Block,0);//寻卡
            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
            Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
            delay_ms(200);

          Run_State=2;
          break;
      }
      case 2:
      {
          //公交站随机播报
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
            for(uint8_t i = 0;i<8;i++)  // 延时8秒等待小创识别并重复播报
            {
                delay_ms(500);
                delay_ms(500);
            }
            Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
            Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
            
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
            
            //F4→F2  ETC
            ETC_Data.xETC_Pass_RFID(Card1_Block,Card1_Block,0);
						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
//            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
            delay_ms(200);
        Run_State = 3;
          break;
      }
      case 3:
      {
				
            Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
            //安卓识别文字图片与图形颜色图片
            Android_Data.xMainCar_Send_Android(Identify_TFT_Words_Arr);//文字
            Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
            for(uint8_t i=0;i<8;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
            }
            
            Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,"A1D2E3");//多功能信息显示标志物A显示A1D2E3（还不行）
            LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
            delay_ms(500);
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            
            //路灯
            Get_ChuShi_Num_LuDeng = Smart_Light_Data.xSmart_Light_Get_Init_Level();         //获得路灯初始挡
            Get_MuBiao_Num_LuDeng = (Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]%4)+1;  //获得路灯目标挡
            Smart_Light_Data.xSmart_Light_Appoint_Level(Get_MuBiao_Num_LuDeng);            
            delay_ms(500);
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            delay_ms(200);
          Run_State=4;
          break;
      }
      case 4:
      {
            Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片

            //F2→B2
            track_time_Start=1;//开始计时(1ms)
            RFID_Data.xRFID_Track_Read(25,Card1_Block,Card1_Block,0);//寻卡
            delay_ms(400);
            
            if(Count_ms<600)//特殊标志物在E2
            {
                Count_ms=0;
                //过特殊标志物
                Motor_Data.xCAR_Go(25,500);
                Motor_Data.xCAR_Go(25,300);
                delay_ms(500);
                //特殊标志物后一小段进行寻卡
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                //D2->B2
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                delay_ms(200); 
            }
            else if((1500>Count_ms)&&(Count_ms>=600))//特殊标志物在D2    1450
            {
                Count_ms=0;
                //过特殊标志物
                Motor_Data.xCAR_Go(25,500);
                Motor_Data.xCAR_Go(25,300);
                delay_ms(500);
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                delay_ms(200); 
            }
            else//特殊标志物在C2
            {
                Count_ms=0;
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
								delay_ms(200);
							
                 //过特殊标志物
                Motor_Data.xCAR_Go(25,500);
                Motor_Data.xCAR_Go(25,500);
								delay_ms(600);
                
                RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                delay_ms(300); 
            }          
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            delay_ms(200);
            Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
            
            
          Run_State=5;  
          break;
      }
      case 5:
      {
          //B2 安卓识别车牌
            Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);//车牌
                                                                         //车型
            for(uint8_t i=0;i<8;i++)//等待八秒
            {
                delay_ms(500);
                delay_ms(500);
            }
            Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,"202501");//多功能信息显示标志物B显示202501（还不行）
            delay_ms(200);
            Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
            Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("富强民主");
            delay_ms(500);
            delay_ms(500);
            Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
            delay_ms(500);
              //启动从车
            for(uint8_t i = 0; i<3; i++)//启动从车
            {   
              FollowCar_Data.xStart_Command_To_FollowCar();
              delay_ms(100);
            } 
            for(uint8_t i = 0; i<50; i++)//等待
            {   
              delay_ms(500);
              delay_ms(500);
            }
            Run_State=6;  
          break;
      }
      case 6:
      {
          //B2->B6
          Motor_Data.xCAR_Track_Go();    
          Motor_Data.xCAR_Track_Go();
          delay_ms(200);
          Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
          //B6->D6
          Motor_Data.xCAR_Track_Go();
          
          
          
          Run_State=7;
          break;
      }
      case 7:
      {
           //D6->F6
           Barrier_Data.xBarrier_Licence_Tx("B8542D");//车牌号
           delay_ms(500);
           Motor_Data.xCAR_Track_Go();
          
          Run_State=8;
          break;
      }
      case 8:
      {
            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
            //B6->B7(倒车)
            Motor_Data.xCAR_Go(30,140);//往前一点防止车库降一时压到车尾
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x01);
            Motor_Data.xCAR_Track_Time(20,1000);
            delay_ms(300); //延时
            Motor_Data.xCAR_Back(20,850);//长
            Motor_Data.xCAR_Back(20,850);//长
            delay_ms(500); //延时 
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,Get_ChuShi_Num_LuDeng);
//            unsigned int Kai_Qi_Ma[3]={0x00,0x00,0x00};
            YT3_Handle_Card1_Data();
            Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码
            delay_ms(200);
            LED_Display_Data.xLED_Display_Time(0x00);//停止计时
          Run_State=9;
          break;
      }
      case 9:
      {

            
          Run_State=10;
          break;
      }
      case 10:
      {

            
          Run_State=11;
          break;
      }
      case 11:
      {
//					  track_time_Start=1;//开始计时(1ms)
//				 //F4→F2  ETC
//            ETC_Data.xETC_Pass_RFID(Card1_Block,Card1_Block,0);
//            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
//						Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);   
//            Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
//						Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
//						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
//          Run_State=12;
          break;
      }
			case 12:
			{
//				for(uint8_t i=0;i<5;i++)//等待八秒
//        {
//             Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A); 
//        }
//				Run_State=13;
//				break;
//				XiaoChuang_Data.xSend_Command_To_XiaoChuang(0x02);  //开始测距	
				Ultrasonic_Ranging();
				LED_Display_Data.xLED_Display_Distance(dis);
				FollowCar_Data.XSend_dis_To_fromcar();
				delay_ms(500);
				Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
				if(Android_Data.Two_Code_State == 1)    //识别二维码成功
				{
				Android_Data.Two_Code_State = 0;
				uint8_t buf[50];
				sprintf((char*)buf,"two_code data : %s \r\n",Two_Code_Data_Store);  
				Send_InfoData_To_Fifo((char*)buf,strlen((char*)buf)); 
				}
      
      default:break;           
    }

}

		

//			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
//			if(Android_Data.Two_Code_State == 1)    //识别二维码成功
//		  {
//			Android_Data.Two_Code_State = 0;
//			uint8_t buf[50];
//			sprintf((char*)buf,"two_code data : %s \r\n",Two_Code_Data_Store);  
//			Send_InfoData_To_Fifo((char*)buf,strlen((char*)buf)); 
//			Run_State = 6;
//		  }
}
#endif



#if YT4
	
#define Card1_Block  14                    //填写卡1地址块
#define Card2_Block  card2_sector_block                     //****************卡2地址块需修改为0~2其中一个数，具体看安卓的识别结果
#define Card2_Place  card2_position                     


void xAuto_Run_Function(void)
{
    switch(Run_State)                         
	{
      case 1:
			{
				LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
				LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
				delay_ms(300);
				
				//F7-F6
				Motor_Data.xCAR_Track_Go();
				RFID_Data.xRFID_Read(Card1_Block); //定点读一下有没有卡
				delay_ms(300);
        
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_Go(30,140);//走开十字路口，防止可能重复识别十字路口的卡片
				delay_ms(300);
				//F6-D6
				//公交站语音播报
				Motor_Data.xCAR_Go(45,700);
				delay_ms(400);
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_Go(30,140);
				delay_ms(300);
				XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
				delay_ms(500);
				Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
				for(uint8_t i = 0;i<8;i++)  // 延时8秒等待小创识别并重复播报
				{
						delay_ms(500);
						delay_ms(500);
				}
				Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
				Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
				Voice_Report_Data.xVoice_Report_Speak_Weather();//播报天气状况
				delay_ms(300);
				Run_State = 2;
				break;
			}
			case 2:
			{
				//D6-B6
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_Go(30,300);
				delay_ms(300);
				ETC_Data.xETC_Pass_RFID(Card1_Block,Card2_Block,0);
				Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				for(uint8_t i = 0; i<2; i++)//等待
				{   
					delay_ms(500);
					delay_ms(500);
				}

				//测距
				Ultrasonic_Ranging();
				FollowCar_Data.XSend_dis_To_fromcar();
				//识别二维码
				Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
				if(Android_Data.Two_Code_State == 1)    //识别二维码成功
				{
				Android_Data.Two_Code_State = 0;
				uint8_t buf[50];
				sprintf((char*)buf,"two_code data : %s \r\n",Two_Code_Data_Store);  
				Send_InfoData_To_Fifo((char*)buf,strlen((char*)buf)); 
				delay_ms(500);
				}
				Run_State = 3;
				break;
			}
			case 3:
			{
				//B6-B4
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
				Motor_Data.xCAR_Go(30,140);//走开十字路口，防止可能重复识别十字路口的卡片
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr); 
				for(uint8_t i=0;i<8;i++)//等待八秒
				{
						delay_ms(500);
						delay_ms(500);
				}
				for(uint8_t i=0;i<3;i++)//发送三次
				{
						Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,"A1D2E3");//多功能信息显示标志物A显示A1D2E3（还不行）
						delay_ms(200);
						LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
						delay_ms(200);
				}
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				delay_ms(200);
				Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
				
//				delay_ms(500);
				//立体显示

				if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x00)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("大风");
				else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x01)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("多云");
				else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x02)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("晴");
				else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x03)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("小雪");
				else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x04)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("小雨");
				else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x05)
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("阴天");
				for(uint8_t i = 0; i<3; i++)
				{   
					delay_ms(500);
					delay_ms(500);
				}
				
				Run_State = 4;
				break;
			}
			case 4:
			{
				//B4-D4  交通灯识别
				Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
				delay_ms(1000);
				Motor_Data.xCAR_Track_Time(20,800);//使车身摆正
				Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
				for(uint8_t i=0;i<3;i++) //等待安卓回传
				{
					delay_ms(500);
					delay_ms(500);
				}
				if(Android_Data.Red_State == 1)
				{
						Android_Data.Red_State = 0;
						Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
						delay_ms(500);
				}
				else if(Android_Data.Yellow_State == 1)
				{
						Android_Data.Yellow_State = 0;
						Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
						delay_ms(500);
				}
				else if(Android_Data.Green_State == 1)
				{
						Android_Data.Green_State = 0;
						Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
						delay_ms(500);
				}
				else//蒙一个
				{
						Android_Data.Green_State = 0;
						Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
						delay_ms(500);
				}
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
				Motor_Data.xCAR_Go(30,140);//走开十字路口，防止可能重复识别十字路口的卡片
				 //启动从车
//				for(uint8_t i = 0; i<3; i++)//启动从车
//				{   
//					FollowCar_Data.xStart_Command_To_FollowCar();
//					delay_ms(100);
//				} 
//				for(uint8_t i = 0; i<20; i++)//等待
//				{   
//					delay_ms(500);
//					delay_ms(500);
//				}
				//D4-F4
				
				Barrier_Data.xBarrier_Licence_Tx("B8542D");//车牌号
				delay_ms(500);			
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,Card2_Block);//寻卡
				YT1_Handle_Card1_Data(READ_RFID1);//************************************解析处理卡1内的信息，得到卡2的位置和读取的地址块
				delay_ms(500);
				
				Run_State = 5;
				break;
			}
			case 5:
			{
				
				//安卓识别车型和行人口罩数量
				Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr);//主车发给安卓请求识别TFT口罩行人
				for(uint8_t i=0;i<8;i++)//等待八秒
				{
						delay_ms(500);
						delay_ms(500);
				}
				Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,"FF0205");//多功能信息显示标志物B显示FF0105（还不行）
				delay_ms(200);
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				delay_ms(300);
				//F4-F2
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
				Motor_Data.xCAR_Go(20,250);
				delay_ms(300);
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_Go(20,250);//走开十字路口，防止可能重复识别十字路口的卡片
				delay_ms(500);
				Run_State = 6;
				break;
			}
			case 6:
			{
				//F2-B2  过特殊地形
				track_time_Start=1;//开始计时(1ms) 
				RFID_Data.xRFID_Track_Read(15,Card1_Block,Card2_Block,0);//寻卡
				delay_ms(500);
				
				if(Count_ms<600)//特殊标志物在E2  600
				{
						Count_ms=0;
						//过特殊标志物
						Motor_Data.xCAR_Go(25,550);
						Motor_Data.xCAR_Go(25,350); 
						delay_ms(500);
						//特殊标志物后一小段进行寻卡
						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
						//D2->B2
						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
						delay_ms(200); 
				}
				else if((1600>Count_ms)&&(Count_ms>=600))//特殊标志物在D2    1450
				{
						Count_ms=0;
						//过特殊标志物
						Motor_Data.xCAR_Go(25,500);
						Motor_Data.xCAR_Go(25,300);
						delay_ms(500);
						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
						delay_ms(200); 
				}
				else//特殊标志物在C2
				{
						Count_ms=0;
						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片 
						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
						delay_ms(200);
					
						 //过特殊标志物
						Motor_Data.xCAR_Go(25,500);
						Motor_Data.xCAR_Go(25,300);
						delay_ms(600);
						
						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
						delay_ms(300); 
				} 
				 
				Run_State = 7;		
				break;
			}
			case 7:
			{
				//倒车入库
				Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_Track_Time(20,1000);
				delay_ms(300); //延时
				Motor_Data.xCAR_Back(20,900);
				Motor_Data.xCAR_Back(20,900);
//				Motor_Data.xCAR_Back(16,500);
				delay_ms(500); //延时 
//            unsigned int Kai_Qi_Ma[3]={0x00,0x00,0x00};
				//处理卡2数据
				Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码
//				delay_ms(200);
				LED_Display_Data.xLED_Display_Time(0x00);//停止计时
				
				Run_State = 8;
				break;
			}
			case 8:
			{
				Run_State = 9;
				break;
			}
			default:break;
			
	}
	
}


#endif

#if YT5
#define Card1_Block  17                    //填写卡1地址块
#define Card2_Block  2                     //****************卡2地址块需修改为0~2其中一个数，具体看安卓的识别结果
#define Card3_Block  0                     //卡3地址块

#define Card2_Place  card2_position        //卡2的坐标

uint8_t Get_ChuShi_Num_LuDeng;//路灯初始挡位
uint8_t Get_MuBiao_Num_LuDeng;//路灯目标挡位


void xAuto_Run_Function(void)
{
	switch(Run_State)  
	{
		case 1:
		{
			LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
			LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
			delay_ms(300);
			//D7-D6
			Motor_Data.xCAR_Track_Go();
			delay_ms(300);
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			//D6-B6
			ETC_Data.xETC_Pass();
			delay_ms(300);
			//公交站随机播报
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
			delay_ms(500);
			Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
			for(uint8_t i = 0;i<8;i++)  // 延时8秒等待小创识别并重复播报
			{
					delay_ms(500);
					delay_ms(500);
			}
			Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
			Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
			delay_ms(300);
			Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
			delay_ms(200);
			Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			Run_State = 2;
			break;
		}
		case 2:
		{
			//B6-B4
			Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
			for(uint8_t i=0;i<3;i++) //等待安卓回传
			{
				delay_ms(500);
				delay_ms(500);
			}
			if(Communication_Data.Smart_Traffic_A_Recognition_State == 1){
			if(Android_Data.Red_State == 1)
			{
					Android_Data.Red_State = 0;
					Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
					delay_ms(500);
			}
			else if(Android_Data.Yellow_State == 1)
			{
					Android_Data.Yellow_State = 0;
					Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
					delay_ms(500);
			}
			else if(Android_Data.Green_State == 1)
			{
					Android_Data.Green_State = 0;
					Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
					delay_ms(500);
			}
			else//蒙一个
			{
					Android_Data.Green_State = 0;
					Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
					delay_ms(500);
			}
		}
			Motor_Data.xCAR_Track_Go();
			delay_ms(200);
			RFID_Data.xRFID_Read(Card1_Block); //定点读一下有没有卡
			delay_ms(500);
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			
			delay_ms(300);
			
			Run_State = 3;
			break;
		}
		case 3:
		{
			//识别车牌车型
			for(uint8_t i=0;i<8;i++)
			{
				Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
				delay_ms(1000);
			}
			Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);//车牌                                                               
			for(uint8_t i=0;i<8;i++)//等待八秒
			{
					delay_ms(500);
					delay_ms(500);
			}
			Smart_TFT_Data.xSmart_TFT_Licence_Display(2,"F111F1");//多功能信息显示标志物B显示
			delay_ms(200);
			char licence[] = "F111F1";  // 车牌信息
			FollowCar_Data.XSend_Licence_To_fromcar(licence);       //发送车牌信息给从车

			Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
			delay_ms(300);
			Run_State = 4;
			break;
		}
		case 4:
		{
//			track_time_Start=1;//开始计时(1ms) 
//				RFID_Data.xRFID_Track_Read(15,Card1_Block,Card2_Block,0);//寻卡
//				delay_ms(500);
//				
//				if(Count_ms<600)//特殊标志物在E2  600
//				{
//						Count_ms=0;
//						//过特殊标志物
//						Motor_Data.xCAR_Go(25,550);
//						Motor_Data.xCAR_Go(25,350); 
//						delay_ms(500);
//						//特殊标志物后一小段进行寻卡
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
//						//D2->B2
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
//						delay_ms(200); 
//				}
//				else if((1600>Count_ms)&&(Count_ms>=600))//特殊标志物在D2    1450
//				{
//						Count_ms=0;
//						//过特殊标志物
//						Motor_Data.xCAR_Go(25,500);
//						Motor_Data.xCAR_Go(25,300);
//						delay_ms(500);
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
//						delay_ms(200); 
//				}
//				else//特殊标志物在C2
//				{
//						Count_ms=0;
//						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片 
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//						delay_ms(200);
//					
//						 //过特殊标志物
//						Motor_Data.xCAR_Go(25,500);
//						Motor_Data.xCAR_Go(25,300);
//						delay_ms(600);
//						
//						RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
//						delay_ms(300); 
//				} 
			
			//B4-F4
			//对特殊标志物前面一小段进行识卡
      RFID_Data.xRFID_Track_Read(25,Card1_Block,0,0);//识别到特殊路段停 
			//过特殊标志物
			Motor_Data.xCAR_Go(25,500);
			Motor_Data.xCAR_Go(25,300);
			delay_ms(500);
			//特殊标志物后一小段进行寻卡
			RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡 
			
			Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
			delay_ms(200); 
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			
			 //启动从车		
			for(uint8_t i = 0; i<3; i++)//启动从车
			{   
				FollowCar_Data.xStart_Command_To_FollowCar(fromcar_start1);
				delay_ms(100);
			} 
			for(uint8_t i = 0; i<10; i++)//等待
			{   
				delay_ms(500);
				delay_ms(500);
			}
			YT2_Handle_Card1_Data(READ_RFID1);//************************************解析处理卡1内的信息
			delay_ms(500);
			Run_State = 5;
			break;
		}
		case 5:
		{
			
			//F4-F2
			RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
			Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
			Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
			delay_ms(200);
			
			//安卓识别文字图片与图形颜色图片
			for(uint8_t i=0;i<8;i++)
			{
				Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_A,2);  // 向下翻页
				delay_ms(2000);
			}
			Android_Data.xMainCar_Send_Android(Identify_TFT_Words_Arr);//文字
			Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
			
			for(uint8_t i=0;i<8;i++)//等待八秒
			{
					delay_ms(500);
					delay_ms(500);
			}
			
			
			Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,"A1D2E3");//多功能信息显示标志物A显示A1D2E3（还不行）
//			LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
			//将有效文字发送给从车
			const char* text = "富强民主";  // 文字信息
			FollowCar_Data.XSend_Text_To_fromcar(text);  // 发送文字信息给从车
			delay_ms(500);
			

			
			Run_State = 6;
			break;
		}
		case 6:
		{
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			//路灯感知
			Get_ChuShi_Num_LuDeng = Smart_Light_Data.xSmart_Light_Get_Init_Level();         //获得路灯初始挡
			Get_MuBiao_Num_LuDeng = (((Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]/5)^(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1]))%4)+1;  //获得路灯目标挡
			Smart_Light_Data.xSmart_Light_Appoint_Level(Get_MuBiao_Num_LuDeng);            
			delay_ms(500);
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			delay_ms(300);
			Run_State = 7;
			break;
		}
		case 7:
		{		
			if(strcmp(Card2_Place,"D1")==0)
			{
				//F2-D2-D1-D2-B2
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
				 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				 delay_ms(300);
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				delay_ms(300);
				RFID_Data.xRFID_Track_Read_L(25,1400,Card2_Block);
				Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				delay_ms(300);
				Motor_Data.xCAR_Track_Go();
				delay_ms(300);
				Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
				delay_ms(300);
				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("富强民主");
				for(uint8_t i =0;i<3;i++)
				{
					delay_ms(500);
					delay_ms(500);
				}
				Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
				delay_ms(300);
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
				Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				delay_ms(300);
			}
			else
			{
				//F2-D2
				RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
				 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				 delay_ms(300);
				
				if(strcmp(Card2_Place,"D3")==0)
				{
					//D2-D3-D2-B2
					Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
					delay_ms(300);
					RFID_Data.xRFID_Track_Read_L(25,1400,Card2_Block);
					Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
					delay_ms(300);
					Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
					Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
					Motor_Data.xCAR_Track_Go();
					delay_ms(300);
					Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
					Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("富强民主");
					for(uint8_t i =0;i<3;i++)
					{
						delay_ms(500);
						delay_ms(500);
					}
					Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
					delay_ms(300);
					RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
					Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
					delay_ms(300);
				}	
				else
				{
					//D2-B2
					Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
					Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("富强民主");
					for(uint8_t i =0;i<3;i++)
					{
						delay_ms(500);
						delay_ms(500);
					}
					Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
					delay_ms(300);
					RFID_Data.xRFID_Track_Read(25,Card1_Block,Card2_Block,0);//寻卡
//					Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
					delay_ms(300);
				}
			}
			Run_State = 8;
			break;
	}
			case 8:
			{
					//倒车入库
					Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
					delay_ms(300);
					Motor_Data.xCAR_Go(30,140);//往前一点防止车库降一时压到车尾
					CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,0x01); 
					Motor_Data.xCAR_Track_Time(20,1300);
					delay_ms(300); //延时
					Motor_Data.xCAR_Back(20,1000);
					Motor_Data.xCAR_Back(20,950);
					delay_ms(500); //延时 
					CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,YT5_Carport);   //目标层数由车型信息得到(已求出)
					unsigned int Kai_Qi_Ma[3]={0xA1,0x23,0xB4};
					Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码
					Wireless_Charge_Data.xWireless_Charge_Open_Close(Wireless_Charge_OPEN);
	//				delay_ms(200);
					LED_Display_Data.xLED_Display_Time(0x00);//停止计时
					
					Run_State =9;
				break;
			}
			case 9:
			{
					Run_State =10;
				break;
			}
			case 10:
			{
					Run_State =11;
				break;
			}
			default:break;
	}
}


#endif
		 // uint8_t num = 0;
//		  num = (mask_data^mask_data)%4 + 1;
		  // 发送从车数据
//		  uint8_t dat1,dat2,diss = 125;
//		  
//		  dat1 = diss/10/10;
//		  dat2 = diss/10%10;
//		  uint8_t fromcar_data[8] = {0x55,0x02,0xA4,dat1,dat2,0x00,0x00,0xBB};
//		  FollowCar_Data.xStart_Command_To_FollowCar(fromcar_data);		  
//		  





