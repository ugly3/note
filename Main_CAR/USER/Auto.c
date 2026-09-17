/**
************************************************************************
 *    文件名：all_module 
 *      说明：全自动运行（所用头文件+任务）
************************************************************************
**/

#include "all_module.h"

#define Test 0
#define YT1_2025 0
#define YT2_2025 0
#define YT3_2025 0
#define YT4_2025 0
#define YT5_2025 0

#define YT1_2026 0
#define YT2_2026 0
#define YT3_2026 0
#define YT4_2026 0
#define YT5_2026 0
#define  BS_2026 1
uint8_t  wheel_Speed = 90;                  // 全局转弯速度值
uint16_t wheel_Time = 330;                  // 全局转45°时间

 
/*   
   //将数据打印至Debug区域
				uint8_t Buf[50];
				sprintf((char*)Buf,"data is : %x \r\n",XiaoChuang_Rx_Byte);  
				Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));  
*/

uint8_t Run_State = 0; //运行位置状态



#if Test
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
//            RFID_Data.xRFID_Read_1(17);
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
            if(Android_Data.TFT_Car_Type_sucess_flag==1)
            {
                Android_Data.TFT_Car_Type_sucess_flag=0;
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
            if(Android_Data.TFT_Licence_Flag==1)
            {
                Android_Data.TFT_Licence_Flag=0;
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


#if  YT1_2025 

#define Card1_Block  14                    //填写卡1地址块
#define Card2_Block  card2_sector_block    //卡2地址块
#define Card2_Place  card2_position        //卡2的坐标

uint8_t timeout = 0;
uint8_t Get_Num_Cheku;
uint8_t Get_Num_LuDeng;
void xAuto_Run_Function(void)
{
    uint8_t Buf[50]={0};
    switch(Run_State)                         
	{
      case 1:
      {	
          /*
          uint8_t led_display[3]={0x00,0x00,0x00};
          LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
          LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
          delay_ms(200);
          
          */
          /*
          Ultrasonic_Ranging();
          sprintf((char*)Buf,"dis :%d \r\n",dis);  
          Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
          delay_ms(500);
          delay_ms(500);
          */
          //Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr);
        
        uint8_t two_code_arr[] = {0x55,0xFF,0x02,0x00,0x00,0x00,0x00,0xBB};
        Android_Data.xMainCar_Send_Android(two_code_arr);   //发送请求识别二维码
        delay_ms(500);
        delay_ms(500);
        if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
		{
            Rx_count = 0;
			Android_Data.xMainCar_Send_Android(two_code_arr);   //发送请求识别二维码
			delay_ms(500);
            delay_ms(500);
        }
        if(Android_Data.Two_Code_State == 0)
        {
            Android_Data.Two_Code_State = 1;
            printf("aaa  ");
        }
        if(Android_Data.Two_Code_State == 1)
        {
            Android_Data.Two_Code_State = 0;
            Rx_count = 0;
            YT1_parse_two_codes();
            printf("sdv\r\n");
            printf("%s\r\n",Two_Code_Data_parsed_Store1);
            printf("%s\r\n",Two_Code_Data_parsed_Store2);
            printf("%s\r\n",Two_Code_Data_parsed_Store3);
        }
          Run_State=0;
          break;
        
      }
      case 2:
      {
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //请求交通灯进入识别模式，并请求安卓识别红绿灯
          for(uint8_t i=0;i<3;i++) //等待安卓回传
          {
              delay_ms(500);
              delay_ms(500);
          }
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
            RFID_Data.xRFID_Track_Read_L1(25,450,Card1_Block);//对ETC前面一小段进行识卡
            ETC_Data.xETC_Pass_RFID(Card1_Block,0,0);
            Motor_Data.xCAR_Go(40,200);       //走开十字路口，防止可能重复识别十字路口的卡片
          //D4->F4
            //对特殊标志物前面一小段进行识卡
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
//            delay_ms(200);
//            RFID_Data.xRFID_Read_1(Card1_Block);
//            delay_ms(200);  
//            Motor_Data.xCAR_Go(40,200);
//            RFID_Data.xRFID_Read_1(Card1_Block);
//            delay_ms(200);

            //过特殊标志物
            Motor_Data.xCAR_Go(25,500);
            Motor_Data.xCAR_Go(25,300);
            delay_ms(500);
            //特殊标志物后一小段进行寻卡
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
            RFID_Data.xRFID_Track_Read_L1(25,1100,Card1_Block);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          }
           else if(strcmp(Card2_Place,"D3")==0)
          {   
            //D2->D3->D2
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转  
            RFID_Data.xRFID_Track_Read_L1(25,1400,Card1_Block);
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
            RFID_Data.xRFID_Track_Read_1(27,Card1_Block);//寻卡
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


#if YT2_2025

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
            RFID_Data.xRFID_Read_1(Card1_Block); //定点读一下有没有卡
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
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
            //过特殊标志物
            Motor_Data.xCAR_Go(25,500);
            Motor_Data.xCAR_Go(25,300);
            delay_ms(500);
            //特殊标志物后一小段进行寻卡
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
                 RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
                 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                 RFID_Data.xRFID_Track_Read_L1(25,1400,Card1_Block);
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
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                
                if(strcmp(Card2_Place,"E6")==0)
                {
                    //D6->E6->B6
                    Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
                    RFID_Data.xRFID_Track_Read_L1(25,1400,Card1_Block);
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
                    RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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


#if YT3_2025

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
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
            RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
            delay_ms(400);
            
            if(Count_ms<600)//特殊标志物在E2
            {
                Count_ms=0;
                //过特殊标志物
                Motor_Data.xCAR_Go(25,500);
                Motor_Data.xCAR_Go(25,300);
                delay_ms(500);
                //特殊标志物后一小段进行寻卡
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                //D2->B2
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
                Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
                delay_ms(200); 
            }
            else//特殊标志物在C2
            {
                Count_ms=0;
                Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
								delay_ms(200);
							
                 //过特殊标志物
                Motor_Data.xCAR_Go(25,500);
                Motor_Data.xCAR_Go(25,500);
								delay_ms(600);
                
                RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
//						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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



#if YT4_2025
	
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
				RFID_Data.xRFID_Read_1(Card1_Block); //定点读一下有没有卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
				delay_ms(500);
				
				if(Count_ms<600)//特殊标志物在E2  600
				{
						Count_ms=0;
						//过特殊标志物
						Motor_Data.xCAR_Go(25,550);
						Motor_Data.xCAR_Go(25,350); 
						delay_ms(500);
						//特殊标志物后一小段进行寻卡
						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
						//D2->B2
						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
						Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
						delay_ms(200); 
				}
				else//特殊标志物在C2
				{
						Count_ms=0;
						Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片 
						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
						delay_ms(200);
					
						 //过特殊标志物
						Motor_Data.xCAR_Go(25,500);
						Motor_Data.xCAR_Go(25,300);
						delay_ms(600);
						
						RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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

#if YT5_2025
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
			RFID_Data.xRFID_Read_1(Card1_Block); //定点读一下有没有卡
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
			//B4-F4
			//对特殊标志物前面一小段进行识卡
      RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//识别到特殊路段停 
			//过特殊标志物
			Motor_Data.xCAR_Go(25,500);
			Motor_Data.xCAR_Go(25,300);
			delay_ms(500);
			//特殊标志物后一小段进行寻卡
			RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡 
			
			Motor_Data.xCAR_Go(30,140);       //使车身对准十字路口
			delay_ms(200); 
			Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
			
			 //启动从车		
			for(uint8_t i = 0; i<3; i++)//启动从车
			{   
				FollowCar_Data.xStart_Command_To_FollowCar();
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
			RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
				 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				 delay_ms(300);
				Motor_Data.xCAR_R90(wheel_Speed,wheel_Time*2);
				delay_ms(300);
				RFID_Data.xRFID_Track_Read_L1(25,1400,Card1_Block);
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
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
				Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				delay_ms(300);
			}
			else
			{
				//F2-D2
				RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
				 Motor_Data.xCAR_Go(25,250);//走开十字路口，防止可能重复识别十字路口的卡片
				 delay_ms(300);
				
				if(strcmp(Card2_Place,"D3")==0)
				{
					//D2-D3-D2-B2
					Motor_Data.xCAR_L90(wheel_Speed,wheel_Time*2);
					delay_ms(300);
					RFID_Data.xRFID_Track_Read_L1(25,1400,Card1_Block);
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
					RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
					RFID_Data.xRFID_Track_Read_1(25,Card1_Block);//寻卡
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
          /*
          uint8_t led_display[3]={0x00,0x00,0x00};
          LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
          LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
//		  LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
          delay_ms(200);
          */
/*****************************************TFT*********************************/        
//		  Smart_TFT_Data.xSmart_TFT_Licence_Display(1,"A1D2E3");//多功能信息显示标志物A显示车牌A1D2E3
//        char Hex_num[3]={0x01,0x20,0x26};
//        Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,Hex_num);//TFT显示012026
//        Smart_TFT_Data.xSmart_TFT_Distance_Diaplay(1,120);//TFT直接显示120
//        Smart_TFT_Data.xSmart_TFT_Image_Up_Dowm_Auto(Smart_TFT_Data.Device_B,2);

/*****************************************测距*********************************/
//          Ultrasonic_Ranging();
//          sprintf((char*)Buf,"dis :%d \r\n",dis);  
//          Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
//          delay_ms(500);
//          delay_ms(500);
          
//          Identify_TFT_Mask_Arr[3]=0X01;
          //Android_Data.xMainCar_Send_Android(Identify_TFT_Mask_Arr);
          
          
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
////            Motor_Data.xCAR_Back(20,250);
//            Rx_count = 0;
//			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
//			delay_ms(500);
//            delay_ms(500);
//            delay_ms(500);
////            Motor_Data.xCAR_Go(20,250);
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
//            uint8_t led_display[3]={0x00,0x00,0x00};
//        LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
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
/*********************************************特殊地形随机摆放**************************************/
//        // B2->F2
//    track_time_Start = 1;
//    Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹
//    delay_ms(200);
////    sprintf((char*)Buf,"%d \r\n ",Count_ms);
////    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
////    printf("%d\n",Count_ms);
//    if (Count_ms < 600)                         // B2
//    {
//      Count_ms = 0;
//      delay_ms(500);
//      Motor_Data.xCAR_Go(30, 500); // 冲
//      Motor_Data.xCAR_Go(30, 500);
//      delay_ms(500);
//      Motor_Data.xCAR_Track_Go();
//      Motor_Data.xCAR_Track_Go();
//      delay_ms(200);
//    }
//    else if ((1600 > Count_ms) && (Count_ms >= 600)) // D2
//    {
//      Send_InfoData_To_Fifo("1\r\n", 3);
//      Count_ms = 0;
//      delay_ms(500);
//      Motor_Data.xCAR_Go(30, 500); // 冲
//      Motor_Data.xCAR_Go(30, 500);
//      Motor_Data.xCAR_Track_Go();
//      delay_ms(200);
//    }
//    else // E2
//    {
//        Send_InfoData_To_Fifo("2\r\n", 3);
//      delay_ms(500);
//      Count_ms = 0;
//      Motor_Data.xCAR_Go(30, 400); // 使车身对准十字路口
//      delay_ms(500);
//      Motor_Data.xCAR_Track(Motor_Data.Go_speed); // 循迹

//      delay_ms(500);

//      Motor_Data.xCAR_Go(30, 500);
//      Motor_Data.xCAR_Go(30, 500);
//      delay_ms(500);

//      Motor_Data.xCAR_Track_Go();
//      delay_ms(300);
//    } 
//		  
/*******************************************公交站语音文本播报**********************************/
//            uint8_t Vioce_Store[50] = "识别结果为";
//            size_t current_len = strlen((char*)Vioce_Store);
//            size_t append_len  = strlen((char*)Two_Code_Data_parsed_Store1);
//            memcpy(Vioce_Store + current_len, Two_Code_Data_parsed_Store1, append_len );
//            Voice_Report_Data.xVoice_Report_Set_Complex_Command(Vioce_Store);
            
//            Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add(Two_Code_Data_parsed_Store2);//立体显示文字
//            Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
//            Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
//            FollowCar_Data.XSend_temperature_To_fromcar();
//            Voice_Report_Data.xVoice_Report_Speak_Weather();//播报天气状况


#if  YT1_2026 
uint8_t Two_Code_Count = 3;
uint8_t Judge_Card_Block = 9;         //填写卡判别地址块
uint8_t Real_Card1_Block = 0;           //卡1有效数据地址块
uint8_t Real_Card2_Block = 0;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "RFID01";    //卡1需检验的扇区字符
char Judge_Card2_Str[] = "RFID02";     //卡2需检验的扇区字符

uint8_t timeout = 0;
uint8_t special_landform_flag=0;
uint8_t Buf[100]={0};
void xAuto_Run_Function(void)
{
    switch(Run_State)                         
	{
        case 1:
        {	//G6->F6
            uint8_t led_display[3]={0x00,0x00,0x00};
            LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(200);
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_R45(wheel_Speed,wheel_Time);
            delay_ms(100);
            Run_State=2;
            break;
        
      }
      case 2:
      {     //F6->F4
            Identify_TFT_CheXin_Arr[3]=0x01;        //选择tft设备A
//            Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
//            Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
          while(Android_Data.TFT_Licence_Flag!=1 || Android_Data.TFT_Car_Type_sucess_flag!=1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=1)    //40秒超时退出
              {
                  timeout=0;
                  break;
              }
          }
          uint8_t Hex_num[3]={0x02,0x20,0x26};
          if(Android_Data.TFT_Car_Type_sucess_flag==1)
          {
             Android_Data.TFT_Car_Type_sucess_flag=0;
              Android_Data.TFT_Licence_Flag=0;
             Hex_num[0] = TFT_Car_Type_Data_Store;
             Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num);//TFT_B显示XX2026
          }
          else
              Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num);
          
          delay_ms(100);
          Motor_Data.xCAR_L45(wheel_Speed,wheel_Time);
          Motor_Data.xCAR_Track_Go();
          delay_ms(100);
            Run_State = 3;
            break;
      }
      case 3:
      {     //F4->F2 交通灯B
           Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B);  //发送请求识别红绿灯B
            while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            delay_ms(200);
            Motor_Data.xCAR_Track_Go();
            Run_State=4;
            break;
      }
      case 4:
      {     //F2    测距
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          Motor_Data.xCAR_Back(25,800);
          Motor_Data.xCAR_Track(20);
          delay_ms(500);
          delay_ms(500);
          Ultrasonic_Ranging();
          delay_ms(500);
          delay_ms(500);
          delay_ms(500);
          printf("dis:%d ",dis);
          
          //二维码识别
        Identify_Two_Code_Arr[3] = 0x01;
        Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
        delay_ms(500);
        delay_ms(500);
        delay_ms(500);
        YT1_parse_two_codes();
          
        Motor_Data.xCAR_Go(30,300);   //前进一点点
        if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
		{
            delay_ms(500);
            delay_ms(500);
            Rx_count = 0;
			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
			delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            
        }
        if(Android_Data.Two_Code_State == 0)
        {
            Android_Data.Two_Code_State = 1;
            printf("aaa  ");
        }
        if(Android_Data.Two_Code_State == 1)
        {
            Android_Data.Two_Code_State = 0;
            Rx_count = 0;
            YT1_parse_two_codes();
            
            printf("%s\r\n",Two_Code_Data_parsed_Store1);
            printf("%s\r\n",Two_Code_Data_parsed_Store2);
            printf("%s\r\n",Two_Code_Data_parsed_Store3);
        }
        Motor_Data.xCAR_Track_Time(25,350);   //再前进一点点,车身对齐十字路口
        Real_Card1_Block = (Two_Code_Data_parsed_Store3[0]-'0'-1)*4 + Two_Code_Data_parsed_Store3[1]-'0' -1;//真实卡1的有效地址块
        Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);//左转
        delay_ms(500);
        Motor_Data.xCAR_Go(25,300);   //前进一点点
        
        //智能路灯感知调节任务
        delay_ms(100);
        Send_Init_Light_To_fromcar[3] = Smart_Light_Data.xSmart_Light_Get_Init_Level();
        FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
        delay_ms(300);
        FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);
        printf("Init_light:%d\r\n",Send_Init_Light_To_fromcar[3]);
        
        uint8_t F1 = ((dis/40) % 4)+1;
        printf("F1:%d\r\n",F1);
        Smart_Light_Data.xSmart_Light_Appoint_Level(F1);
        Motor_Data.xCAR_Back(25,300);   //倒退一点点
        Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
        delay_ms(500);
            Run_State=5;
            break;
      } 
      case 5:
      {
      //********************************************特殊地形随机摆放
          Motor_Data.xCAR_Track_Time(40,480);//定时循迹
          delay_ms(500);
          Motor_Data.xCAR_Back(20, 600);
          delay_ms(300);
//        // B2->F2
        Roadway_mp_syn();       // 码盘同步
        RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
        delay_ms(300);
        sprintf((char*)Buf,"dis_MP:%d \r\n ",dis_MP);
        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
          printf("dis_MP:%d \n",dis_MP);
        if (dis_MP <= 400)                         // 特殊地形在
        {
          dis_MP=0;
          special_landform_flag =1;
          printf("1\r\n");
          Motor_Data.xCAR_Back(20, 600);
          Motor_Data.xCAR_Track(Motor_Data.Go_speed);
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610); // 冲
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(400);
        }
        else if (1280 > dis_MP) // 特殊地形在
        {
          dis_MP=0;
          printf("2\r\n");
          special_landform_flag =2;
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610); // 冲
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(500);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(300);
        }
        else // 特殊地形在
        {
          dis_MP=0;
          printf("3\r\n");
          special_landform_flag =3;
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(20,250);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(20,250);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          
          RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//特殊标志物前
          delay_ms(400);
          Motor_Data.xCAR_Back(20, 600);
          Motor_Data.xCAR_Track(20);
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610);
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(400);
        } 

          Run_State=6;
          break;
      }
      case 6:
      {     //B2 智能语音交互任务
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
			for(uint8_t i = 0;i<5;i++)  // 延时8秒等待小创识别并重复播报
			{
                delay_ms(500);
                delay_ms(500);
			}
            //B2->B4
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Go();
            delay_ms(500);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            delay_ms(200);
          Run_State=7;
          break;
      }
      case 7:
      {    
          Identify_TFT_Traffic_Arr[3] = 0x02;       //识别图形数量和颜色
//          Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr); //识别交通标志
//          Android_Data.xMainCar_Send_Android(Identify_TFT_Traffic_Arr);
          while(Android_Data.TFT_Graph_CountAndColour_Flag!=1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=3)    //40秒超时退出
              {
                  timeout=0;
                  break;
              }
          }
          Android_Data.TFT_Graph_CountAndColour_Flag=0;
          delay_ms(100);
          uint8_t Hex_num_1[3]={TFT_GraphAndColour_Data_Store[0],TFT_GraphAndColour_Data_Store[1],TFT_GraphAndColour_Data_Store[2]};
          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num_1);//TFT第一排显示图形数量
          delay_ms(100);
          uint8_t Hex_num_2[3]={TFT_GraphAndColour_Data_Store[3],TFT_GraphAndColour_Data_Store[4],TFT_GraphAndColour_Data_Store[5]};
          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num_2);//TFT第二排显示图形数量
          delay_ms(100);
      
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(400);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(400);
          Run_State=8;
          break;
      }
      case 8:
      {   //过ETC  
          ETC_Data.xETC_Pass();
            delay_ms(300);
              Run_State=9;
          break;
      }
      case 9:
      {     
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);
          delay_ms(500);
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);
             
          Run_State=10;
          break;
      }
      case 10:
      {     
             //*********************************************启动从车
            for(uint8_t i = 0; i<3; i++)//启动从车
            {   
              FollowCar_Data.xStart_Command_To_FollowCar();
              delay_ms(100);
            }  
      
            //等待启动
            timeout=0;
           while(FollowCar_Data.FollowCar_Finish_Flag == 0)
           {
               delay_ms(500);
               delay_ms(500);
               delay_ms(200);
               timeout++;
               if(timeout>=100)
               {
                   timeout=0;
                   break;
               }
           }
           FollowCar_Data.FollowCar_Finish_Flag = 0;
          Run_State=11;
          break;
      }
      case 11:
      { //烽火台
          uint8_t plaintext[100] = {0};
          uint8_t plainHex[6] = {0};
         
          Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
          
           YT_1_Algorithm(Two_Code_Data_parsed_Store1,Two_Code_Data_parsed_Store2,(uint8_t *)READ_RFID1,plaintext,plainHex);//四方
          Infrared_Send(plainHex,6);//开启码开启
          delay_ms(200);
          Infrared_Send(plainHex,6);//开启码开启
          delay_ms(200);
          
          YT_1_Algorithm(Two_Code_Data_parsed_Store2,Two_Code_Data_parsed_Store1,(uint8_t *)READ_RFID1,plaintext,plainHex);//四方
          Infrared_Send(plainHex,6);//开启码开启
          delay_ms(200);
          Infrared_Send(plainHex,6);//开启码开启
          delay_ms(200);

          SmokeTower_Data.SmokeTower_Infrared_Open();//试着默认打开
          delay_ms(300);
          SmokeTower_Data.xSmokeTower_Zigbee_Open(plainHex);
          delay_ms(200);
          SmokeTower_Data.xSmokeTower_Zigbee_Open(plainHex);
          
          Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          Run_State=12;
          break;
      }
      case 12:
      {     //道闸
          Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);
          Motor_Data.xCAR_Track_Go();
          delay_ms(300);
          Run_State=13;
          break;
      }
      case 13:
      {    
          printf("weizhi:%02x ",Follow_Send_Weizhi);
          //入库
          if(Follow_Send_Weizhi == 0x01)//d7
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else if(Follow_Send_Weizhi == 0x02)//F7
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Go();
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else if(Follow_Send_Weizhi == 0x03)//G6
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Go();
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else//G4
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Go();
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Go();
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          
          Run_State=14;
          break;
      }
      case 14:
      {  
          for(uint8_t i=0;i<3;i++)
            printf("Wireless:%02x ",Follow_Send_Wireless_Open_Data_Store[i]);
          Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Follow_Send_Wireless_Open_Data_Store);
          delay_ms(500);
          Wireless_Charge_Data.xWireless_Charge_Open_Close(1);
          Run_State=15;
          break;
      }
      case 15:
      {  
          LED_Display_Data.xLED_Display_Time(0x00);//停止计时
          Run_State=16;
          break;
      }
      default:break;           
    }

}

