#include "iic.h"

iic_bus_typ hiic1={
	.SDA_RTC=RCU_GPIOB,
	.SDA_Port=GPIOB,
	.SDA_PIN=GPIO_PIN_11,
	
	.SCL_RTC=RCU_GPIOB,
	.SCL_Port=GPIOB,
	.SCL_PIN=GPIO_PIN_10,
};




void IIC_config(iic_bus_typ *hiic){
	
	rcu_periph_clock_enable(hiic->SCL_RTC);
	gpio_mode_set(hiic->SCL_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, hiic->SCL_PIN);
	gpio_output_options_set(hiic->SCL_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, hiic->SCL_PIN);	
	gpio_bit_write(hiic->SCL_Port, hiic->SCL_PIN, RESET);
	
	rcu_periph_clock_enable(hiic->SDA_RTC);
	gpio_mode_set(hiic->SDA_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, hiic->SDA_PIN);
	gpio_output_options_set(hiic->SDA_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, hiic->SDA_PIN);	
	gpio_bit_write(hiic->SDA_Port, hiic->SDA_PIN, RESET);
	
}



//设置SCL电平状态
void SCL_Write(iic_bus_typ *hiic,int level){
	gpio_bit_write(hiic->SCL_Port, hiic->SCL_PIN, (bit_status)level);

}
//设置SDA电平状态
void SDA_Write(iic_bus_typ *hiic,int level){
	gpio_bit_write(hiic->SDA_Port, hiic->SDA_PIN, (bit_status)level);
}


//读取电平状态
int SDA_Read(iic_bus_typ *hiic){
	return (int)gpio_input_bit_get(hiic->SDA_Port,hiic->SDA_PIN);
}
#define iic_in 0
#define iic_out 1


//设置电平方向
int SDA_Mode(iic_bus_typ *hiic,int mode){

	if(mode==iic_out){  //输出
		gpio_mode_set(hiic->SDA_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, hiic->SDA_PIN);
		gpio_output_options_set(hiic->SDA_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, hiic->SDA_PIN);	
	}else if(mode==iic_in){     //输入
		gpio_mode_set(hiic->SDA_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, hiic->SDA_PIN);
	}
	delay_us(5);	
	return 1;
}



//产生IIC起始信号
void IIC_Start(iic_bus_typ *hiic)
{
	SDA_Mode(hiic,iic_out);		//sda线输出
	SDA_Write(hiic,1);	  
	delay_us(5);	
	SCL_Write(hiic,1);

	delay_us(5);	
 	SDA_Write(hiic,0);	//START:when CLK is high,DATA change form high to low 
	delay_us(5);	
	SCL_Write(hiic,0);	//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(iic_bus_typ *hiic)
{
	SDA_Mode(hiic,iic_out);		//sda线输出
	SCL_Write(hiic,0);
	SDA_Write(hiic,0);	//STOP:when CLK is high DATA change form low to high
	delay_us(5);	
	SCL_Write(hiic,1); 
	SDA_Write(hiic,1);	//发送I2C总线结束信号
	delay_us(5);								   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(iic_bus_typ *hiic)
{
	uint16_t ucErrTime=0;
	SDA_Mode(hiic,iic_in);		//SDA设置为输入  
	SDA_Write(hiic,1);delay_us(1);	   
	SCL_Write(hiic,1);delay_us(1);	 
	while(SDA_Read(hiic))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(hiic);
			return 1;
		}
	}
	SCL_Write(hiic,0);		//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(iic_bus_typ *hiic)
{
	SCL_Write(hiic,0);
	SDA_Mode(hiic,iic_out);
	SDA_Write(hiic,0);
	delay_us(2);	
	SCL_Write(hiic,1);
	delay_us(5);	
	SCL_Write(hiic,0);
}
//不产生ACK应答		    
void IIC_NAck(iic_bus_typ *hiic)
{
	
	SCL_Write(hiic,0);
	SDA_Mode(hiic,iic_out);
	SDA_Write(hiic,1);
	delay_us(2);	
	SCL_Write(hiic,1);
	delay_us(5);	
	SCL_Write(hiic,0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(iic_bus_typ *hiic,uint8_t txd)
{                        
    uint8_t t;   
		SDA_Mode(hiic,iic_out); 	    
    SCL_Write(hiic,0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              	
		if((txd&0x80)>>7){
			SDA_Write(hiic,1);
		}else{
			SDA_Write(hiic,0);
		}
		txd<<=1; 	
		delay_us(2);	
		SCL_Write(hiic,1);
		delay_us(2);
		SCL_Write(hiic,0);	
//		delay_us(2);	
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(iic_bus_typ *hiic,unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_Mode(hiic,iic_in);//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		SCL_Write(hiic,0); 
		delay_us(2);	
		SCL_Write(hiic,1);
		receive<<=1;
		if(SDA_Read(hiic))receive++;   
		delay_us(1);	
		}					 
    if (!ack)
        IIC_NAck(hiic);//发送nACK
    else
        IIC_Ack(hiic); //发送ACK   
    return receive;
}



void IIC_Master_Transmit(iic_bus_typ *hiic ,uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    IIC_Start(hiic); // 发送开始信号
    IIC_Send_Byte(hiic,(uint8_t)(DevAddress )); // 发送设备地址加写操作位
    IIC_Wait_Ack(hiic); // 等待应答
    while (Size--) // 发送数据
    {
        IIC_Send_Byte(hiic,*pData++); // 发送一个字节
        IIC_Wait_Ack(hiic); // 等待应答
    }
    IIC_Stop(hiic); // 发送停止信号
}

void IIC_Mem_Write(iic_bus_typ *hiic ,uint16_t DevAddress,uint16_t MemAddress,uint8_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    IIC_Start(hiic); // 发送开始信号
    IIC_Send_Byte(hiic,(uint8_t)(DevAddress )); // 发送设备地址
    IIC_Wait_Ack(hiic); // 等待应答
		if(MemAddSize==2){
			IIC_Send_Byte(hiic,(uint8_t)(MemAddress>>8 )); // 发送寄存器地址高8位
			IIC_Wait_Ack(hiic); // 等待应答
		}
		IIC_Send_Byte(hiic,(uint8_t)(MemAddress&0x00ff)); // 发送设备地址低8位
		IIC_Wait_Ack(hiic); // 等待应答
    while (Size--) // 发送数据
    {
        IIC_Send_Byte(hiic,*pData++); // 发送一个字节
        IIC_Wait_Ack(hiic); // 等待应答
    }
    IIC_Stop(hiic); // 发送停止信号
}


uint8_t IIC_Master_Receive(iic_bus_typ *hiic, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    IIC_Start(hiic); // 发送开始信号
    IIC_Send_Byte(hiic,(uint8_t)(DevAddress )); // 发送设备地址加读操作位
		//IIC_Send_Byte(hiic,DevAddress); // 发送设备地址加读操作位
    if (IIC_Wait_Ack(hiic)) // 等待应答
    {
        IIC_Stop(hiic); // 如果没有应答，则发送停止信号并退出
        return 1; // 表示失败
    }
    while (Size)
    {
        if (Size == 1)
        {
            *pData = IIC_Read_Byte(hiic,0); // 读取最后一个字节，发送非应答信号
        }
        else
        {
            *pData = IIC_Read_Byte(hiic,1); // 读取字节，发送应答信号
        }
        pData++;
        Size--;
    }
    IIC_Stop(hiic); // 发送停止信号
	
    return 0; // 表示成功
}

