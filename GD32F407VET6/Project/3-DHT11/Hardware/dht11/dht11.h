#ifndef _DHT11_H
#define _DHT11_H

#include "gd32f4xx.h"
#include "systick.h"

//IO引脚
#define DHT11_RTC 		RCU_GPIOD
#define DHT11_GPIO_Port	GPIOD
#define DHT11_Pin 		GPIO_PIN_7

//IO方向设置
#define DHT11_IO_IN()    DHT11_SDA_Mode(1)
#define DHT11_IO_OUT()   DHT11_SDA_Mode(0)

////IO操作函数			  					    			  
#define	DHT11_DQ_OUT_1 gpio_bit_write(DHT11_GPIO_Port, DHT11_Pin, SET)
#define	DHT11_DQ_OUT_0 	gpio_bit_write(DHT11_GPIO_Port, DHT11_Pin, RESET)

#define	DHT11_DQ_IN()  	gpio_input_bit_get(DHT11_GPIO_Port, DHT11_Pin)


uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);	//读取温湿度
uint8_t DHT11_Read_Byte(void);				//读出一个字节
uint8_t DHT11_Read_Bit(void);				//读出一个位
uint8_t DHT11_Check(void);					//检测是否存在DHT11

int DHT11_SDA_Mode(int mode);

void DHT11_Rst(void);						//复位DHT11   
void DHT11_Init(void);



#endif  /* _USART_H */




