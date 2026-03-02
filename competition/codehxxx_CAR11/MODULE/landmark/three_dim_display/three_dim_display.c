/**
************************************************************************
 *    文件名：three_dim_display
 *      说明：立体显示标志物
 *  通信方式
************************************************************************
**/
#include "three_dim_display.h"


Three_Dim_Display_Typedef Three_Dim_Display_Data = 
{
	.xThree_Dim_Display_licence_coord = xThree_Dim_Display_licence_coord,
	.xThree_Dim_Display_Distance = &xThree_Dim_Display_Distance,
	.xThree_Dim_Display_Graph = &xThree_Dim_Display_Graph,
	.xThree_Dim_Display_Colour = &xThree_Dim_Display_Colour,
	.xThree_Dim_Display_Traffic_Caution = &xThree_Dim_Display_Traffic_Caution,
	.xThree_Dim_Display_Traffic_Flag = &xThree_Dim_Display_Traffic_Flag,
	.xThree_Dim_Display_Default = &xThree_Dim_Display_Default,
	.xThree_Dim_Display_RGB_Colour = &xThree_Dim_Display_RGB_Colour,
	.xThree_Dim_Display_Custom_Empty = &xThree_Dim_Display_Custom_Empty,
	.xThree_Dim_Display_Custom_Add = &xThree_Dim_Display_Custom_Add,
    .xThree_Dim_Display_Custom_Add_2 = &xThree_Dim_Display_Custom_Add_2,
};

/*
立体显示标志物发送固定帧头（红外无线通信）
*/
uint8_t Three_Dim_Display_Buf[6] = {0xFF,0x00,0x00,0x00,0x00,0x00};


/*
 *******************前四位车牌和后两位车牌及坐标信息显示模式********************
                           * 帧头---> 0xFF  
---------------------------------------------------------------------------------
   主指令 |   数据1    |   数据2   |   数据3  |   数据4   |     说明
   0x20   |   车牌[1]  |   车牌[2] |  车牌[3] |  车牌[4]  |   显示车牌及坐标
   0x10	  |   车牌[5]  |   车牌[6] |  横坐标  |  纵坐标   |   显示车牌及坐标
**********************************************************************************
参数1： 车牌---如："ABC123" （六位车牌）
参数2： 横坐标x --- 如：'A'   坐标需要用字符来表示
参数3： 纵坐标y --- 如：'1'   坐标需要用字符来表示  数字字符范围是0 ~ 9
返回值：无
*/
void xThree_Dim_Display_licence_coord(char*str,char x,char y)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	//先发送车牌前四位
	Temp[1] = 0x20;
	Temp[2] = *(str+0);   //数据1
	Temp[3] = *(str+1);   //数据2
	Temp[4] = *(str+2);   //数据3
	Temp[5] = *(str+3);   //数据4
	for(uint8_t i=0;i<3;i++)   //前四位车牌数据
	{
		Infrared_Send(Temp,6);
		delay_ms(100);
	}
	Temp[1] = 0x10;
	Temp[2] = *(str+4);   //数据1
	Temp[3] = *(str+5);   //数据2
    Temp[4] = x;          //横坐标
    Temp[5] = y;	      //纵坐标
	for(uint8_t i=0;i<3;i++)   //后两位车牌及坐标点
	{
		Infrared_Send(Temp,6);
		delay_ms(100);		
	}
}                         

/*
 ******************************距离信息信息显示模式*****************************
                           * 帧头---> 0xFF  
---------------------------------------------------------------------------------
   主指令 |   数据1     |   数据2    |   数据3  |   数据4  |     说明
   0x11   |   距离十位  |   距离个位 |   0x00   |  0x00    |   显示距离
**********************************************************************************
参数1： 要显示的距离，两位数
返回值：无
*/
void xThree_Dim_Display_Distance(uint16_t dis)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x11; //主指令
	Temp[2] = dis / 10 % 10;  //距离十位--cm
	Temp[3] = dis % 10;       //距离个位--cm
	for(uint8_t i=0;i<3;i++)
	{
		Infrared_Send(Temp,6);
		delay_ms(100);				
	}
}

