/**
************************************************************************
*    文件名：mixture
*      说明：混合模块，例如：按键----自己创建的函数
************************************************************************
**/
#include "mixture.h"

// 初始化函数
Mixture_Typedef Mixture_Data =
	{
		.xGet_CheckSum = &xGet_CheckSum,
		.xKey_Read = &xKey_Read,
		.xTIM2_Init = &xTIM2_Init,
		.xTIM3_Init = &xTIM3_Init,
		.xCAR_KeyRun_Function = &xCAR_KeyRun_Function,
		.xMixture_Zigbe_RecceiveError_Handler = &xMixture_Zigbe_RecceiveError_Handler,
		.xMixture_Debug_Data = &xMixture_Debug_Data,
		.xBubble_Sort = &xBubble_Sort,
		.xTba_Both_Led = &xTba_Both_Led,
};

Key_Typedef KeyData =
	{
		.S1_value = 0x01,
		.S2_value = 0x02,
		.S3_value = 0x04,
		.S4_value = 0x08,
		.S1_Flag = 0,
		.S2_Flag = 0,
		.S3_Flag = 0,
		.S4_Flag = 0,
		.Trg = 0x00, // 短按
		.Cont = 0x00 // 长按（一般不用）
};

/* 计算校验和---主指令---副指令1---副指令2--副指令3 -
参数1：主指令
参数2：副指令1
参数3：副指令2
参数4：副指令3
*/
uint8_t xGet_CheckSum(uint8_t main_com, uint8_t sub_com1, uint8_t sub_com2, uint8_t sub_com3)
{
	uint8_t SUM;
	SUM = (main_com + sub_com1 + sub_com2 + sub_com3) % 256;
	return SUM;
}

/* 三行按键法 */
#define KEYPORT (S1) | (S2 << 1) | (S3 << 2) | (S4 << 3) | 0xF0 // 按键相关
void xKey_Read(void)
{
	uint8_t ReadDate = (KEYPORT) ^ 0xFF; // 取反
	KeyData.Trg = ReadDate & (ReadDate ^ KeyData.Cont);
	KeyData.Cont = ReadDate;

	/* 按键标志位*/
	if (KeyData.Trg & KeyData.S1_value) // 按键1
	{
		KeyData.S1_Flag = 1;
	}
	if (KeyData.Trg & KeyData.S2_value) // 按键2
	{
		KeyData.S2_Flag = 1;
	}
	if (KeyData.Trg & KeyData.S3_value) // 按键3
	{
		KeyData.S3_Flag = 1;
	}
	if (KeyData.Trg & KeyData.S4_value) // 按键4
	{
		KeyData.S4_Flag = 1;
	}
}

/* TIM2 每1ms进入一次 */
void xTIM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启定时器2的时钟
	TIM_InternalClockConfig(TIM2);						 // 选择内部时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能更新中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	TIM_Cmd(TIM2, ENABLE); // 开启定时器2
}

/* TIM3 每1ms进入一次 */
void xTIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启定时器2的时钟
	TIM_InternalClockConfig(TIM3);						 // 选择内部时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能更新中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	TIM_Cmd(TIM3, ENABLE); // 开启定时器2
}

