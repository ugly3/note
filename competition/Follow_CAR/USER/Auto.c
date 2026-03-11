/**
************************************************************************
*    文件名：all_module
*      说明：全自动运行（所用头文件+任务）
************************************************************************
**/

#include "all_module.h"

#define YT3_2025 0
#define YT1_2026 0
#define YT2_2026 0
#define YT3_2026 0
#define YT4_2026 0
#define YT5_2026 0

uint8_t Go_Speed = 50;     // 全局行进速度值
uint8_t wheel_Speed = 90;  // 全局转弯速度值
uint16_t wheel_Time = 330; // 全局转45°时间
uint16_t distance = 0;     // 记录超声波数据
uint8_t gear_init = 0;     // 智能路灯初始档位

uint8_t Run_State = 0; // 运行位置状态

#if YT3_2025

#define Card1_Block 17 // 填写卡1地址块
#define Card2_Block 2  //****************卡2地址块需修改为0~2其中一个数，具体看安卓的识别结果
#define Card3_Block 0  // 卡3地址块

#define Card2_Place card2_position // 卡2的坐标

uint8_t Get_ChuShi_Num_LuDeng; // 路灯初始挡位
uint8_t Get_MuBiao_Num_LuDeng; // 路灯目标挡位

void xAuto_Run_Function(void)
{
  switch (Run_State)
  {
  case 1:
  {
    LED_Display_Data.xLED_Display_Data(0x00, 0x00, 0x00, 2);        // LED显示标志物第二排显示000000
    LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart); // 开始计时
    delay_ms(300);

    // G6→F6
    Motor_Data.xCAR_Go(30, 140); // 使车身对准十字路口
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);

    // F6→F4 交通灯识别
    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A); // 请求交通灯进入识别模式，并请求安卓识别红绿灯
    for (uint8_t i = 0; i < 3; i++)                                           // 等待安卓回传
    {
      delay_ms(500);
      delay_ms(500);
    }
    if (Android_Data.Red_State == 1)
    {
      Android_Data.Red_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 1); // 发送给交通灯标志物请求确认
      delay_ms(500);
    }
    else if (Android_Data.Yellow_State == 1)
    {
      Android_Data.Yellow_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 3);
      delay_ms(500);
    }
    else if (Android_Data.Green_State == 1)
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
      delay_ms(500);
    }
    else // 蒙一个
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
      delay_ms(500);
    }
    Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片
    Motor_Data.xCAR_Go(30, 140); // 使车身对准十字路口
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    delay_ms(200);

    Run_State = 2;
    break;
  }
  case 2:
  {
    // 公交站随机播报
    XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
    delay_ms(500);
    Voice_Report_Data.xVoice_Report_Random_Command(); // 播报随机指令
    for (uint8_t i = 0; i < 8; i++)                   // 延时8秒等待小创识别并重复播报
    {
      delay_ms(500);
      delay_ms(500);
    }
    Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur(); // 查询天气温度
    Voice_Report_Data.xVoice_Report_Speak_temperature();          // 播报温度

    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片

    // F4→F2  ETC
    ETC_Data.xETC_Pass_RFID(Card1_Block, Card1_Block, 0);
    Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片
    //            Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
    delay_ms(200);
    Run_State = 3;
    break;
  }
  case 3:
  {

    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    // 安卓识别文字图片与图形颜色图片
    Android_Data.xMainCar_Send_Android(Identify_TFT_Words_Arr);                // 文字
    Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr); // 图形加颜色的数量
    for (uint8_t i = 0; i < 8; i++)                                            // 等待八秒
    {
      delay_ms(500);
      delay_ms(500);
    }

    //        char Hex_num[3]={0x01,0x20,0x26};
    //        Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,Hex_num);//TFT显示012026
    LED_Display_Data.xLED_Display_Data(0xF3, 0xF5, 0xF1, 2); // LED显示标志物第二排显示F3F5F1
    delay_ms(500);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);

    // 路灯
    Get_ChuShi_Num_LuDeng = Smart_Light_Data.xSmart_Light_Get_Init_Level();                     // 获得路灯初始挡
    Get_MuBiao_Num_LuDeng = (Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[1] % 4) + 1; // 获得路灯目标挡
    Smart_Light_Data.xSmart_Light_Appoint_Level(Get_MuBiao_Num_LuDeng);
    delay_ms(500);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(200);
    Run_State = 4;
    break;
  }
  case 4:
  {
    Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片

    // F2→B2
    delay_ms(400);

    if (Count_ms < 600) // 特殊标志物在E2
    {
      Count_ms = 0;
      // 过特殊标志物
      Motor_Data.xCAR_Go(25, 500);
      Motor_Data.xCAR_Go(25, 300);
      delay_ms(500);
      Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片
      // D2->B2
      Motor_Data.xCAR_Go(30, 140); // 使车身对准十字路口
      delay_ms(200);
    }
    else if ((1500 > Count_ms) && (Count_ms >= 600)) // 特殊标志物在D2    1450
    {
      Count_ms = 0;
      // 过特殊标志物
      Motor_Data.xCAR_Go(25, 500);
      Motor_Data.xCAR_Go(25, 300);
      delay_ms(500);
      Motor_Data.xCAR_Go(30, 140); // 使车身对准十字路口
      delay_ms(200);
    }
    else // 特殊标志物在C2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Go(25, 250); // 走开十字路口，防止可能重复识别十字路口的卡片
      delay_ms(200);

      // 过特殊标志物
      Motor_Data.xCAR_Go(25, 500);
      Motor_Data.xCAR_Go(25, 500);
      delay_ms(600);

      Motor_Data.xCAR_Go(30, 140); // 使车身对准十字路口
      delay_ms(300);
    }
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(200);
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);

    Run_State = 5;
    break;
  }
  case 5:
  {
    // B2 安卓识别车牌
    Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr); // 车牌
                                                                  // 车型
    for (uint8_t i = 0; i < 8; i++)                               // 等待八秒
    {
      delay_ms(500);
      delay_ms(500);
    }
    uint8_t
        //        char Hex_num[3]={0x01,0x20,0x26};
        //        Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,Hex_num);//TFT显示012026
        delay_ms(200);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add("富强民主");
    delay_ms(500);
    delay_ms(500);
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    delay_ms(500);
    // 启动从车
    for (uint8_t i = 0; i < 3; i++) // 启动从车
    {
      FollowCar_Data.xStart_Command_To_FollowCar();
      delay_ms(100);
    }
    for (uint8_t i = 0; i < 50; i++) // 等待
    {
      delay_ms(500);
      delay_ms(500);
    }
    Run_State = 6;
    break;
  }
  case 6:
  {
    // B2->B6
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_Track_Go();
    delay_ms(200);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    // B6->D6
    Motor_Data.xCAR_Track_Go();

    Run_State = 7;
    break;
  }
  case 7:
  {
    // D6->F6
    Barrier_Data.xBarrier_Licence_Tx("B8542D"); // 车牌号
    delay_ms(500);
    Motor_Data.xCAR_Track_Go();

    Run_State = 8;
    break;
  }
  case 8:
  {
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    // B6->B7(倒车)
    Motor_Data.xCAR_Go(30, 140); // 往前一点防止车库降一时压到车尾
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 0x01);
    Motor_Data.xCAR_Track_Time(20, 1000);
    delay_ms(300);                 // 延时
    Motor_Data.xCAR_Back(20, 850); // 长
    Motor_Data.xCAR_Back(20, 850); // 长
    delay_ms(500);                 // 延时
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, Get_ChuShi_Num_LuDeng);
    //            unsigned int Kai_Qi_Ma[3]={0x00,0x00,0x00};
    YT3_Handle_Card1_Data();
    Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma); // 需修改开启码
    delay_ms(200);
    LED_Display_Data.xLED_Display_Time(0x00); // 停止计时
    Run_State = 9;
    break;
  }
  case 9:
  {

    Run_State = 10;
    break;
  }
  case 10:
  {

    Run_State = 11;
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
    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
    if (Android_Data.Two_Code_State == 1)                      // 识别二维码成功
    {
      Android_Data.Two_Code_State = 0;
      uint8_t buf[50];
      sprintf((char *)buf, "two_code data : %s \r\n", Two_Code_Data_Store);
      Send_InfoData_To_Fifo((char *)buf, strlen((char *)buf));
    }

    break;
  }
  default:
    break;
  }
}
#endif
/*****************************************从车启动*********************************/
//          if(Communication_Data.FollowCar_Start_flag == 1)
//          {
//              Count++;
//              sprintf((char*)Buf1,"%d\r\n ",Count);
//              Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1));
//              Communication_Data.FollowCar_Start_flag = 0;
//              Motor_Data.xCAR_Track_Go();
//              Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
//              Motor_Data.xCAR_Track_Go();
//              Run_State = 2;
//           }
//          sprintf((char*)Buf1,"flag:%d\r\n ",Communication_Data.FollowCar_Start_flag);
//          Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1));

