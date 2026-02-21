/**
************************************************************************
 *    文件名：three_dim_carport
 *      说明：立体车库标志物
 *  通信方式：Zigbee无线通信
************************************************************************
**/
#include "three_dim_carport.h"


CarPort_Tyepdef CarPort_Data = 
{
	.Device_A = 0x0D,
	.Device_B = 0x05,
	.xCarPort_Control_Arrive_Level = &xCarPort_Control_Arrive_Level,
	.xCarPort_Ack_State = &xCarPort_Ack_State,
	.xCarPort_Ack_Infrared_State = &xCarPort_Ack_Infrared_State,
	.xCarPort_Control_Arrive_Delay = &xCarPort_Control_Arrive_Delay,
	.xCarPort_CarBack_Into = &xCarPort_CarBack_Into
};


/*立体车库标志物发送固定指令，帧头和帧尾
注意---当前我们的立体车库标志物为A
*/
uint8_t Carport_Buf[8] = {0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB};  



/*
 ******************************指定立体车库到达的层数*****************************
* 帧头1---> 0x55   * 帧头2---车库A：0x0D--车库B：0x05    帧尾---> 0xBB
---------------------------------------------------------------------------------
   主指令 |   副指令1     |   副指令2  |   副指令3  |     说明
   0x01   |    0x01       |   0x00     |     0x00   | 立体车库复位（到达第一层）
   0x01	  |    0x02       |   0x00     |     0x00   | 立体车库到达第二层
   0x01   |    0x03       |   0x00     |     0x00   | 立体车库达到第三层
   0x01	  |    0x04       |   0x00     |     0x00   | 立体车库到达第四层
**********************************************************************************
参数1：选择设备（A或B）
参数2：选择要到达的层数
返回值：无
*/

void xCarPort_Control_Arrive_Level(uint8_t device,uint8_t num)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t Get_Num_State;
	uint8_t Delay_Num;
   
    Get_Num_State = CarPort_Data.xCarPort_Ack_State(CarPort_Data.Device_A);	 //一定要先写判断当前层数，再发送指定层数命令
	Get_Num_State = CarPort_Data.xCarPort_Ack_State(CarPort_Data.Device_A);
	Get_Num_State = CarPort_Data.xCarPort_Ack_State(CarPort_Data.Device_A);
	
	uint8_t Buf[50];
	sprintf((char*)Buf,"cheku is : %d \r\n",Get_Num_State);  
	Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf)); 
	
  memcpy(Temp,Carport_Buf,sizeof(Carport_Buf));
	
	if(device == 0x0D)  //设备A
	{
		Temp[1] = 0x0D;
	}
	else if(device == 0x05)  //设备B
	{
		Temp[1] = 0x05;
	}
	Temp[2] = 0x01;  //主指令
	Temp[3] = num;   //选择到达的层数
	Temp[4] = 0x00;
	Temp[5] = 0x00;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum;
	for(uint8_t i=0;i<3;i++) //发送三次控制指令；注意：一定要让i=0初始化；
	{
		Send_ZigbeeData_To_Fifo(Temp, 8);	//发送控制命令
		delay_ms(100);		
	}
	if(Get_Num_State == num)   //当前层数和指定的层数相等退出
	{
		CarPort_Data.xCarPort_Control_Arrive_Delay(0);
		return;   //当前层数与指定层数相等直接退出
    }
	else if(Get_Num_State < num)  //当前层数小于指定层数
	{
		Delay_Num = (num - Get_Num_State);   //得出当前层数和指定层数的相差值
		CarPort_Data.xCarPort_Control_Arrive_Delay(Delay_Num);
	}
	else if(Get_Num_State > num)  //当前层数大于指定层数
	{
		Delay_Num=(Get_Num_State - num);    //得出当前层数和指定层数的相差值
		CarPort_Data.xCarPort_Control_Arrive_Delay(Delay_Num);
	}
}


