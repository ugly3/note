#include "BH1750.h"
#include "systick.h"


//从机BH1750设备地址
#define I2C1_SLAVE_ADDRESS7    0x46

void i2cx_config(void)
{
    /* configure I2C1 clock */
    i2c_clock_config(I2CX, 100000, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2CX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2CX);
    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
}
//写入1字节数据到BH170,设备地址0x46
void BH170_WriteReg(uint8_t reg_add)
{

	  /* wait until I2C bus is idle */
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2CX);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus（将从地址发送到I2C总线）*/
    i2c_master_addressing(I2CX, I2C1_SLAVE_ADDRESS7, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set*/
    while(!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
 

		/* send a data byte */
		i2c_data_transmit(I2CX,reg_add);
		/* wait until the transmission data register is empty*/
		while(!i2c_flag_get(I2CX, I2C_FLAG_TBE));
		 i2c_stop_on_bus(I2CX);
		 /* wait until the stop condition is finished */
    while(I2C_CTL0(I2CX)&0x0200);
  
}
//读取BH170数据,设备地址0x46
void BH170_ReadData(unsigned char*Read)
{
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2CX);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2CX, I2C1_SLAVE_ADDRESS7, I2C_RECEIVER);
    /* disable ACK before clearing ADDSEND bit */
    i2c_ack_config(I2CX, I2C_ACK_DISABLE);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
    /* Wait until the last data byte is received into the shift register */
    /* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_RBNE));

    /* read a data from I2C_DATA */
    Read[0] = i2c_data_receive(I2CX);

    /* read a data from I2C_DATA */
    Read[1] = i2c_data_receive(I2CX);
    /* send a stop condition */
    i2c_stop_on_bus(I2CX);
}

void BH1750_Init(void)
{
    rcu_periph_clock_enable(BH1750_RCU_GPIOX);//GPIOB时钟使能
    gpio_af_set(BH1750_PORT, GPIO_AF_4, BH1750_PIN_SCL);//复用功能4，GPIO_AF_4配置成I2C
    gpio_af_set(BH1750_PORT, GPIO_AF_4, BH1750_PIN_SDA);//复用功能4，GPIO_AF_4置成I2C
    gpio_mode_set(BH1750_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,BH1750_PIN_SCL);
    gpio_output_options_set(BH1750_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,BH1750_PIN_SCL);
    gpio_mode_set(BH1750_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,BH1750_PIN_SDA);
    gpio_output_options_set(BH1750_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,BH1750_PIN_SDA);
    rcu_periph_clock_enable(BH1750_RCU_I2CX);//I2C0时钟使能
    i2cx_config();
    BH170_WriteReg(0x01);// power on（开机）
    BH170_WriteReg(0x10);//H- resolution mode(H-分辨率模式)
    delay_1ms(180);//等待180ms
}

uint8_t DataBuff[2];//存储数组
float LightData;//光照强度（含小数位，单位lx）
uint32_t LightData_Hex;//光照强度（整数，单位lx）

void BH1750_Get_Data(void)
{
    BH170_WriteReg(0x01);// power on
    BH170_WriteReg(0x10);//H- resolution mode
    delay_1ms(180);//等待180ms
    BH170_ReadData(DataBuff);//读取数据
    LightData=((DataBuff[0]<<8)+DataBuff[1])/1.2f;//数据转换成光强度，单位lx
    LightData_Hex=LightData;//float转换成整数
}