/*****************************************二维码识别*********************************/
//        Identify_Two_Code_Arr[3] = 0x01;
//        Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
//        delay_ms(500);
//        delay_ms(500);
//        delay_ms(500);
//        YT3_parse_two_codes();
//
//        if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
//		{
////            Motor_Data.xCAR_Back(20,150);
//            Rx_count = 0;
//			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
//			delay_ms(500);
//            delay_ms(500);
//            delay_ms(500);
////            Motor_Data.xCAR_Go(20,150);
//        }
//        if(Android_Data.Two_Code_State == 0)
//        {
//            Android_Data.Two_Code_State = 1;
//            printf("aaa  ");
//        }
//        if(Android_Data.Two_Code_State == 1)
//        {
//            Android_Data.Two_Code_State = 0;
//            Rx_count = 0;
//            YT3_parse_two_codes();
//
//            printf("sdv\r\n");
////            printf("%s\r\n",Two_Code_Data_parsed_Store1);
////            printf("%s\r\n",Two_Code_Data_parsed_Store2);
////            printf("%s\r\n",Two_Code_Data_parsed_Store3);
//
//            char Vioce_Store[50] = "识别结果为";
//            strcat(Vioce_Store,Two_Code_Data_parsed_Store1);

//            Voice_Report_Data.xVoice_Report_Set_Complex_Command(Vioce_Store);
//            Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add(Two_Code_Data_parsed_Store2);