/*
 ********************************图形信息显示模式********************************
                           * 帧头---> 0xFF  
---------------------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |   数据4  |     说明
   0x12   |   0x01   |   0x00   |   0x00   |   0x00   |     矩形
   0x12   |   0x02   |   0x00   |   0x00   |   0x00   |     圆形
   0x12   |   0x03   |   0x00   |   0x00   |   0x00   |   三角形
   0x12   |   0x04   |   0x00   |   0x00   |   0x00   |     菱形
   0x12   |   0x05   |   0x00   |   0x00   |   0x0    |   五角星
**********************************************************************************
参数1： 要显示的图形
返回值：无
*/
void xThree_Dim_Display_Graph(uint8_t type)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x12;  //主指令
	switch(type)
	{
		case rectangle:      //矩形
		{
			Temp[2] = 0x01;
			break;
		}
		case circle:         //圆形 
		{
			Temp[2] = 0x02;
			break;
		}
		case triangle:       //三角形
		{
			Temp[2] = 0x03;
			break;			
		}
		case rhombus:        //菱形
		{
			Temp[2] = 0x04;
			break;			
		}
		case five_star:      //五角星
		{
			Temp[2] = 0x05;
			break;			
		}
		default:break;
	}
	for(uint8_t i=0;i<3;i++)  //发三次数据
	{
		Infrared_Send(Temp,6);
		delay_ms(100);						
	}
}

/*
 ********************************颜色信息显示模式********************************
                           * 帧头---> 0xFF  
---------------------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |   数据4  |     说明
   0x13   |   0x01   |   0x00   |   0x00   |   0x00   |     红色
   0x13   |   0x02   |   0x00   |   0x00   |   0x00   |     绿色
   0x13   |   0x03   |   0x00   |   0x00   |   0x00   |     蓝色
   0x13   |   0x04   |   0x00   |   0x00   |   0x00   |     黄色
   0x13   |   0x05   |   0x00   |   0x00   |   0x00   |     品色
   0x13   |   0x06   |   0x00   |   0x00   |   0x00   |     青色
   0x13   |   0x07   |   0x00   |   0x00   |   0x00   |     黑色
   0x13   |   0x08   |   0x00   |   0x00   |   0x00   |     白色
**********************************************************************************
参数1： 要显示的图形
返回值：无
*/
void xThree_Dim_Display_Colour(uint8_t type)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x13;  //主指令
	switch(type)
	{
		case red:     //红色
		{
			Temp[2] = 0x01;
			break;
		}
		case green:   //绿色
		{
			Temp[2] = 0x02;
			break;
		}
		case blue:    //蓝色
		{
			Temp[2] = 0x03;
			break;
		}
		case yellow:  //黄色
		{
			Temp[2] = 0x04;
			break;
		}
		case pinse:   //品色
		{
			Temp[2] = 0x05;
			break;
		}
		case cyan:    //青色
		{
			Temp[2] = 0x06;
			break;
		}
		case black:   //黑色
		{
			Temp[2] = 0x07;
			break;
		}
		case white:   //白色
		{
			Temp[2] = 0x08;
			break;
		}
		default:break;
	}
	for(uint8_t i=0;i<3;i++)
	{
		Infrared_Send(Temp,6);
		delay_ms(100);								
	}
}

/*
 ********************************交通警示牌信息显示模式**************************
                           * 帧头---> 0xFF  
---------------------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |   数据4  |     说明
   0x14   |   0x01   |   0x00   |   0x00   |   0x00   |  前方学校，减速慢行 
   0x14   |   0x02   |   0x00   |   0x00   |   0x00   |  前方施工，禁止通行
   0x14   |   0x03   |   0x00   |   0x00   |   0x00   |  塌方路段，注意安全
   0x14   |   0x04   |   0x00   |   0x00   |   0x00   |  追尾危险，保持车距
   0x14   |   0x05   |   0x00   |   0x00   |   0x00   |  严禁，酒后驾车！ 
   0x14   |   0x06   |   0x00   |   0x00   |   0x00   |  严禁，乱扔垃圾！
**********************************************************************************
参数1： 要显示的交通警示牌
返回值：无
*/
void xThree_Dim_Display_Traffic_Caution(uint8_t type)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x14;  //主指令
    switch(type)
	{
		case 1:
		{
			Temp[2] = 0x01;   //前方学校，减速慢行
			break;
		}
		case 2:
		{
			Temp[2] = 0x02;   //前方施工，禁止通行
			break;
		}
		case 3:
		{
			Temp[2] = 0x03;   //塌方路段，注意安全
			break;
		}
		case 4:
		{
			Temp[2] = 0x04;   //追尾危险，保持车距 
			break;
		}
		case 5:
		{
			Temp[2] = 0x05;   //严禁，酒后驾车！
			break;
		}
		case 6:
		{
			Temp[2] = 0x06;   //严禁，乱扔垃圾！
			break;
		}
		default:break;
	}
    for(uint8_t i=0;i<3;i++)
 	{
		Infrared_Send(Temp,6);
		delay_ms(100);										
	}		
}