/*定时器3的中断服务函数  每1ms进入一次*/
void TIM3_IRQHandler(void)
{
	static uint8_t zigbee_cnt2ms = 0;
	static uint8_t wifi_cnt3ms = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		zigbee_cnt2ms++;
		wifi_cnt3ms++;

		if (zigbee_cnt2ms >= 2)
		{
			zigbee_cnt2ms = 0;
			Can_ZigBeeRx_Check(); // Zigbee交互数据处理
		}
		if (wifi_cnt3ms >= 3)
		{
			wifi_cnt3ms = 0;
			Can_WifiRx_Check(); // Wifi交互数据处理 (安卓与主车)
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

// 任务板双向灯标志位
volatile bool Tab_Both_Led_Flag = 0;

/*定时器2的中断服务函数  每1ms进入一次*/
void TIM2_IRQHandler(void)
{
	static uint8_t Key_cnt10ms = 0;
	static uint8_t Led_cnt250ms = 0;
	static uint8_t XiaoChuang_cnt50ms = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_cnt10ms++;
		Led_cnt250ms++;
		XiaoChuang_cnt50ms++;

		if (XiaoChuang_cnt50ms >= 50)
		{
			XiaoChuang_cnt50ms = 0;
			XiaoChuang_Data.xXiaoChuang_Analyze_Rx_Command();
		}

		if (Key_cnt10ms >= 10) // 按键检测10ms
		{
			Key_cnt10ms = 0;
			Mixture_Data.xKey_Read();
		}
		if (Led_cnt250ms >= 250) // led闪烁
		{
			Led_cnt250ms = 0;
			LED4 = !LED4;
			if (Tab_Both_Led_Flag)
			{
				GPIO_ToggleBits(GPIOH, GPIO_Pin_10);
				GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
			}
			else
			{
				GPIO_SetBits(GPIOH, GPIO_Pin_10);
				GPIO_SetBits(GPIOH, GPIO_Pin_11);
			}
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

extern uint8_t Run_State; // 运行位置状态
void xCAR_KeyRun_Function(void)
{
	if (KeyData.S1_Flag) // 按键1
	{
		KeyData.S1_Flag = 0; // 禁止屏蔽
		Run_State = 0;
	}
	if (KeyData.S2_Flag) // 按键2
	{
		KeyData.S2_Flag = 0; // 禁止屏蔽
		Run_State = 1;
		/*********************************************************无线充电标志物**************************************/
		//		unsigned int Kai_Qi_Ma[3]={0xA1,0x23,0xB4};
		//		Wireless_Charge_Data.xWireless_Charge_KaiQiMa_XiuGai(Kai_Qi_Ma);
		//		Wireless_Charge_Data.xWireless_Charge_KaiQiMa_Open(Kai_Qi_Ma);//需修改开启码

		//      char Hex_num[3]={0x01,0x20,0x26};
		//      Smart_TFT_Data.xSmart_TFT_Hex_Diaplay(1,Hex_num);//TFTA十六进制显示模式，显示012026
		//		Smart_TFT_Data.xSmart_TFT_Licence_Display(1,"F222F2");//TFTA车牌显示模式
		//		LED_Display_Data.xLED_Display_Data(0xF3,0xF5,0xF1,2);//LED显示标志物第二排显示F3F5F1
		/*********************************************************安卓识别********************************************/
		//		Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
		/*********************************************************公交站***********************************************/
		//		Voice_Report_Data.xVoice_Report_Inquire_Weather_Temperatur();  //查询天气温度
		//		Voice_Report_Data.xVoice_Report_Speak_temperature();//播报温度
		//		Voice_Report_Data.xVoice_Report_Speak_Weather();//播报天气状况
		//		if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x00)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("大风");
		//		else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x01)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("多云");
		//		else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x02)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("晴");
		//		else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x03)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("小雪");
		//		else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x04)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("小雨");
		//		else if(Voice_Report_Data.xVoice_Report_Rx_Weather_Temperatur[0]==0x05)
		//				Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add_2("阴天");

		/*********************************************************交通灯***********************************************/
		//	    Smart_Traffic_Data.xSmart_Traffic_Ask_State(Smart_Traffic_Data.Device_A);  //发送请求识别红绿灯
		//		for(uint8_t i=0;i<3;i++) //等待安卓回传
		//		{
		//			delay_ms(500);
		//			delay_ms(500);
		//		}
		//		if(Communication_Data.Smart_Traffic_A_Recognition_State == 1){
		//		if(Android_Data.Red_State == 1)
		//		{
		//				Android_Data.Red_State = 0;
		//				Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,1);//发送给交通灯标志物请求确认
		//				delay_ms(500);
		//		}
		//		else if(Android_Data.Yellow_State == 1)
		//		{
		//				Android_Data.Yellow_State = 0;
		//				Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,3);
		//				delay_ms(500);
		//		}
		//		else if(Android_Data.Green_State == 1)
		//		{
		//				Android_Data.Green_State = 0;
		//				Smart_Traffic_Data.xSmart_Traffic_Colour_Recognition(Smart_Traffic_Data.Device_A,2);
		//				delay_ms(500);
		//		}
		//	}
		/*********************************************************立体显示标志物***********************************************/
		//        Three_Dim_Display_Data.xThree_Dim_Display_licence_coord("ABS123",'A',1);
		//        delay_ms(500);
		//        delay_ms(500);
		//        Three_Dim_Display_Data.xThree_Dim_Display_Custom_Add("黄东圳");
		//        Three_Dim_Display_Data.xThree_Dim_Display_Distance(16);
		//        Three_Dim_Display_Data.xThree_Dim_Display_Graph(0X01);
		//        Three_Dim_Display_Data.xThree_Dim_Display_Colour(0X01);
		//        Three_Dim_Display_Data.xThree_Dim_Display_Traffic_Caution(0X01);
		//        Three_Dim_Display_Data.xThree_Dim_Display_RGB_Colour(0xff,0x00,0x00);
	}
	if (KeyData.S3_Flag) // 按键3
	{
		KeyData.S3_Flag = 0; // 禁止屏蔽
		Run_State = 0;
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
      if (strlen((const char *)Two_Code_Data_parsed_Store1) > 0)
      {
        // 假设任务需要将字符 'D' 和 '7' 识别为十六进制数值
        // 这里提供一种通用的字符转数值方法

        // 转成十六进制，无线充电标志物开启码
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
		//		Android_Data.xMainCar_Send_Android(Identify_TFT_License_Arr);//车牌

		//		 unsigned int Kai_Qi_Ma[3]={0xA1,0x23,0xB4};
		//		Android_Data.xMainCar_Send_Android(Identify_TFT_Graph_CountAndColour_Arr);//图形加颜色的数量
		//		for(uint8_t i=0;i<8;i++)//等待八秒
		//		{
		//				delay_ms(500);
		//				delay_ms(500);
		//		}
	}
	if (KeyData.S4_Flag) // 按键4
	{
		KeyData.S4_Flag = 0;
		Run_State = 0;
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
	}
}

// 开启 / 关闭任务板中的双向灯
void xTba_Both_Led(uint8_t swch)
{
	switch (swch)
	{
	case SET:
		Tab_Both_Led_Flag = 1;
		break;
	case RESET:
		Tab_Both_Led_Flag = 0;
		break;
	default:
		break;
	}
}

// 接收到Zigbee的错误指令就将错误指令发到Debug区
void xMixture_Zigbe_RecceiveError_Handler(uint8_t Cmd1, uint8_t Cmd2)
{
	uint8_t Buf[50];
	sprintf((char *)Buf, "CMD2-3: %#x-%#x", Cmd1, Cmd2); // 将错误指令打印到Debug
	Send_InfoData_To_Fifo((char *)Buf, strlen((char *)Buf));
}

// 将数据打印到Debug，判断数据
void xMixture_Debug_Data(uint8_t num)
{
	uint8_t buf[50];
	Send_InfoData_To_Fifo((char *)buf, strlen((char *)buf));
}

// 冒泡排序 （从小到大）
void xBubble_Sort(uint16_t *arr, uint8_t len)
{
	uint8_t i, j;
	uint16_t Temp;
	for (i = 0; i < len - 1; i++)
	{
		for (j = 0; j < len - 1 - i; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				Temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = Temp;
			}
		}
	}
}

// 将十六进制转为十进制
uint16_t HEX_DEC(uint8_t *buf, uint16_t value)
{
	value = 0;
	for (uint16_t i = 0; i < sizeof(buf); i++)
	{
		value = (value << 8) | buf[i];
	}
	return value;
}

uint8_t CharToHex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0;
}
