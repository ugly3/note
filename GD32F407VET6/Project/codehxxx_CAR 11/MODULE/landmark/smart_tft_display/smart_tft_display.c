/**
************************************************************************
 *    文件名：smart_tft_display
 *      说明：智能TFT显示标志物
************************************************************************
**/
#include "smart_tft_display.h"

Smart_TFT_Typedef Smart_TFT_Data = 
{
	.Device_A = 1,
	.Device_B = 2,
	.Device_C = 3,
	.xSmart_TFT_Image_Display = &xSmart_TFT_Image_Display,
	.xSmart_TFT_Image_Up_Dowm_Auto = &xSmart_TFT_Image_Up_Dowm_Auto,
	.xSmart_TFT_Licence_Display = &xSmart_TFT_Licence_Display,
	.xSmart_TFT_Time_Diaplay =&xSmart_TFT_Time_Diaplay,
	.xSmart_TFT_Hex_Diaplay =&xSmart_TFT_Hex_Diaplay,
	.xSmart_TFT_Distance_Diaplay =&xSmart_TFT_Distance_Diaplay,
	.xSmart_TFT_Traffic_Flag_Diaplay =&xSmart_TFT_Traffic_Flag_Diaplay,
};

/*
TFT标志物固定指令发送 帧头1，帧尾
*/
uint8_t Smart_TFT_Buf[8] = {0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB};    //智能交通灯标志物A



/*
 *****************************图片显示***************************************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B） --0x12（设备C）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|   副指令2    |  副指令3  | 说明
   0x10   |  0x00  |   0x01~0x20  |    0x00   | 显示指定图片
******************************************************************************
参数1：选择设备（A或B或C）
参数2：图片编码0x01~0x20
返回值：无
*/
void xSmart_TFT_Image_Display(uint8_t device,uint8_t num)
{
	uint8_t CheckSum;
	uint8_t Temp[8];
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}		
	Temp[2] = 0x10; 
	Temp[3] = 0x00; 
	Temp[4] = num;
	Temp[3] = 0x00;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}

/*
 *****************************图片显示模式（向上，下，自动翻页）**************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x10   |  0x01  |   0x00   |    0x00   | 图片向上翻页
   0x10   |  0x02  |   0x00   |    0x00   | 图片向下翻页
   0x10   |  0x03  |   0x00   |    0x00   | 图片自动翻页模式（10秒间隔）
******************************************************************************
参数1：选择设备（A或B）
参数2：1（向上翻页）---2（向下翻页）--3（自动翻页）
返回值：无
*/
void xSmart_TFT_Image_Up_Dowm_Auto(uint8_t device,uint8_t mode)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}		
	Temp[2] = 0x10;
	switch(mode)
	{
		case 1:  //向上翻页
		{
			Temp[3] = 0x01;
		}
		case 2:  //向下翻页
		{
			Temp[3] = 0x02;
		}
		case 3:  //自动翻页（间隔10秒）
		{
			Temp[3] = 0x03;
		}
		default:break;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
//	for(uint8_t i=0;i<3;i++)
//	{
		Send_ZigbeeData_To_Fifo(Temp,8);  //发送三次数据
//		delay_ms(60);
//	}
}

/*
 *****************************车牌显示（前三位+后三位）**************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x20   |  0xXX  |   0xXX   |    0xXX   | 车牌前三位数据显示（ASCII格式）
   0x21   |  0xXX  |   0xXX   |    0xXX   | 车牌后三位数据显示（ASCII格式）
******************************************************************************
参数1：选择设备（A或B）
参数2：
返回值：无
*/
void xSmart_TFT_Licence_Display(uint8_t device,char*str)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}		
	//先发送前三位车牌数据
	Temp[2] = 0x20;  //主指令
	Temp[3] = *(str+0);
	Temp[4] = *(str+1);
	Temp[5] = *(str+2);
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);  //发送三次车牌前三位数据
		delay_ms(100);
	}
	delay_ms(300);
	//再发送后三位车牌数据
	Temp[2] = 0x21;  //主指令
	Temp[3] = *(str+3);
	Temp[4] = *(str+4);
	Temp[5] = *(str+5);
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);  //发送三次车牌后三位数据
		delay_ms(100);
	}
}