//        }

/************************************************车型识别*********************************************************/
//        Identify_TFT_CheXin_Arr[3]=0x02;
//        Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);

/************************************************车库*******************************************************/
//        CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,3);
/************************************************LED显示*********************************************************/
//        LED_Display_Data.xLED_Display_Data(0x00,0x00,0x00,2);//LED显示标志物第二排显示000000
//        LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
//        LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeClose);
//        LED_Display_Data.xLED_Display_Distance(125);
//        delay_ms(500);
//        delay_ms(500);
//        uint8_t Display_Data[]={0xF3,0xF5,0xF1};
//        LED_Display_Data.xLED_Display_Data(Display_Data,2);//LED显示标志物第二排显示F3F5F1
//        Motor_Data.xCAR_Track_Go();
//        Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);

/****************************************************RFID**************************************************/
//        RFID_Data.xRFID_Write(Judge_Card_Block);
//        RFID_Data.xRFID_Track_Read_2(30,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
//        Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
//        RFID_Data.xRFID_Track_Read_2(30,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
//        Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
//        RFID_Data.xRFID_Track_Read_L2(30,1400,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
//        uint8_t block_add = 0;
//        block_add = 4*4+2-1; //(a-1)*4+b-1;(a-1)*4+b;a*4+b-1; a*4+b  第5扇区第2数据块  17 18 21 22
//        RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
/**********************************************打印数据**********************************************************/
//    sprintf((char*)Buf,"%s \r\n",WRITE_RFID);
//      Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
//        for(uint8_t i=100;i<120;i++)
//        {
//            sprintf((char*)Buf,"%d ",i);
//            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
//            delay_ms(500);
//        }
/*****************************************************无线充电标志物***********************************************/
//        Wireless_Charge_Data.xWireless_Charge_Open_Close(Wireless_Charge_OPEN);
//        delay_ms(500);
//        delay_ms(500);
//        Wireless_Charge_Data.xWireless_Charge_Open_Close(Wireless_Charge_CLOSE);

//        uint8_t Init_Open_Kaiqima[3]={0xA1,0x23,0xB4};
//        Wireless_Charge_Data.xWireless_Charge_KaiQiMa_XiuGai(Init_Open_Kaiqima);
//        delay_ms(500);
//        delay_ms(500);
//        Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Init_Open_Kaiqima);

/*****************************************************烽火台标志物***********************************************/
//        SmokeTower_Data.SmokeTower_Infrared_Open();
//        uint8_t SmokeTower_Open_Data[6]={0};
//        SmokeTower_Data.xSmokeTower_Zigbee_Open(SmokeTower_Open_Data);

//            uint8_t  Kai_Qi_Ma[3]={0x00,0x00,0x00};
//            Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码