#endif


#if YT2_2026
uint8_t Two_Code_Count = 3;
uint8_t Judge_Card_Block = 9;         //填写卡判别地址块，第 3 扇区第 2 个数据块
uint8_t Real_Card1_Block = 0;           //卡1有效数据地址块
uint8_t Real_Card2_Block = 0;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "CARD01";
char Judge_Card2_Str[] = "CARD02";

uint8_t Init_dangwei=0;
uint8_t timeout = 0;
uint8_t Buf[100]={0};

void xAuto_Run_Function(void)
{
  switch (Run_State)
  {
  case 1:
  {
        uint8_t led_display[3]={0x00,0x00,0x00};
        LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
        LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
        delay_ms(200);
        
        Motor_Data.xCAR_Track_Go();
        delay_ms(200);
        Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
        // ETC
        ETC_Data.xETC_Pass();
        delay_ms(300);
        
    
        Run_State = 2;
    break;
  }
  case 2:
  {
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time); // F6
    delay_ms(300);
    // 车牌与车型识别任务
      //***********************************************安卓识别**********************
      Identify_TFT_Store[2]=0x00;//识别类型
      Identify_TFT_Store[3]=0x01;//TFT设备A
      Android_Data.xMainCar_Send_Android(Identify_TFT_Store);
      delay_ms(200);
      while(Android_Data.TFT_Car_Type_sucess_flag ==0)
      {
           delay_ms(500);
           delay_ms(500);
           timeout++;
           if(timeout>=40)
           {
               break;
           }
      }
      Android_Data.TFT_Car_Type_sucess_flag = 0;
      
      uint8_t Hex_num[3] ={0x03,0x20,0x26};
      if(timeout>=40)
      {
          timeout=0;
      }
      else 
      {
          timeout=0;
          Hex_num[2] = TFT_Car_Type_Data_Store;
      }
       
      Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num);//TFT B显示
      printf("Car_Type:%s ",Hex_num);
      
      Motor_Data.xCAR_L45(wheel_Speed, wheel_Time); // F6
      delay_ms(300);
      Motor_Data.xCAR_Track_Go();
      delay_ms(200);
      Motor_Data.xCAR_Track(25);
      delay_ms(200);
          
            
    Run_State = 3;
    break;
  }
  case 3:
  {
      //**********************测距***********************
          delay_ms(500);
          delay_ms(500);
          Ultrasonic_Ranging();
          delay_ms(500);
          delay_ms(500);
          delay_ms(500);
          printf("dis:%d ",dis);
          
        //********************************二维码识别***********************
        Identify_Two_Code_Arr[3] = 0x01;
        Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
        delay_ms(500);
        delay_ms(500);
        delay_ms(500);
        YT2_parse_two_codes();
          
        Motor_Data.xCAR_Go(30,350);   //车身对齐十字路口
         if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
        {
             Motor_Data.xCAR_Back(20,300);   
            delay_ms(500);
            delay_ms(500);
            Rx_count = 0;
            Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            YT2_parse_two_codes();
            if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
            {
                 Motor_Data.xCAR_Back(25,200);   
                delay_ms(500);
                delay_ms(500);
                Rx_count = 0;
                Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                delay_ms(500);
                delay_ms(500);
                delay_ms(500);
                YT2_parse_two_codes();
                Motor_Data.xCAR_Go(25,250); 
                delay_ms(400);
            }
            Motor_Data.xCAR_Go(30,300); 
        }

        if(Android_Data.Two_Code_State == 0)
        {
            Android_Data.Two_Code_State = 1;
            printf("aaa  ");
        }
        if(Android_Data.Two_Code_State == 1)
        {
            Android_Data.Two_Code_State = 0;
            Rx_count = 0;
            YT2_parse_two_codes();
            
            printf("%s\r\n",Two_Code_Data_parsed_Store1);
            printf("%s\r\n",Two_Code_Data_parsed_Store2);
            printf("%s\r\n",Two_Code_Data_parsed_Store3);
        }
        Real_Card1_Block = (Two_Code_Data_parsed_Store3[0] -'0'-1)*4+ Two_Code_Data_parsed_Store3[1] -'0'-1;//计算卡1有效数据地址
        RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
        delay_ms(200);
        Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2); 
        delay_ms(300);
        
        //*****************************发送二维码有效数据给从车**************/
        FollowCar_Data.XSend_Text_To_fromcar(READ_RFID1);
        
        
    Run_State = 4;
    break;
  }
  case 4:
  {
      //******************************************************智能语音交互任务***************************
    XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
    delay_ms(500);
    Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
    for(uint8_t i = 0;i<6;i++)  // 延时8秒等待小创识别并重复播报
    {
        delay_ms(500);
        delay_ms(500);
    }
      
      
       Motor_Data.xCAR_Track_Go(); 
    Motor_Data.xCAR_Track_Go(); 
    delay_ms(500);

    delay_ms(500);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2); 
    delay_ms(500);

    Run_State = 5;
    break;
  }
  case 5:
  {
      //*************************************************特殊地形*************************
              Motor_Data.xCAR_Track_Time(40,480);//定时循迹
              delay_ms(500);
              Motor_Data.xCAR_Back(20, 600);
              delay_ms(300);
            
            Roadway_mp_syn();       // 码盘同步
            RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
            delay_ms(300);
            sprintf((char*)Buf,"dis_MP:%d \r\n ",dis_MP);
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
            printf("dis_MP:%d \n",dis_MP);
            if (dis_MP <= 400)                         // 特殊地形在
            {
              dis_MP=0;
              printf("E2\r\n");
              Motor_Data.xCAR_Back(20, 600);
              Motor_Data.xCAR_Track(Motor_Data.Go_speed);
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610); // 冲
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(400);
            }
            else if (1280 > dis_MP) // 特殊地形在
            {
              dis_MP=0;
              printf("D2\r\n");
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610); // 冲
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(500);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(300);
            }
            else // 特殊地形在
            {
              dis_MP=0;
              printf("C2\r\n");
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              Motor_Data.xCAR_Go(20,250);
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              Motor_Data.xCAR_Go(20,250);
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              
              RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//特殊标志物前
              delay_ms(400);
              Motor_Data.xCAR_Back(20, 600);
              Motor_Data.xCAR_Track(20);
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610);
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(400);
            } 
      
            
    Run_State = 6;
    break;
  }
  case 6:
  {
  //***********************************************************智能路灯感知调节任务**************************************
         Motor_Data.xCAR_Go(25,300);   //前进一点点
         delay_ms(500);
         delay_ms(500);
         Init_dangwei = Smart_Light_Data.xSmart_Light_Get_Init_Level();
          printf("Init:%d\r\n",Init_dangwei);  
          uint8_t Object_dangwei = ((int)pow((dis/60), (dis/60)))%4+1;//********pow为次幂运算函数
         Smart_Light_Data.xSmart_Light_Appoint_Level(Object_dangwei);
         printf("Object:%d\r\n",Object_dangwei);
          delay_ms(300);
          Send_Init_Light_To_fromcar[3] = Init_dangwei;
         FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
        delay_ms(300);
        FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
         Motor_Data.xCAR_Back(25,300);   //倒退一点点
         delay_ms(300);


      
      Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2); 

    Run_State = 7;
    break;
  }
  case 7:
  {
      //*****************************************************交通灯B***************************************
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B);  //发送请求识别红绿灯B
            timeout=0;
      while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            Motor_Data.xCAR_Track_Go();
            delay_ms(400);

    Run_State = 8;
    break;
  }
  case 8:
  {
    Motor_Data.xCAR_L90(wheel_Speed, wheel_Time * 2); // B4
    delay_ms(300);
    Motor_Data.xCAR_Track_Go();
    delay_ms(300);
    Motor_Data.xCAR_L45(wheel_Speed, wheel_Time); // D4

        //***********************************************安卓识别**********************
        Identify_TFT_Graph_CountAndColour_Arr[3]=0x02;
		Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
        Android_Data.TFT_GraphCount_Flag=0;
        Android_Data.TFT_GraphColour_Flag=0;
        timeout=0;
        while(Android_Data.TFT_GraphCount_Flag != 1 && Android_Data.TFT_GraphColour_Flag != 1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=60)    //40秒超时退出
              {
                  break;
              }
          }
          Android_Data.TFT_GraphCount_Flag = 0;
          Android_Data.TFT_GraphColour_Flag = 0;
          uint8_t Hex_num[6]={0};
          uint8_t tft_hex[3]={0xA1,0xD2,0XE3};
          if(timeout >= 60)
          {
              timeout =0;
          }
          else
          {
              string_to_hex_digits(TFT_GraphCount_Data_Store,Hex_num);
              tft_hex[0] = (Hex_num[0]<<4)|Hex_num[1];
              tft_hex[1] = (Hex_num[2]<<4)|Hex_num[3];
              tft_hex[2] = (Hex_num[4]<<4)|Hex_num[5];
          }
          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,tft_hex);//TFT显示
          printf("GraphCount:%s \r\n",TFT_GraphCount_Data_Store);
          memset(TFT_GraphCount_Data_Store, 0, sizeof(TFT_GraphCount_Data_Store));   //将接收到的数据清零
          uint8_t output[10]={0};
          transform(TFT_GraphColour_Data_Store,output);//加上固定字符F
          Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add(output);//F?F?F?F?
          printf("GraphCount:%s \r\n",TFT_GraphColour_Data_Store);
          memset(TFT_GraphColour_Data_Store, 0, sizeof(TFT_GraphColour_Data_Store));   //将接收到的数据清零  
          
          
    Run_State = 9;
    break;
  }
  case 9:
  {
    Motor_Data.xCAR_R45(wheel_Speed, wheel_Time); // D4
    delay_ms(300);
    Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2); // D4
    delay_ms(300);
     //***********************************************道闸************************
    Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);
    Motor_Data.xCAR_Track_Go();
    delay_ms(300); 
    
      
    Run_State = 10;
    break;
  }
  case 10:
  {
      //***************************************************启动从车*********************
        for(uint8_t i = 0; i<3; i++)//启动从车
        {   
          FollowCar_Data.xStart_Command_To_FollowCar();
          delay_ms(100);
        }  
  
        //等待启动
        timeout=0;
       while(FollowCar_Data.FollowCar_Finish_Flag == 0)
       {
           delay_ms(500);
           delay_ms(500);
           delay_ms(200);
           timeout++;
           if(timeout>=100)
           {
               timeout=0;
               break;
           }
       }
       FollowCar_Data.FollowCar_Finish_Flag = 0;

    Run_State = 11;
    break;
  }
  case 11:
  {
      if(Follow_Send_Weizhi == 0x01)//B7
      {
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2); 
          delay_ms(300); 
          Motor_Data.xCAR_Track_Go();
          delay_ms(300); 
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time * 2); 
          delay_ms(300); 
          Motor_Data.xCAR_Track_Time(20,900);
          delay_ms(300); //延时
          Motor_Data.xCAR_Back(20,1600);
      }
      else if(Follow_Send_Weizhi == 0x02)//D7
      {
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200); //延时
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(300); //延时
          Motor_Data.xCAR_Track_Time(20,900);
          delay_ms(300); //延时
          Motor_Data.xCAR_Back(20,1600);
      }
      else if(Follow_Send_Weizhi == 0x03)//G6
      {
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200); //延时
          ETC_Data.xETC_Pass();
          delay_ms(300);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200);
          Motor_Data.xCAR_Track_Time(20,900);
          delay_ms(300); //延时
          Motor_Data.xCAR_Back(20,1800);
      }
      else//G4
      {
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200); //延时
          ETC_Data.xETC_Pass();
          delay_ms(300);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200);
          Motor_Data.xCAR_Track_Go();
          delay_ms(300); 
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(200);
          Motor_Data.xCAR_Track_Time(20,900);
          delay_ms(300); //延时
          Motor_Data.xCAR_Back(20,1800);
      }
      delay_ms(300); //延时
      LED_Display_Data.xLED_Display_Time(0x00);//停止计时
      LED_Display_Data.xLED_Display_Time(0x00);//停止计时
      
      Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open((uint8_t *)READ_RFID1);
      delay_ms(500);
      Wireless_Charge_Data.xWireless_Charge_Open_Close(1);
      for(uint8_t i=0;i<3;i++)
            printf("Wireless:%02x ",Follow_Send_Wireless_Open_Data_Store[i]);
      
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



