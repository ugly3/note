#include "ALL.h"




#define I2C1_SLAVE_ADDRESS7    0x46
void i2c1_config(void)
{
    /* configure I2C1 clock */
    i2c_clock_config(I2C1, 100000, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, MPU6050_SLAVE_ADDRESS, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C1, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C1, reg_add);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
    
    /* send the byte to be written */
    i2c_data_transmit(I2C1, reg_dat); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C1);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1)&0x0200);
}

/**
  * 函数功能: 从MPU6050寄存器读取数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReadData(uint8_t reg_add,unsigned char*Read,uint8_t num)
{
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));

    if(2 == num){
        i2c_ackpos_config(I2C1,I2C_ACKPOS_NEXT);
    }
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, MPU6050_SLAVE_ADDRESS, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C1 , I2C_FLAG_TBE));

    /* enable I2C0*/
    i2c_enable(I2C1);
    
    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, reg_add);  
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, MPU6050_SLAVE_ADDRESS, I2C_RECEIVER);

    if(num < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C1,I2C_ACK_DISABLE);
    }
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1,I2C_FLAG_ADDSEND);
    
    if(1 == num){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C1);
    }
    
    /* while there is data to be read */
    while(num){
        if(3 == num){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C1,I2C_ACK_DISABLE);
        }
        if(2 == num){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C1);
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C1, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *Read = i2c_data_receive(I2C1);
            
            /* point to the next location where the byte read will be saved */
            Read++; 
            
            /* decrement the read bytes counter */
            num--;
        } 
    }
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C1,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C1,I2C_ACKPOS_CURRENT);
}


void Motion_Interrupt(void)             //运动中断
{
    MPU6050_WriteReg(MPU6050_RA_MOT_THR,10);              //运动阈值，如果要增加灵敏度，可以把值改到10以下
    MPU6050_WriteReg(MPU6050_RA_MOT_DUR,0x14);            //检测时间20ms 单位1ms 
}

/**
  * 函数功能: 中断初始化设置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU_INT_Init(void)
{
//自由落体、运动、静止中断 三选一  
//    Free_Fall_Interrupt();                          //自由落体中断
    Motion_Interrupt();                               //运动中断
//    Zero_Motion_Interrupt();                        //静止中断
    MPU6050_WriteReg(MPU6050_RA_CONFIG,0x04);           //配置外部引脚采样和DLPF数字低通滤波器
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x04);     //加速度传感器量程和高通滤波器配置
    MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG,0X1C);      //INT引脚低电平平时
    MPU6050_WriteReg(MPU6050_RA_INT_ENABLE,0x40);       //中断使能寄存器
}
/**
  * 函数功能: 初始化MPU6050芯片
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_Init(void)
{
  int i=0,j=0;
    
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_10);//复用功能4
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_11);//复用功能4
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);//PB6,PB7配置成I2C0
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
   
    rcu_periph_clock_enable(RCU_I2C1);//I2C1时钟使能
    i2c1_config();
    //////////////////////////外部中断配置//////////////////////////////////
//    rcu_periph_clock_enable(RCU_GPIOA);
//    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_7);
//    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
    
    rcu_periph_clock_enable(RCU_GPIOE);// 开启时钟
    rcu_periph_clock_enable(RCU_SYSCFG);// 开启系统配置时钟
    gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_7);//配置成下拉输入
    syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN7);// 将引脚连接到EXTI线
    exti_init(EXTI_7, EXTI_INTERRUPT, EXTI_TRIG_RISING);//上升沿触发
    exti_interrupt_enable(EXTI_7);//使能EXTI线的中断
    exti_interrupt_flag_clear(EXTI_7);//中断使能
    exti_interrupt_flag_clear(EXTI_7);//清除中断标志位
    
    nvic_irq_enable(EXTI5_9_IRQn, 2, 2);
    
    /* configure key EXTI line */
    
    delay_1ms(180);//等待180ms
    
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //配置加速度传感器工作在2G模式，不自检
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	MPU_INT_Init();
  
	if(MPU6050ReadID() == 0)//读取MPU6050 ID
	{	
	  while(1);
    }

}

/**
  * 函数功能: 读取MPU6050的ID
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
	//	printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
	//	printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}


/**
  * 函数功能: 读取MPU6050的原始温度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
	*tempData = (buf[0] << 8) | buf[1];
}

/**
  * 函数功能: 读取MPU6050的温度数据，转化成摄氏度
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
	temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}

//读取MPU6050的加速度和角速度数据 
void MPU6050Read_Acc_Gyro(short *accData,short *gyroData)
{
    uint8_t buf1[6],buf2[6];
    
    MPU6050_ReadData(MPU6050_ACC_OUT, &buf1[0], 6);
    accData[0] = (buf1[0] << 8) | buf1[1];
    accData[1] = (buf1[2] << 8) | buf1[3];
    accData[2] = (buf1[4] << 8) | buf1[5];
    
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf2,6);
    gyroData[0] = (buf2[0] << 8) | buf2[1];
    gyroData[1] = (buf2[2] << 8) | buf2[3];
    gyroData[2] = (buf2[4] << 8) | buf2[5];
}


uint8_t IntFlag=0,LedFlag=0;;
void MPU6050_earthquake(void)
{
    if(IntFlag)
    {
        LedFlag^=0x01;
        if(LedFlag)
            gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4输出高电平
        else
            gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4输出低电平
    }
}

//外部中断
void EXTI5_9_IRQHandler(void)
{
    if( SET == exti_interrupt_flag_get(EXTI_7))
    {
        IntFlag=1;
        exti_interrupt_flag_clear(EXTI_7);
    }
		
}


