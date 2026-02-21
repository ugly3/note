/**
************************************************************************
 *    文件名：motor_drive（主车电机驱动相关函数）
 *      说明：主车的循迹，前进后退，左右90度转，左右50度转，
************************************************************************
**/
#include "motor_drive.h"
#include "roadway_check.h"   //道路检测


Motor_Typedef Motor_Data =
{
	.Track_White_Flag = 0,
	.xLR45_time = 380,
	.xLR45_speed = 90,
	.xLR90_time = 750,
	.xLR90_speed = 90,
	.GO_Time = 400,
	.Go_speed = 40,
	.Go_Mp_disance = 300,
	.xCAR_Track = &xCAR_Track,
	.xCAR_Track_Time = &xCAR_Track_Time,
	.xCAR_Go = &xCAR_Go,
	.xCAR_Back = &xCAR_Back,
	.xCAR_Track_Go = &xCAR_Track_Go,
	.xCAR_L90 = &xCAR_L90,
	.xCAR_R90 = &xCAR_R90,
	.xCAR_L45 = &xCAR_L45,
	.xCAR_R45 = &xCAR_R45,
	.xCAR_Track_Correct_White = &xCAR_Track_Correct_White,
	.xCAR_Track_White = &xCAR_Track_White,
};


#if CAR_CODE
void Car_Go(uint8_t speed, uint16_t temp)   // 主车前进 参数：速度/码盘
{
    Roadway_mp_syn();       // 码盘同步
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 1;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    temp_MP = temp;         // 码盘值
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数
    while(Stop_Flag != 0x03);       // 等待前进完成     //使用码盘判断的时候 Stop_Flag 为3
}

void Car_Back(uint8_t speed, uint16_t temp) // 主车后退 参数：速度/码盘
{
    Roadway_mp_syn();       // 码盘同步
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 1;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    temp_MP = temp;         // 码盘值
    Car_Spend = speed;      // 速度值
    Control(-Car_Spend, -Car_Spend); // 电机驱动函数   Car_Spend 正值向前走，负值向后走
    while(Stop_Flag != 0x03);       // 等待后退完成
}
#endif

/**************************主车电机运行******************************************/

/*
主车循迹
参数1：速度
*/
void xCAR_Track(uint8_t speed)
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 1;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数 
    while(Stop_Flag != 0x01);       // 等待循迹完成	
}

/*
主车时间循迹（循迹指定时间）
参数1：
参数2：
*/
void xCAR_Track_Time(uint8_t speed,uint16_t time)
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 1;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数
	
	  while(time >= 700)
		{
			delay_ms(700);
			time -= 700;
		}
		delay_ms(time);
	  Track_Flag = 0;
    Send_UpMotor(0,0);
    delay_ms(100);	
}


/*
主车前进
参数1: 速度
参数2：码盘值
*/
//void xCAR_Go(uint8_t speed,uint8_t temp)
//{
//    Roadway_mp_syn();       // 码盘同步
//    Stop_Flag = 0;          // 运行状态标志位
//    Go_Flag = 1;            // 前进标志位
//    wheel_L_Flag = 0;       // 左转标志位
//    wheel_R_Flag = 0;       // 右转标志位
//    wheel_Nav_Flag = 0;     // 码盘旋转标志位
//    Back_Flag = 0;          // 后退标志位
//    Track_Flag = 0;         // 循迹标志位
//    temp_MP = temp;         // 码盘值
//    Car_Spend = speed;      // 速度值
//    Control(Car_Spend, Car_Spend);  // 电机驱动函数
//    while(Stop_Flag != 0x03);       // 等待前进完成     //使用码盘判断的时候 Stop_Flag 为3	
//}

/*
主车前进
参数1: 速度
参数2：时间（电机运行时间）
*/
void xCAR_Go(uint8_t speed,uint16_t time)
{
//	delay_ms(100);
	Car_Spend = speed;
	Control(Car_Spend,Car_Spend);
	delay_ms(time);
	Send_UpMotor(0,0);     //停车
//	delay_ms(100);
}

/*
主车后退
参数1: 速度
参数2：时间（电机运行时间）
*/
void xCAR_Back(uint8_t speed,uint16_t time)
{
//	delay_ms(100);
	Car_Spend = speed;
	Control(-Car_Spend,-Car_Spend);
	while(time >= 700)
	{
		delay_ms(700);
		time -= 700;
	}
	delay_ms(time);
	Send_UpMotor(0,0);     //停车
//	delay_ms(100);
}



/*
主车循迹完成后再往前走一段距离
参数：无
*/
void xCAR_Track_Go(void)
{
	delay_ms(100);
	Motor_Data.xCAR_Track(Motor_Data.Go_speed);   //循迹
	delay_ms(100);
	Motor_Data.xCAR_Go(30,400);   //前进
	delay_ms(100);
}