/*
 ********************************交通标志信息显示模式******************
                           * 帧头---> 0xFF  
-----------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |   数据4  |   说明
   0x15   |   0x01   |   0x00   |   0x00   |   0x00   |   直行 
   0x15   |   0x02   |   0x00   |   0x00   |   0x00   |   左转
   0x15   |   0x03   |   0x00   |   0x00   |   0x00   |   右转
   0x15   |   0x04   |   0x00   |   0x00   |   0x00   |   掉头
   0x15   |   0x05   |   0x00   |   0x00   |   0x00   |   禁止直行 
   0x15   |   0x06   |   0x00   |   0x00   |   0x00   |   禁止通行
***********************************************************************
参数1： 要显示的交通标志
返回值：无
*/
void xThree_Dim_Display_Traffic_Flag(uint8_t type)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x15;  //主指令
	switch(type)
	{
		case straight:     //直行
		{
			Temp[2] = 0x01;   
			break;
		}
		case turn_left:    //左转
		{
			Temp[2] = 0x02;   
			break;
		}
		case turn_right:   //右转
		{
			Temp[2] = 0x03;   
			break;
		}
		case turn_round:   //掉头
		{
			Temp[2] = 0x04;   
			break;
		}
		case no_straight:  //禁止直行
		{
			Temp[2] = 0x05;   
			break;
		}
		case no_pass:      //禁止通行
		{
			Temp[2] = 0x06;   
			break;
		}
		default:break;
	}
	for(uint8_t i=0;i<3;i++)  //发三次数据
	{
		Infrared_Send(Temp,6);
		delay_ms(100);												
	}
}

/*
 ********************************显示默认信息*************************
                           * 帧头---> 0xFF  
-----------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |   数据4  |   说明
   0x16   |   0x01   |   0x00   |   0x00   |   0x00   |   显示默认信息 
***********************************************************************
参数：无
返回值：无
*/
void xThree_Dim_Display_Default(void)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x16;  //主指令
    Temp[2] = 0x01;
    for(uint8_t i=0;i<3;i++)  //发三次数据
 	{
		Infrared_Send(Temp,6);
		delay_ms(100);														
	}		
}

/*
 ********************************显示默认信息*************************
                           * 帧头---> 0xFF  
-----------------------------------------------------------------------
   主指令 |   数据1  |   数据2     |   数据3     |   数据4     |   说明
   0x16   |   0x01   |   0xXX（R） |   0xXX（G） |   0xXX（B） |   设置文字颜色（RGB格式） 
***********************************************************************
参数1：R（0x00~0xFF）   红色
参数2：G（0x00~0xFF）   绿色
参数3：B（0x00~0xFF）   蓝色
返回值：无
*/
void xThree_Dim_Display_RGB_Colour(uint8_t R,uint8_t G,uint8_t B)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x17;  //主指令
	Temp[2] = 0x01;
	Temp[3] = R;
	Temp[4] = G;
	Temp[5] = B;
    for(uint8_t i=0;i<3;i++)  //发三次数据
 	{
		Infrared_Send(Temp,6);
		delay_ms(100);														
	}		
}

 