/*
 ******************************请求回传立体车库当前位于的层数************************
* 帧头1---> 0x55   * 帧头2---车库A：0x0D--车库B：0x05    帧尾---> 0xBB       
---------------------------------------------------------------------------------
   主指令 |   副指令1     |   副指令2  |   副指令3  |     说明                     主指令0x02:发送指令请求返回数据
   0x02   |    0x01       |   0x00     |     0x00   | 请求返回立体车库当前层数     
-----------------------------------------------------------------------------------
   0x03	  |    0x01       |   0x01     |     0x00   | 返回车库位于第一层       主指令0x03:接收车库Zigbee回传的指令
   0x03   |    0x01       |   0x02     |     0x00   | 返回车库位于第二层
   0x03	  |    0x01       |   0x03     |     0x00   | 返回车库位于第三层
   0x03	  |    0x01       |   0x04     |     0x00   | 返回车库位于第四层
**********************************************************************************
参数：选择立体车库设备（A或B）
返回值：返回当前车库位于的层数
*/
uint8_t xCarPort_Ack_State(uint8_t device)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	uint8_t Receive_A_State;  //接收设备A的层数
	uint8_t Receive_B_State;  //接收设备B的层数
	
	memcpy(Temp,Carport_Buf,sizeof(Carport_Buf));
	if(device == 0x0D)   //设备A
	{
		Temp[1] = 0x0D;
	}
	else if(device == 0x05)  //设备B
	{
		Temp[1] = 0x05;
	}
	Temp[2] = 0x02;  //主指令，请求返回
	Temp[3] = 0x01;  //副指令1，返回立体车库当前层数
	Temp[4] = 0x00;
	Temp[5] = 0x00;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum; //校验和  
	if(device == 0x0D) //设备A
	{
		while(Communication_Data.CarPort_Back_A_Level == 0)  //判断有没有返回当前车库位于第几层数据
		{
			Send_ZigbeeData_To_Fifo(Temp,8);  //若没有返回车库位于第几层数据，则发送请求回传车库位于几层数据
			delay_ms(100);
			TimeOut++;
			if(TimeOut >25)
			{
				break;    //超时2.5s后，退出此循环，防止卡死
			}
		}
		Receive_A_State = Communication_Data.CarPort_Back_A_Level;
		Communication_Data.CarPort_Back_A_Level = 0;     //对设备A的标志位清零
		return Receive_A_State;
	}
	else if(device == 0x05)  //设备B
	{
		while(Communication_Data.CarPort_Back_B_Level == 0)  //判断有没有返回当前车库位于第几层数据
		{
			Send_ZigbeeData_To_Fifo(Temp,8);  //若没有返回车库位于第几层数据，则发送请求回传车库位于几层数据
			delay_ms(100);
			TimeOut++;
			if(TimeOut >25)
			{
				break;    //超时2.5s后，退出此循环，防止卡死
			}
		}
		Receive_B_State = Communication_Data.CarPort_Back_A_Level;
		Communication_Data.CarPort_Back_B_Level = 0;     //对设备A的标志位清零
		return Receive_B_State;
	}
	return 0;
}