#if YT1_2026
#define Two_Code_Count 2
uint8_t Count = 0;
uint8_t time_out = 0;
uint8_t Buf1[200];
uint8_t Hex_Data_Store1[10];
bool lock_flag = 0; // 锁车标志，0为未锁车，1为已锁车
void xAuto_Run_Function(void)
{
  if (lock_flag == 0)
  {
    while (Communication_Data.FollowCar_Start_flag == 0)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out > 230) // 等待10秒后超时
      {
        Run_State = 1; // 设置状态为1，进入下一阶段
        time_out = 0;  // 重置超时计数器
        break;         // 跳出循环，继续执行后续代码
      }
    }
    Run_State = 1; // 设置状态为1，进入下一阶段
    lock_flag = 1;
  }
  switch (Run_State)
  {
  case 1:
  {
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    delay_ms(200);
    // 二维码识别
    Identify_Two_Code_Arr[3] = 0x01;
    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
    delay_ms(500);
    delay_ms(500);
    delay_ms(500);
    YT1_parse_two_codes();

    if (Android_Data.Two_Code_State == 0) // 如果未进入识别模式
    {
      Motor_Data.xCAR_Back(20, 450);
      Rx_count = 0;
      Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
      delay_ms(500);
      delay_ms(500);
      delay_ms(500);
      Motor_Data.xCAR_Go(20, 520);
    }
    if (Android_Data.Two_Code_State == 0)
    {
      Android_Data.Two_Code_State = 1;
      printf("aaa  ");
    }
    if (Android_Data.Two_Code_State == 1)
    {
      Android_Data.Two_Code_State = 0;
      Rx_count = 0;
      YT1_parse_two_codes();

      printf("sdv\r\n");
      printf("%s\r\n", Two_Code_Data_parsed_Store1);
      printf("%s\r\n", Two_Code_Data_parsed_Store2);

      // 每个字符转成十六进制，无线充电标志物开启码
      for (int i = 0; i < strlen((char *)Two_Code_Data_parsed_Store2); i++)
      {
        char c = Two_Code_Data_parsed_Store2[i];
        if (c >= '0' && c <= '9')
          Hex_Data_Store1[i] = c - '0';
        else if (c >= 'A' && c <= 'F')
          Hex_Data_Store1[i] = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
          Hex_Data_Store1[i] = c - 'a' + 10;
      }

      Send_wireless_open_To_MainCar[3] = Hex_Data_Store1[0];
      Send_wireless_open_To_MainCar[4] = Hex_Data_Store1[1];
      Send_wireless_open_To_MainCar[5] = Hex_Data_Store1[2];
      MainCar_Data.xSend_Command_To_MainCar(Send_wireless_open_To_MainCar);
      delay_ms(100);
      MainCar_Data.xSend_Command_To_MainCar(Send_wireless_open_To_MainCar);

      if (strcmp((const char *)Two_Code_Data_parsed_Store1, "D7") == 0)
        Send_Weizhi_To_MainCar[3] = 0x01;
      else if (strcmp((const char *)Two_Code_Data_parsed_Store1, "F7") == 0)
        Send_Weizhi_To_MainCar[3] = 0x02;
      else if (strcmp((const char *)Two_Code_Data_parsed_Store1, "G6") == 0) // G6
        Send_Weizhi_To_MainCar[3] = 0x03;
      else // G4
        Send_Weizhi_To_MainCar[3] = 0x04;

      MainCar_Data.xSend_Command_To_MainCar(Send_Weizhi_To_MainCar);
      delay_ms(100);
      MainCar_Data.xSend_Command_To_MainCar(Send_Weizhi_To_MainCar);
      delay_ms(100);
    }
    Run_State = 2;
    break;
  }

  case 2:
  {
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    Motor_Data.xCAR_Track_Go();
    delay_ms(200); // B4
    // 交通灯
    uint8_t time_out = 0;
    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A); // 发送请求识别红绿灯B
    while (Android_Data.traffic_light_flag != 1)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out >= 5)
      {
        time_out = 0;
        break;
      }
    }
    Android_Data.traffic_light_flag = 0;
    if (Android_Data.Red_State == 1)
    {
      Android_Data.Red_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 1); // 发送给交通灯标志物请求确认
    }
    else if (Android_Data.Yellow_State == 1)
    {
      Android_Data.Yellow_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 3);
    }
    else if (Android_Data.Green_State == 1)
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    else // 蒙一个
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_R90(wheel_Speed + 10, wheel_Time * 2);

    //          sprintf((char*)Buf1,"flag:%d\r\n ",Communication_Data.FollowCar_Start_flag);
    //          Send_InfoData_To_Fifo((char*)Buf1,strlen((char*)Buf1));

    Run_State = 3;
    break;
  }
  case 3:
  {
    // B2->F2
    delay_ms(400);
    track_time_Start = 1;
    Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹
    delay_ms(400);
    sprintf((char *)Buf1, "Count_ms:%d\r\n ", Count_ms);
    Send_InfoData_To_Fifo((char *)Buf1, strlen((char *)Buf1));

    if (Count_ms < 600) // B2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Back(20, 600);
      delay_ms(400);
      Motor_Data.xCAR_Track(20);
      delay_ms(400);
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(500);
      Motor_Data.xCAR_Track_Go();
      Motor_Data.xCAR_Track_Go();
      delay_ms(200);
    }
    else if (1200 > Count_ms) // D2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(400);
      Motor_Data.xCAR_Track_Go();
      delay_ms(200);
    }
    else // E2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Go(25, 300); // 使车身对准十字路口
      delay_ms(500);
      Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹
      delay_ms(500);
      Motor_Data.xCAR_Back(20, 600);
      delay_ms(400);
      Motor_Data.xCAR_Track(20);
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(500);

      Motor_Data.xCAR_Track_Go();
      delay_ms(300);
    }
    Run_State = 4;
    break;
  }
  case 4:
  {
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_Track_Go();
    delay_ms(200); // F6
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_Track_Go(); // D6
    Motor_Data.xCAR_Track_Go(); // B6
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 1);

    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    delay_ms(200);
    Motor_Data.xCAR_Track_Time(35, 700);
    Motor_Data.xCAR_Back(35, 600);

    Motor_Data.xCAR_Track_Time(35, 700);
    delay_ms(200);

    Motor_Data.xCAR_Back(35, 600);
    Motor_Data.xCAR_Back(35, 250);
    printf("%d\r\n", MainCar_Send_Cengshu);
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, MainCar_Send_Cengshu);
    delay_ms(200);
    MainCar_Data.xSend_Command_To_MainCar(Send_wireless_open_To_MainCar);
    MainCar_Data.xSend_Command_To_MainCar(Send_Weizhi_To_MainCar);
    MainCar_Data.xStart_Command_To_MainCar(); // 启动主车
    delay_ms(200);
    MainCar_Data.xStart_Command_To_MainCar();
    delay_ms(200);
    MainCar_Data.xStart_Command_To_MainCar();

    Run_State = 5;
    break;
  }
  case 5:
  {
    Run_State = 6;
    break;
  }
  case 6:
  {
    Run_State = 7;
    break;
  }
  case 7:
  {
    Run_State = 8;
    break;
  }
  case 8:
  {
    Run_State = 9;
    break;
  }
  case 9:
  {
    Run_State = 10;
    break;
  }
  case 10:
  {

    Run_State = 11;
    break;
  }
  case 11:
  {

    Run_State = 12;
    break;
  }
  case 12:
  {

    Run_State = 13;
    break;
  }
  }
}