#if  YT3_2026 
uint8_t Two_Code_Count = 3;
uint8_t Judge_Card_Block = 5;         //填写卡判别地址块
uint8_t Real_Card1_Block = 9;           //卡1有效数据地址块
uint8_t Real_Card2_Block = 14;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "ID01";
char Judge_Card2_Str[] = "ID02";

uint8_t Init_dangwei=0;
uint8_t timeout = 0;
uint8_t special_landform_flag=0;

void xAuto_Run_Function(void)
{
    uint8_t Buf[100]={0};
    switch(Run_State)                         
	{
        case 1:
        {	//G6->F6
            uint8_t led_display[3]={0x00,0x00,0x00};
            LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(200);
            Motor_Data.xCAR_Track_Go();
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Time(30,850);
            delay_ms(300);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(500);
            delay_ms(500);
            Run_State=2;
            break;
      }
      case 2:
      {     Ultrasonic_Ranging();
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            dis-=120;
            printf("dis:%d ",dis);
            
            Run_State = 3;
            break;
      }
      case 3:
      {     
              //二维码识别
        Identify_Two_Code_Arr[3] = 0x01;
        Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
        delay_ms(500);
        delay_ms(500);
        delay_ms(500);
        YT3_parse_two_codes();
          
        if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
		{   
            Motor_Data.xCAR_Back(30,300);
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Rx_count = 0;
			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
			delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Motor_Data.xCAR_Go(30,300); 
        }
        if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
		{   
            Motor_Data.xCAR_Back(30,200);
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Rx_count = 0;
			Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
			delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Motor_Data.xCAR_Go(30,200); 
        }
        if(Android_Data.Two_Code_State == 0)
        {
            Android_Data.Two_Code_State = 1;
            printf("aaa  ");
        }
        if(Android_Data.Two_Code_State == 1)
        {
            Android_Data.Two_Code_State = 0;
            Rx_count = 0;
            YT3_parse_two_codes();
            
            printf("%s\r\n",Two_Code_Data_parsed_Store1);
            printf("%s\r\n",Two_Code_Data_parsed_Store2);
            printf("%s\r\n",Two_Code_Data_parsed_Store3);
        }
            delay_ms(500);
            delay_ms(500);
            Run_State=4;
            break;
      }
      case 4:
      {     //公交站播报
            uint8_t Vioce_Store[50] = "识别结果为";
            size_t current_len = strlen((char*)Vioce_Store);
            size_t append_len  = strlen((char*)Two_Code_Data_parsed_Store1);
            memcpy(Vioce_Store + current_len, Two_Code_Data_parsed_Store1, append_len );
            Voice_Report_Data.xVoice_Report_Set_Complex_Command(Vioce_Store);
            delay_ms(500);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            Motor_Data.xCAR_Track_Go();
            delay_ms(500);
          
            Run_State=5;
            break;
      } 
      case 5:
      {
          //安卓识别图形数量及颜色
           Identify_TFT_Graph_CountAndColour_Arr[3]=0x02;
//           Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);
//           delay_ms(500);
//           Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);
            delay_ms(500);
          while(Android_Data.TFT_Graph_CountAndColour_Flag!=1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=3)    //40秒超时退出
              {
                  timeout=0;
                  break;
              }
          }
          Android_Data.TFT_Graph_CountAndColour_Flag=0;
          delay_ms(100);
          uint8_t Display_Data[3]={0};
          Display_Data[0] = TFT_GraphCount_Data_Store[0]<<4|TFT_GraphCount_Data_Store[1];
          Display_Data[1] = TFT_GraphCount_Data_Store[2]<<4|TFT_GraphCount_Data_Store[3];
          Display_Data[2] = TFT_GraphCount_Data_Store[4]<<4|TFT_GraphCount_Data_Store[5];
          LED_Display_Data.xLED_Display_Data(Display_Data,2);//LED显示标志物第二排显示
          
          uint8_t Hex_num_1[3]={TFT_GraphColour_Data_Store[0],TFT_GraphColour_Data_Store[1],TFT_GraphColour_Data_Store[2]};
          Hex_num_1[0]=0xF0|TFT_GraphColour_Data_Store[0];
          Hex_num_1[1]=0xF0|TFT_GraphColour_Data_Store[1];
          Hex_num_1[2]=0xF0|TFT_GraphColour_Data_Store[2];
          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,Hex_num_1);//TFT第一排显示图形数量
          delay_ms(500);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(500);
          delay_ms(500);
          Run_State=6;
          break;
      }
      case 6:
      {     
           /*********************************************特殊地形随机摆放**************************************/
          Motor_Data.xCAR_Track_Time(40,480);//定时循迹
          delay_ms(500);
          Motor_Data.xCAR_Back(20, 600);
          delay_ms(300);
//        // B2->F2
        Roadway_mp_syn();       // 码盘同步
        RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
        delay_ms(300);
        sprintf((char*)Buf,"dis_MP:%d \r\n ",dis_MP);
        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
          printf("dis_MP:%d \n",dis_MP);
        if (dis_MP <= 400)                         // 特殊地形在
        {
          dis_MP=0;
          special_landform_flag =1;
          printf("1\r\n");
          Motor_Data.xCAR_Back(20, 600);
          Motor_Data.xCAR_Track(Motor_Data.Go_speed);
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610); // 冲
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(400);
        }
        else if (1280 > dis_MP) // 特殊地形在
        {
          dis_MP=0;
          printf("2\r\n");
          special_landform_flag =2;
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610); // 冲
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(500);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(300);
        }
        else // 特殊地形在
        {
          dis_MP=0;
          printf("3\r\n");
          special_landform_flag =3;
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(20,250);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(20,250);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          
          RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//特殊标志物前
          delay_ms(400);
          Motor_Data.xCAR_Back(20, 600);
          Motor_Data.xCAR_Track(20);
          delay_ms(400);
          Motor_Data.xCAR_Go(30, 610);
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(400);
          RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          delay_ms(400);
        } 
          
          Run_State=7;
          break;
      }
      case 7:
      {    
      //智能路灯感知调节任务
         Motor_Data.xCAR_Go(25,300);   //前进一点点
         delay_ms(500);
         delay_ms(500);
         Init_dangwei = Smart_Light_Data.xSmart_Light_Get_Init_Level();
          printf("Init:%d\r\n",Init_dangwei);  
         uint8_t Object_dangwei = (Init_dangwei * Init_dangwei+3*dis)%4 +1;
         Smart_Light_Data.xSmart_Light_Appoint_Level(Object_dangwei);
         printf("Object:%d\r\n",Object_dangwei);
          delay_ms(300);
          Send_Init_Light_To_fromcar[3] = Init_dangwei;
         FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
        delay_ms(300);
        FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
         Motor_Data.xCAR_Back(25,300);   //倒退一点点
         delay_ms(500);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(500);
          
          Run_State=8;
          break;
      }
      case 8:
      {  
          //交通灯B
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B);  //发送请求识别红绿灯B
            while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            delay_ms(500);
            RFID_Data.xRFID_Track_Read_2(15,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
            
           Run_State=9;
          break;
      }
      case 9:
      {     
          delay_ms(500);
          Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
          delay_ms(500);
          
          //安卓识别车牌车型
          Identify_TFT_CheXin_Arr[3]=0x01;
//          Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
//           delay_ms(500);
//           Android_Data.xMainCar_Send_Android(Identify_TFT_CheXin_Arr);
            delay_ms(500);
          while(Android_Data.TFT_Licence_Flag!=1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=3)    //40秒超时退出
              {
                  timeout=0;
                  break;
              }
          }
          Android_Data.TFT_Licence_Flag=0;
          delay_ms(500);
          Smart_TFT_Data.xSmart_TFT_Licence_Display(1,(char *)TFT_License_Data_Store);//多功能信息显示标志物（A）显示车牌格式
          delay_ms(500);
          Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
          delay_ms(500);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          
          Run_State=10;
          break;
      }
      case 10:
      {     
          FollowCar_Data.XSend_Licence_To_fromcar((char *)TFT_License_Data_Store);//发送车牌给从车
          delay_ms(300);
            //道闸
          Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);
          Motor_Data.xCAR_Track_Go();
          delay_ms(500);
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(500);
          Motor_Data.xCAR_Track_Time(30,880);//定时循迹,避免与从车相撞
          delay_ms(500);
          
            //启动从车
            for(uint8_t i = 0; i<3; i++)//启动从车
            {   
              FollowCar_Data.xStart_Command_To_FollowCar();
              delay_ms(100);
            }  
          
          Run_State=11;
          break;
      }
      case 11:
      { 
           //等待启动
            timeout=0;
           while(FollowCar_Data.FollowCar_Finish_Flag == 0)//2分钟
           {
               delay_ms(500);
               delay_ms(500);
               delay_ms(200);
               timeout++;
               if(timeout>=100)
               {
                   timeout=0;
                   break;
               }
           }
           FollowCar_Data.FollowCar_Finish_Flag = 0;
          Motor_Data.xCAR_Back(30,800);
          Run_State=12;
          break;
      }
      case 12:
      {   
          for(uint8_t i=0;i<20;i++)
          {
              delay_ms(500);
              delay_ms(500);
          }
          Motor_Data.xCAR_Track_Go();
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(300);
          Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
          delay_ms(300);
          
          printf("Follow_Algorithm:");//2 3 3
          for(uint8_t i=0;i<3;i++)
            printf(":%d ",Follow_Send_Algorithm_Data_Store[i]);
          //算法处理（通用一维Valid卷积函数）
          uint8_t kernel_size = sizeof(Follow_Send_Algorithm_Data_Store) / sizeof(Follow_Send_Algorithm_Data_Store[0]);//卷积核大小
          uint8_t result[6] = {0};
          YT_3_Algorithm(READ_RFID2, Follow_Send_Algorithm_Data_Store, kernel_size, result, 6);
          printf("Smoke:");
          for(uint8_t i=0;i<6;i++)
            printf("%d ",result[i]);  //0x0F 0x1E 0x22 0x2D 0x2C 0x20
          /*********************************************************************报警台**************************************/
          Infrared_Send(result,6);//开启码开启
          delay_ms(200);
          Infrared_Send(result,6);//开启码开启
          delay_ms(200);
          SmokeTower_Data.SmokeTower_Infrared_Open();//试着默认打开
          delay_ms(300);
          
          SmokeTower_Data.xSmokeTower_Zigbee_Open(result);
          delay_ms(200);
          SmokeTower_Data.xSmokeTower_Zigbee_Open(result);
            
          Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
          delay_ms(300);
          Run_State=13;
          break;
      }
      case 13:
      {    
         ETC_Data.xETC_Pass();
          delay_ms(300);
          
          uint8_t sum=0;
          for(uint8_t i=0;i<4;i++)
          {
              sum += READ_RFID1[i];
          }
          if(sum%2==1)//奇数 F1
          {
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(300); //延时
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else //G2
          {
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(300); //延时
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(300); //延时
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1400);
          }
          LED_Display_Data.xLED_Display_Time(0x00);//停止计时
          Wireless_Charge_Data.xWireless_Charge_Open_Close(1);
          Run_State=14; 
          
          break;
      }
      case 14:
      {  
          Run_State=15;
          break;
      }
      case 15:
      {  
          Run_State=16;
          break;
      }
      default:break;           
    }

}

