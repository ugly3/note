
/***************************************************************************//**
  文件: 24C08.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20200401
	平台:MINI-GD32F407

*******************************************************************************/
#include "gd32f4xx.h"
#include "systick.h"

#define	EE_ADDR 0xa0		

 void EE_IIC_Delay(uint16_t us);


#define EE_SCL_PIN  GPIO_PIN_6   //模拟IIC的SCL信号  1.修改引脚即可修改IIC接口
#define EE_SDA_PIN  GPIO_PIN_7   //模拟IIC的SDA信号

void EE_SDA_IN(void) 	//配置成输入  
	{  
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_7);//PB7配置成输入
		EE_IIC_Delay(10);
} 
void EE_SDA_OUT(void)//配置成输出
{	
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);//PB7配置成输出
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		EE_IIC_Delay(10);
}

void EE_SCK_OUT(void) //配置成输出
{
		rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);//PB6配置成输出
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		EE_IIC_Delay(10);

}


void EE_IIC_SCL(uint8_t val)        
{	gpio_bit_write(GPIOB, GPIO_PIN_6,val);                   //SCL      2.修改引脚即可修改IIC接口 
	EE_IIC_Delay(2);
}	
void EE_IIC_SDA(uint8_t val)         
{	gpio_bit_write(GPIOB, GPIO_PIN_7,val);                    //SDA	
	EE_IIC_Delay(2);
}




unsigned char EE_READ_SDA(void)
{
  return gpio_input_bit_get(GPIOB,GPIO_PIN_7);//读取PB7引脚电平
}	



/******************************************************************************
*函  数：void EE_IIC_Delay(void)
*功　能：IIC延时
*参  数：无
*返回值：无
*备  注: 移植时只需要将EE_IIC_Delay()换成自己的延时即可
*******************************************************************************/	
void EE_IIC_Delay(uint16_t us)
{
		int i,j;
		for( i = 0; i < us; i++)    
		{
			for( j=0;j<36;j++)
			{

			}
		}

}
/******************************************************************************
*函  数：void IIC_Init(void)
*功　能：IIC初始化
*参  数：无
*返回值：无
*备  注：无
*******************************************************************************/

void EE_IIC_Init(void)
{			

    EE_SCK_OUT();
    EE_SDA_OUT();
    EE_IIC_SCL(1);
    EE_IIC_SDA(1);
        
}
	
void EE_IIC_Start(void)
{
	EE_IIC_Delay(10);
	EE_SDA_OUT(); //sda线输出 
	EE_IIC_Delay(4);
	EE_IIC_SDA(1);	
	EE_IIC_SCL(1);
	EE_IIC_Delay(4);
 	EE_IIC_SDA(0); //START:when CLK is high,DATA change form high to low 
	EE_IIC_Delay(4);
	EE_IIC_SCL(0); //钳住I2C总线，准备发送或接收数据
	EE_IIC_Delay(10);	
}

	  
void EE_IIC_Stop(void)
{
	EE_IIC_Delay(4);
	EE_SDA_OUT(); //sda线输出
	EE_IIC_Delay(4);
	EE_IIC_SCL(0);
	EE_IIC_SDA(0); //STOP:when CLK is high DATA change form low to high
  EE_IIC_Delay(4);//等待
	EE_IIC_SCL(1); 
	EE_IIC_SDA(1); //发送I2C总线结束信号
  EE_IIC_Delay(4);							   	
}


uint8_t EE_IIC_WaitAck(void)
{
	uint8_t ucErrTime=0;
	EE_SDA_IN(); //SDA设置为输入  （从机给一个低电平做为应答）
	EE_IIC_Delay(4);	
	EE_IIC_SDA(1);
	EE_IIC_Delay(4);	   
	EE_IIC_SCL(1);
	EE_IIC_Delay(4);
	while(EE_READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			EE_IIC_Stop();
			return 1;
		}
	}
	EE_IIC_SCL(0); //时钟输出0 	
	EE_IIC_Delay(4);	
	return 0;  
} 



void EE_IIC_Ack(void)
{
	EE_IIC_SCL(0);
	EE_SDA_OUT();
	EE_IIC_Delay(4);
	EE_IIC_SDA(0);
	EE_IIC_Delay(4);
	EE_IIC_SCL(1);
	EE_IIC_Delay(4);
	EE_IIC_SCL(0);
	EE_IIC_Delay(4);
}


void EE_IIC_NAck(void)
{
	EE_IIC_SCL(0);
	EE_SDA_OUT();
	EE_IIC_Delay(4);
	EE_IIC_SDA(1);
	EE_IIC_Delay(1);
	EE_IIC_SCL(1);
	EE_IIC_Delay(1);
	EE_IIC_SCL(0);
	EE_IIC_Delay(4);
}					 				     

	  
void EE_IIC_SendByte(uint8_t data)
{                        
    uint8_t t;   
    EE_SDA_OUT(); 
	  EE_IIC_Delay(4);	
    EE_IIC_SCL(0); //拉低时钟开始数据传输
		EE_IIC_Delay(4);
    for(t=0;t<8;t++)
    {              
        EE_IIC_SDA((data&0x80)>>7);
        EE_IIC_Delay(5);			
        EE_IIC_SCL(1);
        data<<=1;
        EE_IIC_Delay(5);
        EE_IIC_SCL(0);	   
    }
    EE_IIC_Delay(5);
} 	 

	
uint8_t EE_IIC_ReadByte(uint8_t ack)
{
	uint8_t i,receive=0;
	EE_SDA_IN(); //SDA设置为输入模式 等待接收从机返回数据
		EE_IIC_Delay(4);
    for(i=0;i<8;i++ )
	{
        EE_IIC_SCL(0); 
        EE_IIC_Delay(1);
        EE_IIC_SCL(1);
        receive<<=1;
        if(EE_READ_SDA())receive++; //从机发送的电平
        EE_IIC_Delay(1); 
    }					 
    if(ack)
        EE_IIC_Ack(); //发送ACK 
    else
        EE_IIC_NAck(); //发送nACK  
    return receive;
}


uint8_t EE_IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf)
{
	EE_IIC_Start();	
	EE_IIC_Delay(4);
	EE_IIC_SendByte(I2C_Addr);	 //发送从机地址
	if(EE_IIC_WaitAck()) //如果从机未应答则数据发送失败
	{
		EE_IIC_Stop();
		return 1;
	}
	EE_IIC_SendByte(reg); //发送寄存器地址
	EE_IIC_WaitAck();	  
	
	EE_IIC_Start();
	EE_IIC_SendByte(I2C_Addr+1); //进入接收模式			   
	EE_IIC_WaitAck();
	*buf=EE_IIC_ReadByte(0);	   
   EE_IIC_Stop(); //产生一个停止条件
	return 0;
}


uint8_t EE_EE_IIC_SendByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data)
{
	EE_IIC_Start();
	EE_IIC_Delay(4);
	EE_IIC_SendByte(I2C_Addr); //发送从机地址
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop();
		return 1; //从机地址写入失败
	}
	EE_IIC_SendByte(reg); //发送寄存器地址
        EE_IIC_WaitAck();	  
	EE_IIC_SendByte(data); 
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop(); 
		return 1; //数据写入失败
	}
	EE_IIC_Stop(); //产生一个停止条件
    
	return 0;
}


