#include "RC522.h"

#include <string.h>
#include "systick.h"
#include "oled.h"
#include "stdio.h"


// SPI初始化函数
void SPI_Init(void)
{
    spi_parameter_struct spi_init_struct;
    // 1. 开启SPI和GPIO时钟
    rcu_periph_clock_enable(RCU_GPIOA);  // 使能GPIOA时钟
    rcu_periph_clock_enable(RCU_SPI0);   // 使能SPI0时钟

    // 2. 配置GPIO引脚
    //SDA (PA4): 复用推挽输出
    gpio_mode_set(MFRC522_GPIO_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MFRC522_GPIO_SDA_PIN);
    gpio_output_options_set(MFRC522_GPIO_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MFRC522_GPIO_SDA_PIN);
    
    // SCK (PA5): 复用推挽输出
    gpio_mode_set(MF522_SCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MF522_SCK_PIN);
    gpio_output_options_set(MF522_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MF522_SCK_PIN);

    // MOSI (PA7): 复用推挽输出
    gpio_mode_set(MF522_MOSI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MF522_MOSI_PIN);
    gpio_output_options_set(MF522_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MF522_MOSI_PIN);

    // MISO (PA6): 输入浮空
    gpio_mode_set(MF522_MISO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MF522_MISO_PIN);

    // CS (PA4): 普通推挽输出
    gpio_mode_set(MF522_NSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MF522_NSS_PIN);
    gpio_output_options_set(MF522_NSS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MF522_NSS_PIN);
    gpio_bit_set(MF522_NSS_PORT, MF522_NSS_PIN);  // 默认拉高CS引脚

    // 3. 配置SPI0
    
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;  // 全双工模式
    spi_init_struct.device_mode = SPI_MASTER;              // 主设备模式
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;       // 8位数据帧
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;  // 时钟极性低，相位1边沿
    spi_init_struct.nss = SPI_NSS_SOFT;                    // 软件NSS
    spi_init_struct.prescale = SPI_PSC_2;                  // 分频因子为2
    spi_init_struct.endian = SPI_ENDIAN_MSB;               // 大端模式
    spi_init(SPI0, &spi_init_struct);

    // 4. 使能SPI0
    spi_enable(SPI0);
}



/**************************************************************
*功  能：	RC522 芯片初始化
*参  数: 	无
*返回值: 	无 
**************************************************************/
void InitRc522(void)
{
	SPI_Init();
	delay_1ms(100);  
  PcdReset();
  PcdAntennaOff();
  delay_1ms(2);  
  PcdAntennaOn();
  M500PcdConfigISOType( 'A' );
}


void Reset_RC522(void)
{
  PcdReset();
  PcdAntennaOff();
  delay_1ms(2);  
  PcdAntennaOn();
}                         
/**************************************************************
*功  能：	寻卡
*参  数: 	req_code[IN]:寻卡方式  
*		 	0x52 = 寻感应区内所有符合14443A标准的卡
*		 	0x26 = 寻未进入休眠状态的卡
*		 	pTagType[OUT]：卡片类型代码
*		 	0x4400 = Mifare_UltraLight
*		 	0x0400 = Mifare_One(S50)
*		 	0x0200 = Mifare_One(S70)
*		 	0x0800 = Mifare_Pro(X)
*		 	0x4403 = Mifare_DESFire
*返回值: 	成功返回MI_OK 
**************************************************************/
char PcdRequest(uint8_t   req_code,uint8_t *pTagType)
{
	char   status;  
	uint8_t   unLen;
	uint8_t   ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}