#endif

#if YT4_2026
uint8_t Two_Code_Count = 2;             //二维码数量
uint8_t Judge_Card_Block = 16;         //填写卡判别地址块，4 扇区第 1 数据块
uint8_t Real_Card1_Block = 10;           //卡1有效数据地址块，3 扇区中第 2 个数据块
uint8_t Real_Card2_Block = 0;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "ID01";
char Judge_Card2_Str[] = "ID02";

uint8_t timeout = 0;
uint8_t Init_dangwei = 0;
uint8_t Cengshu = 0;
void xAuto_Run_Function(void)
{
    uint8_t Buf[100]={0};
    switch(Run_State)                         
	{
        case 1:
        {	
            uint8_t led_display[3]={0x00,0x00,0x00};
            LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
          delay_ms(200);
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(200);
            CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,1);
            Motor_Data.xCAR_Go(30,350);
            delay_ms(300);
            Motor_Data.xCAR_Track_Go();
            delay_ms(300);
            Motor_Data.xCAR_Track_Go();
            delay_ms(300);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            delay_ms(300);
            //********************************************************超声波测距
            Motor_Data.xCAR_Back(15,1000);
            Motor_Data.xCAR_Track(30);
            delay_ms(500);
            delay_ms(500);
            Ultrasonic_Ranging();
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            printf("dis:%d ",dis);
            LED_Display_Data.xLED_Display_Distance(dis);
          
          Motor_Data.xCAR_Go(30,350);   //前进一点点
          delay_ms(300);
          Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
          delay_ms(300);
        
          //***********************************************************二维码识别
            Identify_Two_Code_Arr[3] = 0x01;
            Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            YT4_parse_two_codes();
            
            if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
            {
                 Motor_Data.xCAR_Back(20,300);   
                delay_ms(500);
                delay_ms(500);
                Rx_count = 0;
                Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                delay_ms(500);
                delay_ms(500);
                delay_ms(500);
                YT4_parse_two_codes();
                if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
                {
                     Motor_Data.xCAR_Back(25,200);   
                    delay_ms(500);
                    delay_ms(500);
                    Rx_count = 0;
                    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                    delay_ms(500);
                    delay_ms(500);
                    delay_ms(500);
                    YT4_parse_two_codes();
                    Motor_Data.xCAR_Go(25,250); 
                    delay_ms(400);
                }
                Motor_Data.xCAR_Go(30,350); 
            }
            delay_ms(300);
            if(Android_Data.Two_Code_State == 0)
            {
                Android_Data.Two_Code_State = 1;
                printf("aaa  ");
            }
            if(Android_Data.Two_Code_State == 1)
            {
                Android_Data.Two_Code_State = 0;
                Rx_count = 0;
                YT4_parse_two_codes();
                
                printf("%s\r\n",Two_Code_Data_parsed_Store1);
                printf("%s\r\n",Two_Code_Data_parsed_Store2);
//                printf("%s\r\n",Two_Code_Data_parsed_Store3);
            }
            Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
            delay_ms(300);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(300);
            Motor_Data.xCAR_Track_Go();
            
            RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
            delay_ms(300);
            Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
            delay_ms(300);
            Run_State=2;
            break;
      }
      case 2:
      {     
          //********************识别图形数量和颜色（有效图片为包含直行交通标识的图片）
        Identify_TFT_Graph_CountAndColour_Arr[3]=0x01;
		Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
          while(Android_Data.TFT_GraphCount_Flag != 1 && Android_Data.TFT_GraphColour_Flag != 1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=50)    //40秒超时退出
              {
//                  timeout=0;
                  break;
              }
          }
          Android_Data.TFT_GraphCount_Flag = 0;
          Android_Data.TFT_GraphColour_Flag = 0;
          uint8_t Hex_num[6]={0};
          uint8_t tft_hex[3]={0xB2,0xD3,0xE4};
          if(timeout>=50)
          {
              timeout=0;
          }
          else
          {
              string_to_hex_digits(TFT_GraphCount_Data_Store,Hex_num);
              tft_hex[0] = (Hex_num[0]<<4)|Hex_num[1];
              tft_hex[1] = (Hex_num[2]<<4)|Hex_num[3];
              tft_hex[2] = (Hex_num[4]<<4)|Hex_num[5];
          }

          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,tft_hex);//TFT显示
          printf("GraphCount:%s ",TFT_GraphCount_Data_Store);
          memset(TFT_GraphCount_Data_Store, 0, sizeof(TFT_GraphCount_Data_Store));   //将接收到的数据清零
          
          Motor_Data.xCAR_R45(wheel_Speed+10, wheel_Time);
          delay_ms(300);
          Run_State = 3;
            break;
      }
      case 3:
      {     
           //交通灯A
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯A
            while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
            }
            delay_ms(300);
            RFID_Data.xRFID_Track_Read_2(15,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
            delay_ms(300);
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(300);
            
            Run_State=4;
            break;
      }
      case 4:
      {    //******************************智能路灯
          Motor_Data.xCAR_Go(25,400);   //往前一点点
          Init_dangwei= Smart_Light_Data.xSmart_Light_Get_Init_Level();//获得初始挡位
          printf("Init:%d\r\n",Init_dangwei);  
          
          uint8_t Object_dangwei = (Init_dangwei * Init_dangwei * Init_dangwei+3)%4 +1;//目标挡位
          Smart_Light_Data.xSmart_Light_Appoint_Level(Object_dangwei);//执行目标挡位
          printf("Object:%d\r\n",Object_dangwei); 
          
          Send_Init_Light_To_fromcar[3] = Init_dangwei;
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
          delay_ms(300);
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
          Motor_Data.xCAR_Back(25,400);  //往后一点点
          delay_ms(300);
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(300);
          
           /******************************************************报警台**************************************/
          uint8_t output[10] = {0};
          YT_4_Algorithm(Two_Code_Data_parsed_Store1, Two_Code_Data_parsed_Store2, output, sizeof(output));//数据处理
          for(uint8_t i=0;i<6;i++)
          {
              printf("kaiqima :%02x ",output[i]);
          }
          Infrared_Send(output,6);//开启码开启
          delay_ms(200);
          Infrared_Send(output,6);//开启码开启
          delay_ms(200);
          SmokeTower_Data.SmokeTower_Infrared_Open();//试着默认打开
          delay_ms(200);
          SmokeTower_Data.xSmokeTower_Zigbee_Open(output);
          delay_ms(200);
          SmokeTower_Data.xSmokeTower_Zigbee_Open(output);
          delay_ms(200);
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(300);
          RFID_Data.xRFID_Track_Read_2(15,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
          delay_ms(300);
          Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
          delay_ms(300);
          
          Run_State=6;
          break;
      }
      case 6:
      {     
          Cengshu = atoi(READ_RFID1)%4+1;//atoi将字符"2894"变成十进制2894
          Send_RFID_Data_To_fromcar[3]=Cengshu;
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_RFID_Data_To_fromcar);//发给车库层数
          delay_ms(300);
          FollowCar_Data.xSend_Command_TO_FollowCar(Send_RFID_Data_To_fromcar);//发给车库层数
          delay_ms(300);
          //********************************特殊地形****************************/
           Motor_Data.xCAR_Track(30);
          delay_ms(300);
          Motor_Data.xCAR_Back(20,500);
          delay_ms(300);
          Motor_Data.xCAR_Track(20);
          delay_ms(300);
          Motor_Data.xCAR_Go(30, 610);
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          delay_ms(500);
          delay_ms(500);
          
          Run_State=7;
          break;
      }
      case 7:
      {    
        //识别车牌
          Identify_TFT_Store[2]=0x10;//***************需更改
          Identify_TFT_Store[3]=0x02;
          Android_Data.TFT_Licence_Flag =0;
          Android_Data.xMainCar_Send_Android(Identify_TFT_Store);
          delay_ms(100);
          Android_Data.xMainCar_Send_Android(Identify_TFT_Store);
          while(Android_Data.TFT_Licence_Flag!=1)
          {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=50)//***********更改时间
                {
                    timeout=0;
                    break;
                }
          }
          Android_Data.TFT_Licence_Flag = 0;
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(300);
          
          Run_State=8;
          break;
      }
      case 8:
      {  //*************道闸
          printf("License_Data:%s ",TFT_License_Data_Store);
          Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);
          Motor_Data.xCAR_Track_Go();
          delay_ms(300); 
          Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
          delay_ms(300); 
          Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
          delay_ms(300); 
          //*********************************立体显示  B2
          uint8_t output[10]={0};
          transform(TFT_GraphColour_Data_Store,output);
          Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add(output);//F?F?F?F? 
          printf("GraphCount:%s ",TFT_GraphColour_Data_Store);
          delay_ms(500);
          delay_ms(500);           
          memset(TFT_GraphColour_Data_Store, 0, sizeof(TFT_GraphColour_Data_Store));   //将接收到的数据清零
            delay_ms(300); 
          Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
          delay_ms(300); 
          
           Run_State=9;
          break;
      }
      case 9:
      {     
            //********启动从车
            for(uint8_t i = 0; i<3; i++)//启动从车
            {   
              FollowCar_Data.xStart_Command_To_FollowCar();
              delay_ms(100);
            }  
      
            //等待启动
            timeout=0;
           while(FollowCar_Data.FollowCar_Finish_Flag == 0)
           {
               delay_ms(500);
               delay_ms(500);
               delay_ms(200);
               timeout++;
               if(timeout>=100)
               {
                   timeout=0;
                   break;
               }
           }
           FollowCar_Data.FollowCar_Finish_Flag = 0;
        
          
          Run_State=10;
          break;
      }
      case 10:
      {     
          ETC_Data.xETC_Pass();
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          delay_ms(300); 
          if(Init_dangwei%2 == 1)   //进立体车库
          {
              CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,1);
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(300); 
              CarPort_Data.xCarPort_CarBack_Into(CarPort_Data.Device_A);
              Cengshu = atoi(READ_RFID1)%4+1;
              CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,Cengshu);
          }
          else
          {
                Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
                delay_ms(300); 
                Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
                delay_ms(300); 
                 Motor_Data.xCAR_Track_Time(20,900);
                delay_ms(300); //延时
                Motor_Data.xCAR_Back(20,1400);

          }
           LED_Display_Data.xLED_Display_Time(0x00);//停止计时
            delay_ms(200);
           Wireless_Charge_Data.xWireless_Charge_Open_Close(1);
          
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