/*
主车左转90度
参数1：速度
参数2：延时时间（用来作角度）
*/
void xCAR_L90(uint8_t speed,uint16_t time)
{
	delay_ms(100);
	Car_Spend = speed;
	Control(-Car_Spend,Car_Spend);		// 电机驱动函数
	delay_ms(time);			    // 延时作角度
	Send_UpMotor(0,0);			// 停车
	delay_ms(100);
}

/*
主车右转90度
参数1：速度
参数2：延时时间（用来作角度）
*/
void xCAR_R90(uint8_t speed,uint16_t time)
{
	delay_ms(100);
	Car_Spend = speed;
	Control(Car_Spend,-Car_Spend);		// 电机驱动函数
	delay_ms(time);			    // 延时作角度
	Send_UpMotor(0,0);			// 停车
	delay_ms(100);
}



/*
主车左转45度
参数1：速度
参数2：延时时间（用来作角度）
*/
void xCAR_L45(uint8_t speed,uint16_t time)
{
	delay_ms(100);
	Car_Spend = speed;
	Control(-Car_Spend,Car_Spend);		// 电机驱动函数
	delay_ms(time);			    // 延时作角度
	Send_UpMotor(0,0);			// 停车
	delay_ms(100);	
}

/*
主车右转45度
参数1：速度
参数2：延时时间（用来作角度）
*/
void xCAR_R45(uint8_t speed,uint16_t time)
{
	delay_ms(100);
	Car_Spend = speed;
	Control(Car_Spend,-Car_Spend);		// 电机驱动函数
	delay_ms(time);			    // 延时作角度
	Send_UpMotor(0,0);			// 停车
	delay_ms(100);	
}



/*
主车特殊地形循迹判断（遇到白线停车）  （未写完）
*/
void xCAR_Track_Correct_White(uint8_t gd)
{
	if(gd == 0xFF)	// 循迹灯全亮 停止           1为亮，0为灭   1为白线，0为黑线
	{
        Motor_Data.Track_White_Flag = 0;
		Stop_Flag = 1;
		Send_UpMotor(0,0); 	
	}
	else if(gd == 0xE7)                             //E7---1110 0111  车在中间，正常行驶
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
	} 
	else if((gd == 0xF7) || (gd == 0XF3))           //F7---1111 0111   F3---1111 0011  车偏左
	{
		LSpeed = Car_Spend + 20;
		RSpeed=Car_Spend - 40;
	} 
	else if((gd == 0XFB) || (gd == 0XF9))           //FB---1111 1011    F9---1111 1001 车偏左+
	{
		LSpeed = Car_Spend + 40;
		RSpeed = Car_Spend - 60;
	} 
	else if((gd == 0XFD) || (gd == 0XFC))           //FD---1111 1101    FC---1111 1100  车偏左++
	{
		LSpeed = Car_Spend + 60;
		RSpeed = Car_Spend - 90;
	} else if(gd == 0XFE)                           //FE---1111 1110    车偏左（FULL）
	{
		LSpeed = Car_Spend + 80;
		RSpeed = Car_Spend - 120;
	} 
	else if((gd == 0XEF) || (gd == 0XCF))           //EF---1110 1111      CF---1100 1111  车偏右
	{
		RSpeed = Car_Spend + 20;
		LSpeed = Car_Spend - 40;
	} 
	else if((gd == 0XDF) || (gd == 0X9F))          //DF---1101 1111       9F---1001 1111  车偏右+
	{
		RSpeed = Car_Spend + 40;
		LSpeed = Car_Spend - 60;
	} 
	else if((gd == 0XBF) || (gd == 0X3F))          //BF---1011 1111       3F---0011 1111   车偏右++
	{
		RSpeed = Car_Spend + 60;
		LSpeed = Car_Spend - 90;
	} 
	else if(gd == 0X7F)                          //7F---0111 1111        车偏右（FULL）
	{
		RSpeed = Car_Spend + 80;
		LSpeed = Car_Spend - 120;
	}  
	else    //正常行驶
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
	}
// 要加入一下代码，才能实现循迹调整主车位置!!!	
	if(gd==0xFF)   //循迹灯全亮     
	{
		LSpeed = Car_Spend;
		RSpeed = Car_Spend;
		if(count > 1200)
		{
			count=0;
			Send_UpMotor(0,0);
			Track_Flag=0;
			Stop_Flag = 4;
		}
		else {
			count++;
		}			
	}
	else 
	{
		count=0;
	}		
	if(Motor_Data.Track_White_Flag != 0)
	{
		Control(LSpeed,RSpeed);
	}	
}

/*
循迹特殊地形标志物（循迹到白线停车）
参数：
*/
void xCAR_Track_White(uint8_t speed)
{
	Stop_Flag = 0;
	Motor_Data.Track_White_Flag = 1;
	Car_Spend = speed;
	Control(Car_Spend,Car_Spend);
	while(Stop_Flag != 0x01);       // 等待循迹完成	
}