/*
 ******************************请求回传立体车库前/后侧红外状态************************
* 帧头1---> 0x55   * 帧头2---车库A：0x0D--车库B：0x05    帧尾---> 0xBB       
---------------------------------------------------------------------------------
   主指令 |   副指令1     |   副指令2  |   副指令3  |     说明                     主指令0x02:发送指令请求返回数据
   0x02   |    0x02       |   0x00     |     0x00   | 请求返回立体车库前/后侧红外状态    
-----------------------------------------------------------------------------------
   0x03	  |    0x02       |   0x01     |     0x01   |  前侧和后侧被触发            主指令0x03:接收车库Zigbee回传的指令
   0x03   |    0x02       |   0x02     |     0x02   |  前侧和后侧未触发
   0x03	  |    0x02       |   0x01     |     0x02   |  前侧被触发，后侧未触发
   0x03	  |    0x02       |   0x02     |     0x01   |  前侧未触发，后侧被触发
**********************************************************************************
参数：设备（A和B）
返回值：返回前侧和后侧的状态
---副指令2（前）---副指令3（后）
---0x01（被触发）---（0x02未触发）
0x01 -- 0x01（前后被触发）  
0x02 -- 0x02（前后未触发）
0x01 -- 0x02（前触发，后未触发）
0x02 -- 0x01（前未触发，后触发）

//这两个函数接收到： 1（前后被触发）--2（前后未触发）--3（前触发，后未触发）---4（前未触发，后触发）
Communication_Data.CarPort_Back_A_Infrared
Communication_Data.CarPort_Back_B_Infrared

注意：现在的标志物前后测触发与比赛的标志物，手册相反
例如：现在的前后侧触发，比赛都是前后不触发；现在的前触后不触，比赛的前不触，后触；
先在的是按照赛场的代码
*/
uint8_t xCarPort_Ack_Infrared_State(uint8_t device)
{
	uint8_t Temp[8] = {0};
	uint8_t CheckSum;
	uint8_t TimeOut = 0;
	uint8_t Receive_A_State;  //接收设备A的
	uint8_t Receive_B_State;  //接收设备B的
	
	memcpy(Temp,Carport_Buf,sizeof(Carport_Buf));
	if(device == 0x0D)   //设备A
	{
		Temp[1] = 0x0D;
	}
	else if(device == 0x05)  //设备B
	{
		Temp[1] = 0x05;
	}
	Temp[2] = 0x02;  //主指令，请求返回
	Temp[3] = 0x02;  //副指令1，返回前后触发状态
	Temp[4] = 0x00;
	Temp[5] = 0x00;
	CheckSum = Mixture_Data.xGet_CheckSum(Temp[2],Temp[3],Temp[4],Temp[5]);
	Temp[6] = CheckSum; //校验和
    if(device == 0x0D)	//设备A
	{
		while(Communication_Data.CarPort_Back_A_Infrared == 0)    //未接收到前后侧状态进入while
		{
			Send_ZigbeeData_To_Fifo(Temp,8);    //发送请求回传前后侧状态命令
			delay_ms(150);
			TimeOut++;
			if(TimeOut > 5)  //一定要加多次请求判断循环，才能确保发送了请求回传命令，防止倒车入库失败
			{
				break;    //超时2.5s后，退出此循环，防止卡死
			}
		}
		Receive_A_State = Communication_Data.CarPort_Back_A_Infrared;
		Communication_Data.CarPort_Back_A_Infrared = 0;   //接收到的前后测状态清零
        return Receive_A_State;
	}
	else if(device == 0x05) //设备B
	{
		while(Communication_Data.CarPort_Back_B_Infrared == 0)    //未接收到前后侧状态进入while
		{
			Send_ZigbeeData_To_Fifo(Temp,8);     //发送请求回传前后侧状态命令
			delay_ms(150);
			TimeOut++;
			if(TimeOut > 5)  //一定要加多次请求判断循环，才能确保发送了请求回传命令，防止倒车入库失败
			{
				break;    //超时2.5s后，退出此循环，防止卡死
			}
		}
		Receive_B_State = Communication_Data.CarPort_Back_B_Infrared;
		Communication_Data.CarPort_Back_B_Infrared = 0;   //接收到的前后测状态清零
		return Receive_B_State;
	}
	return 0;
}


/*
指定车库到达第几层，同时等车库稳定才可以进行下一步（加延时）
参数：相差几层（如：当前位于一层，想去三层，则相差两层，则写入2）
功能：加入延时，保证立体车库稳定后才进入下一步
*/

void xCarPort_Control_Arrive_Delay(uint8_t delay_num)
{
	switch(delay_num)
	{
		case 0:    //相差0层
		{
			break;  
		}
		case 1:   //相差1层
		{
			for(uint8_t i=0;i<Differ_1_Delay;i++)
			{
				delay_ms(500);
				delay_ms(500);
			}
			break;
		}
		case 2:   //相差2层
		{
			for(uint8_t i=0;i<Differ_2_Delay;i++)
			{
				delay_ms(500);
				delay_ms(500);
			}
			break;
		}
		case 3:   //相差3层
		{
			for(uint8_t i=0;i<Differ_3_Delay;i++)
			{
				delay_ms(500);
				delay_ms(500);
			}
			break;
		}
    default: break;
	}
	return;
}


/*
倒车入库


*/
void xCarPort_CarBack_Into(uint8_t device)
{
	Motor_Data.xCAR_Track_Time(35,750);
	delay_ms(200);
	Motor_Data.xCAR_Back(35,600);
	delay_ms(200);
	Motor_Data.xCAR_Track_Time(35,700);
	delay_ms(200);
	Motor_Data.xCAR_Back(35,600);
	delay_ms(200);
	Motor_Data.xCAR_Track_Time(35,600);
	delay_ms(200);
	Motor_Data.xCAR_Back(35,500);
	Motor_Data.xCAR_Back(35,600);
	uint8_t Back_Flag = 0;
	while(Back_Flag != 3)   //工作室为3，比赛为4
	{
		Motor_Data.xCAR_Back(40,250);
		Back_Flag = CarPort_Data.xCarPort_Ack_Infrared_State(device);
	}
	Motor_Data.xCAR_Go(30,200);
}