#if YT5_2026
uint8_t Two_Code_Count = 3;             //二维码数量
uint8_t Judge_Card_Block = 9;          //填写卡判别地址块，第 3 扇区第 2 个数据
uint8_t Real_Card1_Block = 0;           //卡1有效数据地址块
uint8_t Real_Card2_Block = 0;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "CARD01";  //有效卡
char Judge_Card2_Str[] = "0";       //无效卡

uint8_t timeout = 0;
uint8_t Init_dangwei = 0; 
uint8_t Cengshu = 0;
void xAuto_Run_Function(void)
{
    uint8_t Buf[100]={0};
    switch(Run_State)                         
	{
        case 1:
        {	
            uint8_t led_display[3]={0x00,0x00,0x00};
            LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(200);
            Motor_Data.xCAR_Track_Go();
            delay_ms(300);
            Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
            delay_ms(400);
            Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
            delay_ms(400);
            
            Run_State=2;
            break;
          }
          case 2:
          {     
              //***********************************************安卓识别**********************
              Identify_TFT_Store[2]=0x00;//识别类型
              Identify_TFT_Store[3]=0x01;//TFT设备A
              Android_Data.xMainCar_Send_Android(Identify_TFT_Store);
              delay_ms(200);
              while(Android_Data.TFT_Car_Type_sucess_flag!=1 && Android_Data.TFT_Licence_Flag!=1)
              {
                  delay_ms(500);
                   delay_ms(500);
                   delay_ms(200);
                   timeout++;
                   if(timeout>=3)
                   {
                       timeout=0;
                       break;
                   }
              }
              Android_Data.TFT_Car_Type_sucess_flag = 0;
              Android_Data.TFT_Licence_Flag = 0;
              uint8_t Hex_num[3]={0x20,0x23,0x01};
              Hex_num[2] = TFT_Car_Type_Data_Store;
              Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,Hex_num);//TFT显示2023XX
              
              Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
              delay_ms(400);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              
              Run_State = 3;
                break;
          }
          case 3:
          {     
              //*****************************************************交通灯B***************************************
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_B);  //发送请求识别红绿灯B
            while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_B,2);
            }
            Motor_Data.xCAR_Track_Go();
            delay_ms(400);
          
                Run_State=4;
                break;
          }
          case 4:
          {    
              //******************************************************智能语音交互任务***************************
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(400);
              //B2 智能语音交互任务
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
			for(uint8_t i = 0;i<5;i++)  // 延时8秒等待小创识别并重复播报
			{
                delay_ms(500);
                delay_ms(500);
			}
             Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
             delay_ms(400);
            //*******************************************************超声波测距*************************************
            Motor_Data.xCAR_Back(25,800);
            delay_ms(300);
            Motor_Data.xCAR_Track(30);
             delay_ms(500);
             delay_ms(500);
             Ultrasonic_Ranging();
             delay_ms(500);
             delay_ms(500);
             delay_ms(500);
             printf("dis:%d ",dis);
              
            //********************************************************二维码识别***********************************
            Identify_Two_Code_Arr[3] = 0x01;
            Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            YT5_parse_two_codes();
            
            Motor_Data.xCAR_Go(30,400);   //车身对其十字路口
            delay_ms(400);
            Real_Card1_Block = (Two_Code_Data_parsed_Store3[0] -'0'-1)*4+ Two_Code_Data_parsed_Store3[1] -'0'-1;//计算卡1有效数据地址
            RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
            delay_ms(300);
            if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
            {
                Motor_Data.xCAR_Back(25,200); 
                delay_ms(500);
                delay_ms(500);
                Rx_count = 0;
                Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                delay_ms(500);
                delay_ms(500);
                delay_ms(500);
                YT5_parse_two_codes();
                Real_Card1_Block = (Two_Code_Data_parsed_Store3[0] -'0'-1)*4+ Two_Code_Data_parsed_Store3[1] -'0'-1;//计算卡1有效数据地址
                RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
                delay_ms(300);
                if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
                {
                     Motor_Data.xCAR_Back(25,200);   
                    delay_ms(500);
                    delay_ms(500);
                    Rx_count = 0;
                    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                    delay_ms(500);
                    delay_ms(500);
                    delay_ms(500);
                    YT5_parse_two_codes();
                    Motor_Data.xCAR_Go(25,200); 
                    delay_ms(400);
                    Real_Card1_Block = (Two_Code_Data_parsed_Store3[0] -'0'-1)*4+ Two_Code_Data_parsed_Store3[1] -'0'-1;//计算卡1有效数据地址
                    RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
                    delay_ms(300);
                }
                Motor_Data.xCAR_Go(25,200); 
            }
            delay_ms(400);
            Real_Card1_Block = (Two_Code_Data_parsed_Store3[0] -'0'-1)*4+ Two_Code_Data_parsed_Store3[1] -'0'-1;//计算卡1有效数据地址
            RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//原地寻一下
            if(Android_Data.Two_Code_State == 0)
            {
                Android_Data.Two_Code_State = 1;
                printf("aaa  ");
            }
            if(Android_Data.Two_Code_State == 1)
            {
                Android_Data.Two_Code_State = 0;
                Rx_count = 0;
                YT5_parse_two_codes();
                
                printf("%s\r\n",Two_Code_Data_parsed_Store1);
                printf("%s\r\n",Two_Code_Data_parsed_Store2);
                printf("%s\r\n",Two_Code_Data_parsed_Store3);
            }
    
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(400);
            
              Run_State=6;
              break;
          }
          case 6:
          {     
              //*************************************************特殊地形*************************
              Motor_Data.xCAR_Track_Time(40,480);//定时循迹
              delay_ms(500);
              Motor_Data.xCAR_Back(20, 600);
              delay_ms(300);
            // B2->F2
            Roadway_mp_syn();       // 码盘同步
            RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
            delay_ms(300);
            sprintf((char*)Buf,"dis_MP:%d \r\n ",dis_MP);
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
            printf("dis_MP:%d \n",dis_MP);
            if (dis_MP <= 400)                         // 特殊地形在
            {
              dis_MP=0;
              printf("E2\r\n");
              Motor_Data.xCAR_Back(20, 600);
              Motor_Data.xCAR_Track(Motor_Data.Go_speed);
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610); // 冲
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//寻一整段
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(400);
            }
            else if (1280 > dis_MP) // 特殊地形在
            {
              dis_MP=0;
              printf("D2\r\n");
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610); // 冲
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(500);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(300);
            }
            else // 特殊地形在
            {
              dis_MP=0;
              printf("C2\r\n");
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              Motor_Data.xCAR_Go(20,250);
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              Motor_Data.xCAR_Go(20,250);
              RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              
              RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);//特殊标志物前
              delay_ms(400);
              Motor_Data.xCAR_Back(20, 600);
              Motor_Data.xCAR_Track(20);
              delay_ms(400);
              Motor_Data.xCAR_Go(30, 610);
              Motor_Data.xCAR_Go(30, 610);
              delay_ms(400);
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
              delay_ms(400);
            } 
                  
              Run_State=7;
              break;
          }
          case 7:
          {    
              //B2
              Motor_Data.xCAR_Go(25,300);   //前进一点点
            //*********************************************智能路灯感知调节任务********************
             delay_ms(500);
             delay_ms(500);
             Init_dangwei = Smart_Light_Data.xSmart_Light_Get_Init_Level();
              printf("Init:%d\r\n",Init_dangwei);  
              
              //发给从车路灯初始挡位
             Send_Init_Light_To_fromcar[3] = Init_dangwei;
             FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
             delay_ms(300);
             FollowCar_Data.xSend_Command_TO_FollowCar(Send_Init_Light_To_fromcar);//发给从车路灯初始挡位
              
             uint8_t Object_dangwei = ((int)pow((dis/60), (dis/60)))%4+1;//********pow为次幂运算函数
             Smart_Light_Data.xSmart_Light_Appoint_Level(Object_dangwei);
              printf("Object:%d\r\n",Object_dangwei);
              delay_ms(300);
              
              Motor_Data.xCAR_Back(25,300);
              delay_ms(400);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(400);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(400);
              
              Run_State=8;
              break;
          }
          case 8:
          {
              //******************************************* ETC **********************
              ETC_Data.xETC_Pass();
              delay_ms(300);
              
               Run_State=9;
              break;
          }
          case 9:
          {     
               Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
               delay_ms(400);
                //***********************************************安卓识别**********************
              Identify_TFT_Store[2]=0x00;//识别类型
              Identify_TFT_Store[3]=0x02;//TFT设备A
              Android_Data.xMainCar_Send_Android(Identify_TFT_Store);
              delay_ms(200);
              while(Android_Data.TFT_GraphCount_Flag != 1 && Android_Data.TFT_GraphColour_Flag != 1)
              {
                   delay_ms(500);
                   delay_ms(500);
                   timeout++;
                   if(timeout>=3)
                   {
                       timeout=0;
                       break;
                   }
              }
              Android_Data.TFT_GraphCount_Flag = 0;
              Android_Data.TFT_GraphColour_Flag = 0;
              uint8_t Hex_num[6]={0};
              string_to_hex_digits(TFT_GraphCount_Data_Store,Hex_num);
              uint8_t tft_hex[3]={0};
              tft_hex[0] = (Hex_num[0]<<4)|Hex_num[1];
              tft_hex[1] = (Hex_num[2]<<4)|Hex_num[3];
              tft_hex[2] = (Hex_num[4]<<4)|Hex_num[5];
              Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,tft_hex);//TFT显示
              printf("GraphCount:%s \r\n",TFT_GraphCount_Data_Store);
               memset(TFT_GraphCount_Data_Store, 0, sizeof(TFT_GraphCount_Data_Store));   //将接收到的数据清零

              uint8_t output[10]={0};
              transform(TFT_GraphColour_Data_Store,output);
              Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add(output);//F?F?F?F?
              printf("GraphCount:%s \r\n",TFT_GraphColour_Data_Store);
              memset(TFT_GraphColour_Data_Store, 0, sizeof(TFT_GraphColour_Data_Store));   //将接收到的数据清零

              Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
              delay_ms(400);
               Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);
              delay_ms(400);
              
              Run_State=10;
              break;
          }
          case 10:
          {     
              //***************************************************启动从车*********************
                for(uint8_t i = 0; i<3; i++)//启动从车
                {   
                  FollowCar_Data.xStart_Command_To_FollowCar();
                  delay_ms(100);
                }  
          
                //等待启动
                timeout=0;
               while(FollowCar_Data.FollowCar_Finish_Flag == 0)
               {
                   delay_ms(500);
                   delay_ms(500);
                   delay_ms(200);
                   timeout++;
                   if(timeout>=100)
                   {
                       timeout=0;
                       break;
                   }
               }
               FollowCar_Data.FollowCar_Finish_Flag = 0;
              
              
              Run_State=11;
              break;
          }
          case 11:
          { 
               //***********************************************道闸************************
              Barrier_Data.xBarrier_Licence_Tx((char *)TFT_License_Data_Store);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300); 
              
              Run_State=12;
              break;
          }
          case 12:
          {   
            printf("weizhi:%02x ",Follow_Send_Weizhi);
          //入库
          if(Follow_Send_Weizhi == 0x01)//d7
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else if(Follow_Send_Weizhi == 0x02)//F7
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else if(Follow_Send_Weizhi == 0x03)//G6
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
          else//G4
          {
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Go();
              delay_ms(300);
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Time(20,900);
              delay_ms(300); //延时
              Motor_Data.xCAR_Back(20,1600);
          }
            LED_Display_Data.xLED_Display_Time(0x00);//停止计时
            Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open((uint8_t *)READ_RFID1); 
              Run_State=13;
              break;
          }
          case 13:
          {    
              
              Run_State=14; 
              
              break;
          }
          case 14:
          {  
              Run_State=15;
              break;
          }
          case 15:
          {  
              Run_State=16;
              break;
          }
          default:break;           
    }

}
#endif






