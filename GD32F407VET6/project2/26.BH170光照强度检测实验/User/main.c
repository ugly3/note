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
//写入1字节数据到BH170,设备地址0x46
void BH170_WriteReg(uint8_t reg_add)
{

	  /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C0, I2C1_SLAVE_ADDRESS7, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set*/
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
 

		/* send a data byte */
		i2c_data_transmit(I2C0,reg_add);
		/* wait until the transmission data register is empty*/
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
		 i2c_stop_on_bus(I2C0);
		 /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
  
}

//读取BH170数据,设备地址0x46
void BH170_ReadData(unsigned char*Read)
{

	
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, I2C1_SLAVE_ADDRESS7, I2C_RECEIVER);
    /* disable ACK before clearing ADDSEND bit */
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* Wait until the last data byte is received into the shift register */
    /* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));

    /* read a data from I2C_DATA */
    Read[0] = i2c_data_receive(I2C0);

    /* read a data from I2C_DATA */
    Read[1] = i2c_data_receive(I2C0);
    /* send a stop condition */
    i2c_stop_on_bus(I2C0);

	
}
uint8_t DataBuff[2];//存储数组
float LightData;//光照强度（含小数位，单位lx）
uint32_t LightData_Hex;//光照强度（整数，单位lx）
int main(void)
{
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		rcu_periph_clock_enable(RCU_GPIOB);//GPIOB时钟使能
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
		BH170_WriteReg(0x01);// power on
		BH170_WriteReg(0x10);//H- resolution mode
		delay_1ms(180);//等待180ms
	while(1) 
	{		
		BH170_WriteReg(0x01);// power on
		BH170_WriteReg(0x10);//H- resolution mode
		delay_1ms(180);//等待180ms
		BH170_ReadData(DataBuff);//读取数据
		LightData=((DataBuff[0]<<8)+DataBuff[1])/1.2f;//数据转换成光强度，单位lx
    LightData_Hex=LightData;//float转换成整数
		OLED_ShowNum(6,3,LightData_Hex,5,16);//显示光强度
		OLED_ShowString(72,3,"lx");//显示光强度单位lx
		OLED_ShowCHinese(18,0,0);//光
		OLED_ShowCHinese(36,0,1);//子
		OLED_ShowCHinese(54,0,2);//物
		OLED_ShowCHinese(72,0,3);//联
		OLED_ShowCHinese(90,0,4);//网
	}	  
 
}