#endif

#if YT2_2026
uint8_t Buf1[200];
void xAuto_Run_Function(void)
{
  switch (Run_State)
  {
  case 1:
  {
    Motor_Data.xCAR_Track_Go(); // B6
    Motor_Data.xCAR_Track_Go();
    delay_ms(200); // B4
                   // 交通灯
    uint8_t time_out = 0;
    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A); // 发送请求识别红绿灯B
    while (Android_Data.traffic_light_flag != 1)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out >= 5)
      {
        time_out = 0;
        break;
      }
    }
    Android_Data.traffic_light_flag = 0;
    if (Android_Data.Red_State == 1)
    {
      Android_Data.Red_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 1); // 发送给交通灯标志物请求确认
    }
    else if (Android_Data.Yellow_State == 1)
    {
      Android_Data.Yellow_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 3);
    }
    else if (Android_Data.Green_State == 1)
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    else // 蒙一个
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    Run_State = 2;
    break;
  }
  case 2:
  {
    Motor_Data.xCAR_Track_Go(); // B2
    Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
    delay_ms(200);
    Identify_Two_Code_Arr[3] = 0x01;
    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
    delay_ms(500);
    delay_ms(500);
    delay_ms(500);
    YT2_parse_two_codes();

    if (Android_Data.Two_Code_State == 0) // 如果未进入识别模式
    {
      Motor_Data.xCAR_Back(20, 300);
      Rx_count = 0;
      Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
      delay_ms(500);
      delay_ms(500);
      delay_ms(500);
      Motor_Data.xCAR_Go(20, 300);
    }
    if (Android_Data.Two_Code_State == 0)
    {
      Android_Data.Two_Code_State = 1;
      printf("aaa  ");
    }
    if (Android_Data.Two_Code_State == 1)
    {
      Android_Data.Two_Code_State = 0;
      Rx_count = 0;
      YT2_parse_two_codes();

      printf("sdv\r\n");
      printf("%s\r\n", Two_Code_Data_parsed_Store1);
      if (strcmp((const char *)Two_Code_Data_parsed_Store1, "B7") == 0)
        Send_Weizhi_To_MainCar[3] = 0x01;
      else if (strcmp((const char *)Two_Code_Data_parsed_Store1, "D7") == 0)
        Send_Weizhi_To_MainCar[3] = 0x02;
      else if (strcmp((const char *)Two_Code_Data_parsed_Store1, "G6") == 0)
        Send_Weizhi_To_MainCar[3] = 0x03;
      else // F4
        Send_Weizhi_To_MainCar[3] = 0x04;

      MainCar_Data.xSend_Command_To_MainCar(Send_Weizhi_To_MainCar);
      delay_ms(100);
      MainCar_Data.xSend_Command_To_MainCar(Send_Weizhi_To_MainCar);
      delay_ms(100);
    }

    Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);

    Run_State = 3;
    break;
  }
  case 3:
  {
    SmokeTower_Data.SmokeTower_Infrared_Open();

    Run_State = 4;
    break;
  }
  case 4:
  {
    delay_ms(200);
    track_time_Start = 1;
    Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹
    delay_ms(200);
    sprintf((char *)Buf1, "Count_ms:%d\r\n ", Count_ms);
    Send_InfoData_To_Fifo((char *)Buf1, strlen((char *)Buf1));

    if (Count_ms < 600) // B2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Back(20, 600);
      delay_ms(400);
      Motor_Data.xCAR_Track(20);
      delay_ms(400);
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(500);
      Motor_Data.xCAR_Track_Go();
      Motor_Data.xCAR_Track_Go();
      delay_ms(200);
    }
    else if (1200 > Count_ms) // D2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(400);
      Motor_Data.xCAR_Track_Go();
      delay_ms(200);
    }
    else // E2
    {
      Count_ms = 0;
      Motor_Data.xCAR_Go(25, 300); // 使车身对准十字路口
      delay_ms(500);
      Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹
      delay_ms(500);
      Motor_Data.xCAR_Back(20, 600);
      delay_ms(400);
      Motor_Data.xCAR_Track(20);
      Motor_Data.xCAR_Go(30, 570); // 冲
      Motor_Data.xCAR_Go(30, 570); // 冲
      delay_ms(500);

      Motor_Data.xCAR_Track_Go();
      delay_ms(300);
    }
    Run_State = 5;
    break;
  }
  case 5:
  {
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_Track_Go(); // F4
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2); // F6
    Motor_Data.xCAR_Track_Go();                       // D6
    Motor_Data.xCAR_Track_Go();                       // B6
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 1);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_Track_Go();
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(200);
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, MainCar_Send_Cengshu);
    Motor_Data.xCAR_Back(50, 600);
    Motor_Data.xCAR_Back(35, 200);

    Run_State = 6;
    break;
  }
  case 6:
  {
    Run_State = 7;
    break;
  }
  case 7:
  {
    Run_State = 8;
    break;
  }
  case 8:
  {
    Run_State = 9;
    break;
  }
  case 9:
  {
    Run_State = 10;
    break;
  }
  case 10:
  {

    Run_State = 11;
    break;
  }
  case 11:
  {

    Run_State = 12;
    break;
  }
  case 12:
  {

    Run_State = 13;
    break;
  }
  }
}
#endif