/*
 *****************************TFT计时显示关闭，打开，清零********************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x30   |  0x00  |   0x00   |    0x00   | 计时显示关闭
   0x30   |  0x01  |   0x00   |    0x00   | 计时显示打开
   0x30   |  0x02  |   0x00   |    0x00   | 计时显示清零
******************************************************************************
参数1：选择设备（A或B）
参数2：1（计时显示关闭）---2（计时显示打开）---3（计时显示清零）
返回值：无
*/
void xSmart_TFT_Time_Diaplay(uint8_t device,uint8_t mode)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}		
	Temp[2] = 0x30;
	switch(mode)
	{
		case 1:
		{
			Temp[3] = 0x00;  //计时显示关闭
		}
		case 2:
		{
			Temp[3] = 0x01;  //计时显示打开
		} 
		case 3:
		{
			Temp[3] = 0x02;  //计时显示清零
		}
		default:break;
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}

/*
 *****************************六位HEX格式数据显示***************************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x40   |  0xXX  |   0xXX   |    0xXX  | 六位HEX格式数据显示
******************************************************************************
参数1：选择设备（A或B）
参数2：
返回值：无
*/
void xSmart_TFT_Hex_Diaplay(uint8_t device,char*str)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	Temp[0] = 0x55;
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}		
	Temp[2] = 0x40;
	Temp[3] =0xA1; 
	Temp[4] =0xD2;
	Temp[5] =0xE3;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}

/*
 *****************************距离显示（十进制格式）***************************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x50   |  0x00  |   0x0X   |    0xXX  | 距离显示（十进制格式）
******************************************************************************
参数1：选择设备（A或B）
参数2：
返回值：无
*/
void xSmart_TFT_Distance_Diaplay(uint8_t device,uint16_t dis)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}	
// 获取超声波位置，距离显示都要先获取以下超声波位置
	Temp[2] = 0x50;
	Temp[3] = 0x00;
	Temp[4] = (dis/100%10);
	Temp[5] = (dis/10%10)*16 + (dis%10);
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
}


/*
 *****************************显示交通标志************************************
 * 帧头1---> 0x55   * 帧头2--0x0B（设备A）--0x08（设备B）   * 帧尾---> 0xBB
------------------------------------------------------------------------------
   主指令 | 副指令1|  副指令2 |  副指令3  | 说明
   0x60   |  0x01  |   0x00   |    0x00   | 显示交通标志“直行”
   0x60   |  0x02  |   0x00   |    0x00   | 显示交通标志“左转”
   0x60   |  0x03  |   0x00   |    0x00   | 显示交通标志“右转”
   0x60   |  0x04  |   0x00   |    0x00   | 显示交通标志“掉头”
   0x60   |  0x05  |   0x00   |    0x00   | 显示交通标志“禁止直行”
   0x60   |  0x06  |   0x00   |    0x00   | 显示交通标志“禁止通信”
******************************************************************************
参数1：选择设备（A或B）
参数2：
返回值：无
*/
void xSmart_TFT_Traffic_Flag_Diaplay(uint8_t device,uint8_t flag)
{
	uint8_t CheckSum;
	uint8_t Temp[8] = {0};
	memcpy(Temp,Smart_TFT_Buf,sizeof(Smart_TFT_Buf));
	if(device == 1)  //设备A
	{
		Temp[1] = 0x0B;
	}
	else if(device == 2)  //设备B
	{
		Temp[1] = 0x08;
	}
	else if(device == 3)  //设备C
	{
		Temp[1] = 0x12;
	}	
	Temp[2] = 0x60;
	switch(flag)
	{
		case 1:  //显示直行
		{
			Temp[3] = 0x01;
			break;	
		}
		case 2:  //显示左转
		{
			Temp[3] = 0x02;
			break;	
		}
		case 3:  //显示右转
		{
			Temp[3] = 0x03;
			break;	
		}
		case 4:  //显示掉头
		{
			Temp[3] = 0x04;
			break;	
		}
		case 5:  //禁止直行
		{
			Temp[3] = 0x05;
			break;	
		}
		case 6:  //禁止通信
		{
			Temp[3] = 0x06;
			break;	
		}
        default:break;		
	}
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
//	for(uint8_t i=0;i<3;i++)
//	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);		
//	}
}
















