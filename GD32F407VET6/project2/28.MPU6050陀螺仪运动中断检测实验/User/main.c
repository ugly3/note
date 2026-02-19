/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "systick.h"
#include "oled.h"
#define I2C1_SLAVE_ADDRESS7    0x46
void i2c_config(void)
{
    /* configure I2C1 clock */
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

short Accel[3];
short Gyro [3];
short Temp;
long AccelData[3];//单位mg
long GyroData[3];//单位mdps
uint8_t IntFlag;//MPU6050中断标志
uint8_t CleanTime,LedFlag=0;
int main(void)
{
		int i;
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4配置成输出
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4配置成推挽输出，50M速度
		gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6);//复用功能4
		gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_7);//复用功能4
	  gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_6);//PB6,PB7配置成I2C
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
		OLED_Init();//OLED初始化  
		OLED_Clear();//OLED清屏 
		rcu_periph_clock_enable(RCU_I2C0);//I2C0时钟使能
		i2c_config();
		//////////////////////////外部中断配置//////////////////////////////////
		rcu_periph_clock_enable(RCU_GPIOA);
		gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);//PA5配置成下拉输入
		nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);//中断使能
		/* configure key EXTI line */
		exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);//上升沿触发
		exti_interrupt_flag_clear(EXTI_5);
		delay_1ms(180);//等待180ms
		MPU6050_Init();//MPU6050初始化配置，中断使能配置
	if(MPU6050ReadID() == 0)//读取MPU6050 ID
	{	
	  while(1);
  }
	while(1)
	{
			CleanTime++;
			if(CleanTime>=20)
			{
				CleanTime=0;
				OLED_Clear();//清屏
				if(IntFlag)
				{
					LedFlag^=0x01;
					if(LedFlag)
					{
					gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4输出高电平
					}else
					{
					gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4输出低电平
					}
				}
			}
			OLED_ShowCHinese(18,0,0);//光
			OLED_ShowCHinese(36,0,1);//子
			OLED_ShowCHinese(54,0,2);//物
			OLED_ShowCHinese(72,0,3);//联
			OLED_ShowCHinese(90,0,4);//网
			MPU6050ReadAcc(Accel);//读取加速度数据
			for(i=0;i<3;i++)
			{
			if(Accel[i]>=0)
			{
				AccelData[i]=Accel[i]*2000/32768;//转换成单位mg
			}else
			{
				AccelData[i]=-(-Accel[i]+1)*2000/32768;
			}		
		}
		if(AccelData[0]<0)//判断是否为负数
		{
			OLED_ShowString(0,3,"-");
			OLED_ShowNum(8,3,-AccelData[0],4,16);//加速度x	
		}else
		{
			OLED_ShowNum(8,3,AccelData[0],4,16);//加速度x
		}
		
		if(AccelData[1]<0)//判断是否为负数
		{
			OLED_ShowString(40,3,"-");
			OLED_ShowNum(48,3,-AccelData[1],4,16);//加速度y
		}else
		{
			OLED_ShowNum(48,3,AccelData[1],4,16);//加速度y
		}
		
		if(AccelData[2]<0)//判断是否为负数
		{
			OLED_ShowString(80,3,"-");
			OLED_ShowNum(88,3,-AccelData[2],4,16);//加速度z
		}else
		{
			OLED_ShowNum(88,3,AccelData[2],4,16);//加速度z
		}
			MPU6050ReadGyro(Gyro);//读取陀螺仪数据	 
			for(  i=0;i<3;i++)
			{
			if(Gyro[i]>=0)
			{
				GyroData[i]=Gyro[i]*2000/32768;//转换成单位mdps
			}else
			{
				GyroData[i]=-(-Gyro[i]+1)*2000/32768;
			}
		 }		
			if(GyroData[0]<0)//判断是否为负数
		{
			OLED_ShowString(0,6,"-");
			OLED_ShowNum(8,6,-GyroData[0],4,16);//陀螺仪x轴
		}else
		{
			OLED_ShowNum(8,6,GyroData[0],4,16);//陀螺仪x轴
		}
		if(GyroData[1]<0)//判断是否为负数
		{
			OLED_ShowString(40,6,"-");
			OLED_ShowNum(48,6,-GyroData[1],4,16);//陀螺仪y轴
		}else
		{
			OLED_ShowNum(48,6,GyroData[1],4,16);//陀螺仪y轴
		}
		
		if(GyroData[2]<0)//判断是否为负数
		{
			OLED_ShowString(80,6,"-");
			OLED_ShowNum(88,6,-GyroData[2],4,16);//陀螺仪z轴
		}else
		{
			OLED_ShowNum(88,6,GyroData[2],4,16);//陀螺仪z轴
		}
	}		
 
}