#if YT3_2026
extern uint8_t Send_Algorithm_Parameters_To_MainCar[8];
bool lock_flag = 0; // 锁车标志，0为未锁车，1为已锁车
uint8_t time_out = 0;
void xAuto_Run_Function(void)
{
  if (lock_flag == 0)
  {
    while (Communication_Data.FollowCar_Start_flag == 0)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out > 230) // 等待10秒后超时
      {
        Run_State = 1; // 设置状态为1，进入下一阶段
        time_out = 0;  // 重置超时计数器
        break;         // 跳出循环，继续执行后续代码
      }
    }
    Run_State = 1; // 设置状态为1，进入下一阶段
    lock_flag = 1;
  }
  switch (Run_State)
  {
  case 1:
  {
    Motor_Data.xCAR_Track_Go(); // F6
    delay_ms(400);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(400);
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    delay_ms(500);
    delay_ms(500);
    // 二维码识别
    Identify_Two_Code_Arr[3] = 0x01;
    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
    delay_ms(500);
    delay_ms(500);
    delay_ms(500);
    YT3_parse_two_codes();

    if (Android_Data.Two_Code_State == 0) // 如果未进入识别模式
    {
      Motor_Data.xCAR_Back(20, 450);
      Rx_count = 0;
      delay_ms(500);
      delay_ms(500);
      Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
      delay_ms(500);
      delay_ms(500);
      delay_ms(500);
      Motor_Data.xCAR_Go(20, 450);
    }
    if (Android_Data.Two_Code_State == 0)
    {
      Android_Data.Two_Code_State = 1;
      printf("aaa  ");
    }
    if (Android_Data.Two_Code_State == 1)
    {
      Android_Data.Two_Code_State = 0;
      Rx_count = 0;
      YT3_parse_two_codes();

      printf("sdv\r\n");
      printf("%d\r\n", Two_Code_Data_parsed_Store1[0]);
      printf("%d\r\n", Two_Code_Data_parsed_Store1[1]);
      printf("%s\r\n", Two_Code_Data_parsed_Store2);
    }
    uint8_t Hex_Data_Store1[10];
    for (int i = 0; i < strlen((char *)Two_Code_Data_parsed_Store2); i++)
    {
      char c = Two_Code_Data_parsed_Store2[i];
      if (c >= '0' && c <= '9')
        Hex_Data_Store1[i] = c - '0';
      else if (c >= 'A' && c <= 'F')
        Hex_Data_Store1[i] = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
        Hex_Data_Store1[i] = c - 'a' + 10;
    }

    Send_Algorithm_Parameters_To_MainCar[3] = Hex_Data_Store1[0];
    Send_Algorithm_Parameters_To_MainCar[4] = Hex_Data_Store1[1];
    Send_Algorithm_Parameters_To_MainCar[5] = Hex_Data_Store1[2];
    MainCar_Data.xSend_Command_To_MainCar(Send_Algorithm_Parameters_To_MainCar);
    delay_ms(100);
    MainCar_Data.xSend_Command_To_MainCar(Send_Algorithm_Parameters_To_MainCar);
    delay_ms(500);
    Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
    delay_ms(400);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);

    Run_State = 2;
    break;
  }
  case 2:
  {

    Motor_Data.xCAR_Track_Go(); // F4
    delay_ms(400);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(400);
    printf("Licence_Data:%s ", MainCar_Send_Licence_Data_Store);
    // 道闸系统任务
    Barrier_Data.xBarrier_Licence_Tx(MainCar_Send_Licence_Data_Store); // 车牌号
    Motor_Data.xCAR_Track_Go();                                        // D4
    delay_ms(400);
    Motor_Data.xCAR_Track_Go(); // B4
    delay_ms(400);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    delay_ms(400);
    Motor_Data.xCAR_Track_Go(); // B2
    delay_ms(400);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(450);
    Run_State = 3;
    break;
  }
  case 3:
  {
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    delay_ms(400);
    Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add((char *)Two_Code_Data_parsed_Store2);
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
    delay_ms(400);
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(400);
    MainCar_Data.xStart_Command_To_MainCar();
    Run_State = 4;
    break;
  }
  case 4:
  {
    // 交通灯
    uint8_t time_out = 0;
    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A); // 发送请求识别红绿A
    while (Android_Data.traffic_light_flag != 1)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out >= 5)
      {
        time_out = 0;
        break;
      }
    }
    Android_Data.traffic_light_flag = 0;
    if (Android_Data.Red_State == 1)
    {
      Android_Data.Red_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 1); // 发送给交通灯标志物请求确认
    }
    else if (Android_Data.Yellow_State == 1)
    {
      Android_Data.Yellow_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 3);
    }
    else if (Android_Data.Green_State == 1)
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    else // 蒙一个
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A, 2);
    }
    Motor_Data.xCAR_Track_Go(); // D2
    delay_ms(400);
    Run_State = 5;
    break;
  }
  case 5:
  {
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 1);
    delay_ms(500);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    delay_ms(300);

    CarPort_Data.xCarPort_CarBack_Into(1);

    printf("MainCar_Send_Cengshu:%d ", MainCar_Send_Cengshu);
    uint8_t shiji_Cengshu = (Two_Code_Data_parsed_Store1[0] + Two_Code_Data_parsed_Store1[1]) * MainCar_Send_Cengshu % 4 + 1;
    printf("shiji_Cengshu:%d ", shiji_Cengshu);
    CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, shiji_Cengshu);
    Run_State = 6;
    break;
  }
  case 6:
  {
    Run_State = 7;
    break;
  }
  case 7:
  {
    Run_State = 8;
    break;
  }
  case 8:
  {
    Run_State = 9;
    break;
  }
  case 9:
  {
    Run_State = 10;
    break;
  }
  case 10:
  {

    Run_State = 11;
    break;
  }
  case 11:
  {

    Run_State = 12;
    break;
  }
  case 12:
  {

    Run_State = 13;
    break;
  }
  default:
    break;
  }
}
#endif