/*
 *************************************自定义文本累加显示********************************
                           * 帧头---> 0xFF  
----------------------------------------------------------------------------------------
   主指令 |   数据1             |   数据2             |   数据3     |    数据4  |    说明
   0x31   |   0xXX （文本信息） |   0xXX （文本信息） |   0x55      |    0x00   |    自定义文本累加显示 
****************************************************************************************
参数：输入要写入的文字
返回值：无
*/
void xThree_Dim_Display_Custom_Add(uint8_t*str) 
{
	uint8_t Temp[8] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	
	while(*str)
	{
		if(*str < 0x80)
		{
			Temp[1] = 0x31;
			Temp[2] = *str;	//数据1【文本信息】
			Temp[3] = 0x00;	//数据2【文本信息】
			Temp[4] = 0x00;	//数据3【文字结束】
			Temp[5] = 0x00;	//数据4
			Infrared_Send(Temp, 6);	//发送命令
			str++;
		}
		else
		{
			Temp[1] = 0x31;
			Temp[2] = *str;	//数据1【文本信息】
			Temp[3] = *(str+1);	//数据2【文本信息】
			Temp[4] = 0x00;	//数据3【文字结束】
			Temp[5] = 0x00;	//数据4
			Infrared_Send(Temp, 6);	//发送命令
			str+=2;
		}
			Temp[1] = 0x31;	//主指令
			Temp[2] = 0x00;	//数据1【文本信息】
			Temp[3] = 0x00;	//数据2【文本信息】
			Temp[4] = 0x55;	//数据3【文字结束】
			Temp[5] = 0x00;	//数据4
			Infrared_Send(Temp, 6);	//发送命令		
	}		
}


/*
 *************************************自定义文本清空显示********************************
                           * 帧头---> 0xFF  
----------------------------------------------------------------------------------------
   主指令 |   数据1  |   数据2  |   数据3  |  数据4 |        说明
   0x32   |   0x01   |   0x00   |   0x00   |  0x00  | 结束累加不清除现有显示内容 
   0x32   |   0x02   |   0x00   |   0x00   |  0x00  | 结束累加并清除现有显示内容 
****************************************************************************************
参数： 1（结束累加不清除现有显示内容） --- 2（结束累加并清除现有显示内容）
返回值：无
*/
void xThree_Dim_Display_Custom_Empty(uint8_t type)
{
	uint8_t Temp[6] = {0};
	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
	Temp[1] = 0x32; //主指令
	switch(type)
	{
		case End_Add_NoClear:  //结束累加不清除现有显示内容 
		{
			Temp[2] = 0x01;
			break;
		}
		case End_Add_Clear:  //结束累加并清除现有显示内容 
		{
			Temp[2] = 0x02;
			break;
		}
		default:break;
	}
	for(uint8_t i=0;i<3;i++)  //发三次数据
	{
		Infrared_Send(Temp,6);
		delay_ms(100);
	}
}

	/**********************************************************************
 * 函 数 名 ：  立体显示自定义文本信息   		红外
 * 参    数 ：  *P
 * 返 回 值 ：  无
 * 简    例 ： xThree_Dim_Display_Custom_Add_2("我爱中国")
**********************************************************************/
void xThree_Dim_Display_Custom_Add_2(char *p)
{
	uint16_t p_len=strlen(p);
	uint16_t i;
	uint8_t  Liti_data[6];
	Liti_data[0]=0xff;
	Liti_data[1]=0x31;
	for(i=0;i<p_len;i++)     //i=0,2,4,6    8
	{
		if(*(p+i) & 0x80)
		{
			//汉字处理
			Liti_data[2]=*(p+i);
			i+=1;
			Liti_data[3]=*(p+i);
			if(i>p_len-2)
			  Liti_data[4]=0x55;
			else
			  Liti_data[4]=0x00;
	  }
		else
		{
			//字符处理
			Liti_data[2]=*(p+i);
			Liti_data[3]=0x00;
			if(i>p_len-2)
			  Liti_data[4]=0x55;
			else
			  Liti_data[4]=0x00;
	  }
		
	  Liti_data[5]=0x00;
	 // Send_ZigbeeData_To_Fifo(Liti_data,8);
		Infrared_Send(Liti_data,6);               //红外发射函数
		delay_ms(500);	
    }										
}



/*
显示距离模式下显示编号
*/
//void xThree_Dim_Display_Distance_Code(uint16_t dis)
//{
//	uint8_t Temp[6] = {0};
//	memcpy(Temp,Three_Dim_Display_Buf,sizeof(Three_Dim_Display_Buf));
//    Temp[1] = 0x11; //主指令(显示距离模式)	
//	
//}




















