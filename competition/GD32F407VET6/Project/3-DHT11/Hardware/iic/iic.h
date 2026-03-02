#ifndef __my_iic_H
#define __my_iic_H

#include "gd32f4xx.h"
#include "systick.h"


typedef struct {
	rcu_periph_enum SDA_RTC;
	uint32_t SDA_PIN;
	uint32_t SDA_Port;
	
	rcu_periph_enum SCL_RTC;
	uint32_t SCL_PIN;
	uint32_t SCL_Port;
	
}iic_bus_typ;

extern iic_bus_typ hiic1;


//IIC所有操作函数

void IIC_config(iic_bus_typ *hiic);
		 
void IIC_Start(iic_bus_typ *hiic);				        //发送IIC开始信号
void IIC_Stop(iic_bus_typ *hiic);	  			        //发送IIC停止信号
void IIC_Send_Byte(iic_bus_typ *hiic,uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(iic_bus_typ *hiic,unsigned char ack);   //IIC读取一个字节
uint8_t IIC_Wait_Ack(iic_bus_typ *hiic); 				//IIC等待ACK信号
void IIC_Ack(iic_bus_typ *hiic);					        //IIC发送ACK信号
void IIC_NAck(iic_bus_typ *hiic);				        //IIC不发送ACK信号

void IIC_Master_Transmit(iic_bus_typ *hiic,uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void IIC_Mem_Write(iic_bus_typ *hiic ,uint16_t DevAddress,uint16_t MemAddress,uint8_t MemAddSize, uint8_t *pData, uint16_t Size);
uint8_t IIC_Master_Receive(iic_bus_typ *hiic,uint16_t DevAddress, uint8_t *pData, uint16_t Size);	  


#endif 