#if YT4_2026
void xAuto_Run_Function(void)
{

  switch (Run_State)
  {
  case 1:
  {
    Motor_Data.xCAR_Track_Go(); // B6
                                // 交通灯
    uint8_t time_out = 0;
    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B); // 发送请求识别红绿B
    while (Android_Data.traffic_light_flag != 1)
    {
      delay_ms(500);
      delay_ms(500);
      time_out++;
      if (time_out >= 5)
      {
        time_out = 0;
        break;
      }
    }
    Android_Data.traffic_light_flag = 0;
    if (Android_Data.Red_State == 1)
    {
      Android_Data.Red_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B, 1); // 发送给交通灯标志物请求确认
    }
    else if (Android_Data.Yellow_State == 1)
    {
      Android_Data.Yellow_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B, 3);
    }
    else if (Android_Data.Green_State == 1)
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B, 2);
    }
    else // 蒙一个
    {
      Android_Data.Green_State = 0;
      Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B, 2);
    }
    Motor_Data.xCAR_Track_Go(); // B4
    delay_ms(400);
    Motor_Data.xCAR_R90(wheel_Speed + 10, wheel_Time * 2);
    Run_State = 2;
    break;
  }

  case 2:
  {
    delay_ms(200);
    Motor_Data.xCAR_Track(30); // 循迹
    delay_ms(200);
    Motor_Data.xCAR_Back(20, 680);
    delay_ms(400);
    Motor_Data.xCAR_Track(30);
    delay_ms(400);
    Motor_Data.xCAR_Go(30, 570); // 冲
    Motor_Data.xCAR_Go(30, 570); // 冲
    delay_ms(500);
    Motor_Data.xCAR_Track_Go();
    delay_ms(500);
    // 二维码识别
    Identify_Two_Code_Arr[3] = 0x01;
    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
    delay_ms(500);
    delay_ms(500);
    delay_ms(500);
    YT4_parse_two_codes();

    if (Android_Data.Two_Code_State == 0) // 如果未进入识别模式
    {
      printf("1");
      Motor_Data.xCAR_Back(20, 450);
      Rx_count = 0;
      delay_ms(500);
      delay_ms(500);
      Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
      delay_ms(500);
      delay_ms(500);
      delay_ms(500);
      YT4_parse_two_codes();
      Motor_Data.xCAR_Go(20, 400);
      delay_ms(500);
    }
    if (Android_Data.Two_Code_State == 0) // 如果未进入识别模式
    {
      printf("2");
      Motor_Data.xCAR_Back(20, 300);
      Rx_count = 0;
      delay_ms(500);
      delay_ms(500);
      Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr); // 发送请求识别二维码
      delay_ms(500);
      delay_ms(500);
      delay_ms(500);
      YT4_parse_two_codes();
      Motor_Data.xCAR_Go(20, 280);
      delay_ms(500);
    }
    if (Android_Data.Two_Code_State == 0)
    {
      Android_Data.Two_Code_State = 1;
      printf("aaa  ");
    }
    if (Android_Data.Two_Code_State == 1)
    {
      Android_Data.Two_Code_State = 0;
      Rx_count = 0;
      YT4_parse_two_codes();

      printf("sdv\r\n");
      printf("%s\r\n", Two_Code_Data_parsed_Store1);
    }
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2);
    delay_ms(400);
    Motor_Data.xCAR_Track_Go(); // D2
    delay_ms(400);
    Run_State = 3;
    break;
  }
  case 3:
  {
    // 公交站随机播报
    XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
    delay_ms(500);
    Voice_Report_Data.xVoice_Report_Random_Command(); // 播报随机指令
    for (uint8_t i = 0; i < 5; i++)                   // 延时8秒等待小创识别并重复播报
    {
      delay_ms(500);
      delay_ms(500);
    }
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
    delay_ms(500);
    Motor_Data.xCAR_Track_Go(); // D2
    delay_ms(400);
    printf("%d\r\n", MainCar_Send_Cengshu);
    CarPort_Data.xCarPort_Control_Arrive_Level(1, 1);

    if (MainCar_Send_Cengshu % 2 + 1 == 1)
    {
      Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
      delay_ms(500);
      CarPort_Data.xCarPort_CarBack_Into(1);
    }
    else
    {
      Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
      delay_ms(500);
      Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
      delay_ms(500);
      Motor_Data.xCAR_Back(25, 800);
    }
    Run_State = 4;
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
  case 6:
  {
    Run_State = 7;
    break;
  }
  case 7:
  {
    Run_State = 8;
    break;
  }
  case 8:
  {
    Run_State = 9;
    break;
  }
  case 9:
  {
    Run_State = 10;
    break;
  }
  case 10:
  {

    Run_State = 11;
    break;
  }
  case 11:
  {

    Run_State = 12;
    break;
  }
  case 12:
  {

    Run_State = 13;
    break;
  }
  default:
    break;
  }
}

#endif
#if YT5_2026
#define Two_Code_Count 2

void xAuto_Run_Function(void)
{

  switch (Run_State)
  {
  case 1:
  {
   
    Run_State = 2;
    break;
  }

case 2:
{

  Run_State = 3;
  break;
}
case 3:
{
 
  Run_State = 4;
  break;
}
case 4:
{
  
  Run_State = 5;
  break;
}
case 5:
{
  CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, 1);
  Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2);
  delay_ms(300);
  CarPort_Data.xCarPort_CarBack_Into(1);
  CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A, MainCar_Send_Cengshu);
  Run_State = 6;
  break;
}
case 6:
{
  Run_State = 7;
  break;
}
case 7:
{
  Run_State = 8;
  break;
}
case 8:
{
  Run_State = 9;
  break;
}
case 9:
{
  Run_State = 10;
  break;
}
case 10:
{

  Run_State = 11;
  break;
}
case 11:
{

  Run_State = 12;
  break;
}
case 12:
{

  Run_State = 13;
  break;
}
default:
  break;
}


#endif