#if BS_2026
uint8_t Two_Code_Count = 2;             //二维码数量
uint8_t Judge_Card_Block = 9;          //填写卡判别地址块，第 3 扇区第 2 个数据
uint8_t Real_Card1_Block = 0;           //卡1有效数据地址块
uint8_t Real_Card2_Block = 0;           //卡2有效数据地址块
uint8_t Real_Card3_Block = 0;           //卡3有效数据地址块
//#define Card2_Place  card2_position        //卡2的坐标
char Judge_Card1_Str[] = "0";  //有效卡
char Judge_Card2_Str[] = "0";       //无效卡

uint8_t timeout = 0;
uint8_t Init_dangwei = 0; 
uint8_t Cengshu = 0;
uint8_t Wireless_Kaiqima[3]={0xA1,0xB2,0XC3};

void xAuto_Run_Function(void)
{
    uint8_t Buf[100]={0};
    switch(Run_State)                         
	{
        case 1:
        {	
            uint8_t led_display[3]={0x00,0x00,0x00};
            LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000
            LED_Display_Data.xLED_Display_Time(LED_Display_Data.TimeStart);   //开始计时
            delay_ms(200);
            Motor_Data.xCAR_Track_Go();
            delay_ms(300);
            Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
            delay_ms(400);
            
            Run_State=2;
            break;
          }
          case 2:
          {   
               //***********************************************安卓识别**********************
           Identify_TFT_Store[2]=0x10;
           Identify_TFT_Store[3]=0x01;
            Android_Data.xMainCar_Send_Android(Identify_TFT_Store);

            Android_Data.TFT_Licence_Flag = 0;
            timeout=0;
            while(Android_Data.TFT_Licence_Flag != 1)
              {
                  delay_ms(500);
                  delay_ms(500);
                  timeout++;
                  if(timeout>=30)    //40秒超时退出*************************************************************************
                  {
                      break;
                  }
              }
              Android_Data.TFT_Licence_Flag = 0;
              
              FollowCar_Data.XSend_Licence_To_fromcar((char *)TFT_License_Data_Store);
              delay_ms(200);
              FollowCar_Data.XSend_Licence_To_fromcar((char *)TFT_License_Data_Store);
              delay_ms(200);
              
              if(timeout >= 40)
              {
                  timeout =0;
              }
              else
              {
                  
                 uint8_t Hex_num[6]={0};
                string_to_hex_digits(TFT_License_Data_Store,Hex_num);
                Wireless_Kaiqima[0] = (Hex_num[0]<<4) | Hex_num[1];
                Wireless_Kaiqima[1] = (Hex_num[2]<<4) | Hex_num[3];
                Wireless_Kaiqima[2] = (Hex_num[4]<<4) | Hex_num[5];
              }
              for(uint8_t i=0;i<3;i++)
                  printf("Wireless_Kaiqima:%02x \r\n",Wireless_Kaiqima[i]);

              Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
              delay_ms(300);
              
              Run_State = 3;
                break;
          }
          case 3:
          {     
              RFID_Data.xRFID_Track_Read_2(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block); 
              delay_ms(300);
              
              //*****************************************************交通灯A***************************************
          Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯A
            while( Android_Data.traffic_light_flag != 1)
            {
                delay_ms(500);
                delay_ms(500);
                timeout++;
                if(timeout>=5)
                {
                    timeout=0;
                    break;
                }
            }
            Android_Data.traffic_light_flag=0;
            if(Android_Data.Red_State == 1)
            {
                Android_Data.Red_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
            }
            else if(Android_Data.Yellow_State == 1)
            {
                Android_Data.Yellow_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
            }
            else if(Android_Data.Green_State == 1)
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
            }
            else//蒙一个
            {
                Android_Data.Green_State = 0;
                Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
            }
           RFID_Data.xRFID_Track_Read_2_1(13,Judge_Card_Block,Real_Card1_Block,Real_Card2_Block); 
           delay_ms(300);
          
                Run_State=4;
                break;
          }
          case 4:
          {    
             //*******************************************************超声波测距*************************************
//            Motor_Data.xCAR_Back(25,800);
//            delay_ms(300);
//            Motor_Data.xCAR_Track(30);
             delay_ms(500);
             delay_ms(500);
             Ultrasonic_Ranging();
             delay_ms(500);
             delay_ms(500);
             delay_ms(500);
             printf("dis:%d ",dis);
             
              
           RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(25,280);
          delay_ms(500);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(25,280);
          delay_ms(500);
          RFID_Data.xRFID_Read_2(Judge_Card_Block,Real_Card1_Block,Real_Card2_Block);
          Motor_Data.xCAR_Go(25,250);
            
              Run_State=5;
              
              break;
          }
          case 5:
          {    
                 //*********************************十字路口开始测********二维码识别**********************
            Identify_Two_Code_Arr[3] = 0x01;
            Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            YT2_parse_two_codes();
            
            if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
            {
                 Motor_Data.xCAR_Back(20,300);   
                delay_ms(500);
                delay_ms(500);
                Rx_count = 0;
                Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                delay_ms(500);
                delay_ms(500);
                delay_ms(500);
                YT2_parse_two_codes();
                if(Android_Data.Two_Code_State == 0)  //如果未进入识别模式
                {
                     Motor_Data.xCAR_Back(25,200);   
                    delay_ms(500);
                    delay_ms(500);
                    Rx_count = 0;
                    Android_Data.xMainCar_Send_Android(Identify_Two_Code_Arr);   //发送请求识别二维码
                    delay_ms(500);
                    delay_ms(500);
                    delay_ms(500);
                    
                    YT2_parse_two_codes();
                    Motor_Data.xCAR_Go(25,200); 
                    delay_ms(400);
                }
                Motor_Data.xCAR_Go(20,300); 
            }
            delay_ms(300);
            if(Android_Data.Two_Code_State == 0)
            {
                Android_Data.Two_Code_State = 1;
                printf("aaa  ");
            }
            if(Android_Data.Two_Code_State == 1)
            {
                Android_Data.Two_Code_State = 0;
                Rx_count = 0;
                YT2_parse_two_codes();
                printf("%s\r\n",Two_Code_Data_parsed_Store1);
                printf("%s\r\n",Two_Code_Data_parsed_Store2);
                printf("%s\r\n",Two_Code_Data_parsed_Store3);
            }

            
              Run_State=6;
              break;
          }
          case 6:
          {     
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              Motor_Data.xCAR_Track_Time(30,1000);
              delay_ms(300); //延时
              Motor_Data.xCAR_R90(wheel_Speed, wheel_Time*2);    
              delay_ms(300); //延时
              
              Run_State=7;
              break;
          }
          case 7:
          {    
              Motor_Data.xCAR_Go(25, 300);
             //******************************************************智能语音交互任务***************************
            XiaoChuang_Data.xSend_To_XiaoChuang_Rouse(); // 识别前先唤醒小创
            delay_ms(500);
            Voice_Report_Data.xVoice_Report_Random_Command();  //播报随机指令
			for(uint8_t i = 0;i<5;i++)  // 延时8秒等待小创识别并重复播报
			{
                delay_ms(500);
                delay_ms(500);
			}
            Motor_Data.xCAR_Back(30, 300); 
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
            delay_ms(300);
            
            //***********************************特殊地形****************
          Motor_Data.xCAR_Track(30);
          delay_ms(300);
          Motor_Data.xCAR_Back(20,500);
          delay_ms(300);
          Motor_Data.xCAR_Track(20);
          delay_ms(300);
          Motor_Data.xCAR_Go(30, 610);
          Motor_Data.xCAR_Go(30, 610);
          delay_ms(300);
          Motor_Data.xCAR_Track_Go();
          delay_ms(500);
          delay_ms(500);
            
            
              Run_State=8;
              break;
          }
          case 8:
          {
              //立体显示
              Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add((uint8_t *)READ_RFID2);//立体显示文字
              delay_ms(500);
          delay_ms(500);
              delay_ms(500);
          delay_ms(500);
              
              Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
               Motor_Data.xCAR_Track_Go();
          delay_ms(500);
              Motor_Data.xCAR_R45(wheel_Speed, wheel_Time);
              delay_ms(300);
              
               Run_State=9;
              break;
          }
          case 9:
          {     
              //烽火台
              delay_ms(200);
              SmokeTower_Infrared_Open();
              //TFT
              uint8_t tft_hex[3]={0x5A,0x8D,0x8D};
              Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,tft_hex);//TFT显示
              delay_ms(200);
              Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,tft_hex);//TFT显示
              delay_ms(200);
              Motor_Data.xCAR_L45(wheel_Speed, wheel_Time);
              delay_ms(200);
              Motor_Data.xCAR_Track_Go();
              delay_ms(200);
              Run_State=10;
              
              break;
          }
          case 10:
          {     
                  //***********************************************安卓识别**********************
              Identify_TFT_Graph_CountAndColour_Arr[3]=0x02;
		Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
        Android_Data.TFT_GraphCount_Flag=0;
        Android_Data.TFT_GraphColour_Flag=0;
        timeout=0;
        while(Android_Data.TFT_GraphCount_Flag != 1 && Android_Data.TFT_GraphColour_Flag != 1)
          {
              delay_ms(500);
              delay_ms(500);
              timeout++;
              if(timeout>=50)    //40秒超时退出
              {
                  break;
              }
          }
          Android_Data.TFT_GraphCount_Flag = 0;
          Android_Data.TFT_GraphColour_Flag = 0;
          uint8_t Hex_num[6]={0};
          uint8_t tft_hex[3]={0xA1,0xD2,0XE3};
          uint8_t led_display[3]={0xF3,0xF5,0xF1};
          if(timeout >= 60)
          {
              timeout =0;
          }
          else
          {
              string_to_hex_digits(TFT_GraphCount_Data_Store,Hex_num);
              tft_hex[0] = (Hex_num[0]<<4)|Hex_num[1];
              tft_hex[1] = (Hex_num[2]<<4)|Hex_num[3];
              tft_hex[2] = (Hex_num[4]<<4)|Hex_num[5];
              
            led_display[0] = 0xF0 | TFT_GraphColour_Data_Store[0]-'0';
            led_display[1] = 0xF0 | TFT_GraphColour_Data_Store[1]-'0';
            led_display[2] = 0xF0 | TFT_GraphColour_Data_Store[2]-'0';
          }
          Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(2,tft_hex);//TFT B显示
          delay_ms(200);
          LED_Display_Data.xLED_Display_Data(led_display,2);//LED显示标志物第二排显示000000    
          printf("GraphCount:%s \r\n",TFT_GraphCount_Data_Store);
          printf("GraphCount:%s \r\n",TFT_GraphColour_Data_Store);
           
              
              Run_State=11;
              break;
          }
          case 11:
          { 
               for(uint8_t i = 0; i<3; i++)//启动从车
                {   
                  FollowCar_Data.xStart_Command_To_FollowCar();
                  delay_ms(100);
                }  
          
                //等待启动
                timeout=0;
               while(FollowCar_Data.FollowCar_Finish_Flag == 0)//2分钟
               {
                   delay_ms(500);
                   delay_ms(500);
                   delay_ms(200);
                   timeout++;
                   if(timeout>=100)
                   {
                       timeout=0;
                       break;
                   }
               }
               FollowCar_Data.FollowCar_Finish_Flag = 0;

              
              Run_State=12;
              break;
          }
          case 12:
          {   
            Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
             ETC_Data.xETC_Pass();
              delay_ms(300);
             
              
              Run_State=13;
              break;
          }
          case 13:
          {    
              CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,1);
              delay_ms(200);
               Motor_Data.xCAR_L90(wheel_Speed, wheel_Time*2);
              delay_ms(300);
              CarPort_Data.xCarPort_CarBack_Into(1);
              CarPort_Data.xCarPort_Control_Arrive_Level(CarPort_Data.Device_A,Follow_Send_Weizhi);
              
              Run_State=14; 
              
              break;
          }
          case 14:
          {  
              LED_Display_Data.xLED_Display_Time(0x00);//停止计时
              delay_ms(200);
              LED_Display_Data.xLED_Display_Time(0x00);//停止计时
              delay_ms(200);
              Wireless_Charge_Data.xWireless_Charge_Open_Close(1);
              delay_ms(200);
              Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Wireless_Kaiqima);
              delay_ms(200);
              Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Wireless_Kaiqima);
              Run_State=15;
              break;
          }
          case 15:
          {  
              Run_State=16;
              break;
          }
          default:break;           
    }

}
#endif