/**************************************************************
*功  能：	防冲撞
*参  数: 	pSnr[OUT]:卡片序列号，4字节
*返回值: 	成功返回MI_OK 
**************************************************************/
char PcdAnticoll(uint8_t *pSnr)
{
    char   status;
    uint8_t   i,snr_check=0;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/**************************************************************
*功  能：	选定卡片
*参  数: 	pSnr[IN]:卡片序列号，4字节
*返回值: 	成功返回MI_OK
**************************************************************/
char PcdSelect(uint8_t *pSnr)
{
    char   status;
    uint8_t   i;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/**************************************************************
*功  能：	验证卡片密码
*参  数:	auth_mode[IN]: 密码验证模式 0x60 = 验证A密钥 0x61 = 验证B密钥
*		 	addr[IN]：块地址   pKey[IN]：密码  pSnr[IN]：卡片序列号，4字节
*返回值: 	成功返回MI_OK
**************************************************************/              
char PcdAuthState(uint8_t   auth_mode,uint8_t   addr,uint8_t *pKey,uint8_t *pSnr)
{
    char   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+2] = *(pKey+i);   }
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/**************************************************************
*功  能：	读取M1卡一块数据
*参  数: 	addr[IN]：块地址 pData[OUT]：读出的数据，16字节
*返回值: 	成功返回MI_OK
**************************************************************/ 
char PcdRead(uint8_t   addr,uint8_t *p )
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(p , ucComMF522Buf, 16);   }
    {
//        for (i=0; i<16; i++)
//        {    *(p +i) = ucComMF522Buf[i];   }
        for (i=0; i<5; i++)      //获取前五个数
        {    *(p +i) = ucComMF522Buf[i];   }
        
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/**************************************************************
*功  能：	写数据到M1卡一块
*参  数: 	addr[IN]：块地址  pData[IN]：写入的数据，16字节
*返回值: 	成功返回MI_OK
**************************************************************/                 
char PcdWrite(uint8_t   addr,uint8_t *p )
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(p +i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/**************************************************************
*功  能：	命令卡片进入休眠状态
*参  数: 	无
*返回值: 	成功返回MI_OK
**************************************************************/
char PcdHalt(void)
{
    uint8_t   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}

/**************************************************************
*功  能：	用MF522计算CRC16函数
*参  数: 	无
*返回值: 	无 
**************************************************************/
void CalulateCRC(uint8_t *pIn ,uint8_t   len,uint8_t *pOut )
{
    uint8_t   i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIn +i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}

/**************************************************************
*功  能：	复位RC522
*参  数: 	无
*返回值: 	成功返回MI_OK
**************************************************************/
char PcdReset(void)
{
	
    delay_1us(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_1us(10);
    
	 //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(ModeReg,0x3D);           
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
	
	//必须要
	WriteRawRC(TxAutoReg,0x40);
   
    return MI_OK;
}
//////////////////////////////////////////////////////////////////////
//设置RC632的工作方式 
//////////////////////////////////////////////////////////////////////
char M500PcdConfigISOType(uint8_t   type)
{
	//ISO14443_A
   if (type == 'A')                    
   { 
       ClearBitMask(Status2Reg,0x08);
	   //3F
       WriteRawRC(ModeReg,0x3D);
	   //84
       WriteRawRC(RxSelReg,0x86);
	   //4F
       WriteRawRC(RFCfgReg,0x7F);
	   //tmoLength);// TReloadVal = 'h6a =tmoLength(dec)
   	   WriteRawRC(TReloadRegL,30); 
			 WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
			 WriteRawRC(TPrescalerReg,0x3E);
			 delay_1us(1000);
       PcdAntennaOn();
   }
   else{ return 1; }
   
   return MI_OK;
}

/**************************************************************
*功  能：	置RC522寄存器位
*参  数: 	reg[IN]:寄存器地址  mask[IN]:置位值
*返回值: 	无 
**************************************************************/
void SetBitMask(uint8_t   reg,uint8_t   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
	// set bit mask
    WriteRawRC(reg,tmp | mask);  
}

/**************************************************************
*功  能：	清RC522寄存器位
*参  数: 	reg[IN]:寄存器地址  mask[IN]:清位值
*返回值: 	无 
**************************************************************/
void ClearBitMask(uint8_t   reg,uint8_t   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
	// clear bit mask
    WriteRawRC(reg, tmp & ~mask);  
} 

/**************************************************************
*功  能：	通过RC522和ISO14443卡通讯
*参  数: 	Command[IN]:RC522命令字
*		 	pInData[IN]:通过RC522发送到卡片的数据
*		 	InLenByte[IN]:发送数据的字节长度
*		 	pOutData[OUT]:接收到的卡片返回数据
*		 	*pOutLenBit[OUT]:返回数据的位长度
*返回值: 	无 
**************************************************************/
char PcdComMF522(uint8_t   Command, 
                 uint8_t *pIn , 
                 uint8_t   InLenByte,
                 uint8_t *pOut , 
                 uint8_t *pOutLenBit)
{
    char   status = MI_ERR;
    uint8_t   irqEn   = 0x00;
    uint8_t   waitFor = 0x00;
    uint8_t   lastBits;
    uint8_t   n;
    uint16_t   i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
	//清所有中断位
    ClearBitMask(ComIrqReg,0x80);	
    WriteRawRC(CommandReg,PCD_IDLE);
	//清FIFO缓存
    SetBitMask(FIFOLevelReg,0x80);	 	
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    { 
     //开始传送		
			SetBitMask(BitFramingReg,0x80);  }	 
    										 
    //i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
		i = 2000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOut [i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
   

	// stop timer now
    SetBitMask(ControlReg,0x80);           
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}

/**************************************************************
*功  能：	开启天线（每次启动或关闭天险发射之间应至少有1ms的间隔）
*参  数: 	无
*返回值: 	无 
**************************************************************/
void PcdAntennaOn(void)
{
    uint8_t   i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/**************************************************************
*功  能：	关闭天线
*参  数: 	无
*返回值: 	无 
**************************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
uint8_t ReadRawRC(uint8_t   Address)
{    
    uint8_t ucResult=0;	
//    ucResult = RC522_RD_Reg(SLA_ADDR,  Address);
    uint8_t i, ucAddr;

     SCK_L;
     delay_1us(2);
     SDA_L;
     delay_1us(2);
     ucAddr = ((Address<<1)&0x7E)|0x80;
 
     for(i=8;i>0;i--)
     {
	 	 if(ucAddr&0x80)
         	MOSI_H;
		 else
		    MOSI_L;
         SCK_H;
         delay_1us(2);
         ucAddr <<= 1;
         SCK_L;
         delay_1us(2);
     }
 
     for(i=8;i>0;i--)
     {
         SCK_H;
         delay_1us(2);
         ucResult <<= 1;
         ucResult|= READ_MISO;
         SCK_L;
         delay_1us(2);
     }
 
     NSS_H;
     delay_1us(2);
     SCK_H;
     delay_1us(2);
     return ucResult;
  
}

/////////////////////////////////////////////////////////////////////
//功    能：写RC632寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(uint8_t   Address, uint8_t   value)
{  
    unsigned char i, ucAddr;
 
    SCK_L;
    NSS_L;
    ucAddr = ((Address<<1)&0x7E);
 
    for(i=8;i>0;i--)
    {
		if(ucAddr&0x80)
        	MOSI_H;
		else
			MOSI_L;
        SCK_H;
        ucAddr <<= 1;
        SCK_L;
    }
 
    for(i=8;i>0;i--)
    {
		if(value&0x80)
        	MOSI_H;
		else
			MOSI_L;
        SCK_H;
        value <<= 1;
        SCK_L;
    }
    NSS_H;
    SCK_H;
}



///**************************************************************
//*功  能：	读寄存器
//*参  数: 	addr:寄存器地址
//*返回值: 	读到的值 
//**************************************************************/
//uint8_t RC522_RD_Reg(uint8_t RCsla,uint8_t addr) 		
//{
//	uint8_t temp=0;		 
//	MY_IIC_Start();
//	//发送写器件指令	
//	MY_IIC_Send_Byte(RCsla);	 
//	temp=MY_IIC_Wait_Ack();
//	//发送寄存器地址
//  MY_IIC_Send_Byte(addr);   		
//	temp=MY_IIC_Wait_Ack();
//	//重新启动
//	MY_IIC_Start();
//	//发送读器件指令	 
//	MY_IIC_Send_Byte(RCsla+1);	
//	temp=MY_IIC_Wait_Ack();
//	//读取一个字节,不继续再读,发送NAK 
//  temp=MY_IIC_Read_Byte(0);
//	//产生一个停止条件 	
//  MY_IIC_Stop();
//	//返回读到的值
//	return temp;				
//}  



////写寄存器
////addr:寄存器地址
////val:要写入的值
////返回值:无
//void RC522_WR_Reg(uint8_t RCsla,uint8_t addr,uint8_t val) 
//{
//	MY_IIC_Start();
//	//发送写器件指令
//	MY_IIC_Send_Byte(RCsla);     		 
//	MY_IIC_Wait_Ack();	
//	//发送寄存器地址
//  MY_IIC_Send_Byte(addr);   			
//	MY_IIC_Wait_Ack();
//	//发送值
//	MY_IIC_Send_Byte(val);     							   
//	MY_IIC_Wait_Ack(); 
//	//产生一个停止条件 	
//  MY_IIC_Stop();						   
//}


//等待卡离开
void WaitCardOff(void)
{
	unsigned char status, TagType[2];

	while(1)
	{
		status = PcdRequest(REQ_ALL, TagType);
		if(status)
		{
			status = PcdRequest(REQ_ALL, TagType);
			if(status)
			{
				status = PcdRequest(REQ_ALL, TagType);
				if(status)
				{
					return;
				}
			}
		}
		delay_1ms(10);
	}
}

uint8_t UI0[4]={0xFF,0xFF,0xFF,0xFF};		 	//卡0ID号
uint8_t UI1[4]={0xFF,0xFF,0xFF,0xFF};	 		//卡1ID号
uint8_t UI2[4]={0xFF,0xFF,0xFF,0xFF};			//卡2ID号
uint8_t UI3[4]={0xFF,0xFF,0xFF,0xFF};			//卡3ID号


uint8_t Read_Card(void)
{
//	uint8_t str[30];
    uint8_t cardno;
	uint8_t status;
	uint8_t s=0x00;//地址快
	//卡类型
	unsigned char CT[2];
	//卡号
	unsigned char SN[4];
	//存放RFID 
	unsigned char RFID[5];			
	uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	/*尋卡*/
	status = PcdRequest(PICC_REQALL,CT);
	//尋卡成功
	if(status==MI_OK)
	{            
//			delay_1ms(200);
			status=MI_ERR;
			/*防冲撞*/
			status = PcdAnticoll(SN);
	}
	//防衝撞成功
	if (status==MI_OK)
	{   
			status=MI_ERR;	
			status =PcdSelect(SN);

	}
	//选卡成功
	if(status==MI_OK)
	{
		status=MI_ERR;
		status =PcdAuthState(0x60,s,KEY,SN);
		//验证成功
		if(status==MI_OK)		
		{
			status=MI_ERR;
			//读卡
			status=	PcdRead(s,RFID);
			//讀卡成功
			if(status==MI_OK)
			{
                cardno=0;
                if(RFID[0]==UI0[0]&&RFID[1]==UI0[1]&&RFID[2]==UI0[2]&&RFID[3]==UI0[3])
                {
                    cardno=1;
                }
                else if(RFID[0]==UI1[0]&&RFID[1]==UI1[1]&&RFID[2]==UI1[2]&&RFID[3]==UI1[3])
                {
                    cardno=2;
                }
                else if(RFID[0]==UI2[0]&&RFID[1]==UI2[1]&&RFID[2]==UI2[2]&&RFID[3]==UI2[3])
                {
                    cardno=3;
                }
                else if(RFID[0]==UI3[0]&&RFID[1]==UI3[1]&&RFID[2]==UI3[2]&&RFID[3]==UI3[3])
                {
                    cardno=4;
                }
                else cardno = 0;
                
//				sprintf((char*)str,"%02X %02X %02X %02X ",RFID[0],RFID[1],RFID[2],RFID[3]);
//				OLED_ShowString( 0,0,str);
//                
//                sprintf((char*)str,"%02X %02X %02X %02X ",RFID[4],RFID[5],RFID[6],RFID[7]);
//				OLED_ShowString( 0,2,str);
//                
//                sprintf((char*)str,"%02X %02X %02X %02X ",RFID[8],RFID[9],RFID[10],RFID[11]);
//                OLED_ShowString( 0,4,str);
//									
//                sprintf((char*)str,"%02X %02X %02X %02X ",RFID[12],RFID[13],RFID[14],RFID[15]);
//                OLED_ShowString( 0,6,str);
//				WaitCardOff();
				status=MI_ERR;
//				delay_1ms(100);
			}
            else cardno = 0;
		}
        else cardno = 5;
	}
        else cardno = 6;
    
     return cardno;
}
