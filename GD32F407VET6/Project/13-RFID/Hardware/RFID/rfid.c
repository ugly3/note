#include "rfid.h"

#define MAXRLEN 18  


void SPI_GPIO_Init(void) {
    // 使能 GPIOB 时钟
    rcu_periph_clock_enable(RCU_GPIOB);
	

    // 配置 PB13 (SCK), PB14 (MISO), PB15 (MOSI) 为复用功能
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);  // AF5 是 SPI1 的复用功能
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
		
		rcu_periph_clock_enable(rfid_cs_RTC);
    // 配置 PD1 (CS) 为推挽输出
    gpio_mode_set(rfid_cs_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, rfid_cs_Pin);
    gpio_output_options_set(rfid_cs_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, rfid_cs_Pin);
}

void SPI1_Init(void) {
    // 使能 SPI1 时钟
    rcu_periph_clock_enable(RCU_SPI1);

    // 配置 SPI1
    spi_parameter_struct spi_init_params;
    spi_struct_para_init(&spi_init_params);

    spi_init_params.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  // 全双工模式
    spi_init_params.device_mode          = SPI_MASTER;               // 主模式
    spi_init_params.frame_size           = SPI_FRAMESIZE_8BIT;       // 8 位数据帧
    spi_init_params.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;   // CPOL=0, CPHA=0（模式 0）
    spi_init_params.nss                  = SPI_NSS_SOFT;             // 软件控制 NSS（片选）
    spi_init_params.prescale             = SPI_PSC_8;                // 时钟预分频（84MHz / 8 = 10.5MHz）
    spi_init_params.endian               = SPI_ENDIAN_MSB;           // 高位在前

    spi_init(SPI1, &spi_init_params);

    // 使能 SPI1
    spi_enable(SPI1);
}


short InitRc522(void)
{

	  if(PcdReset() != MI_OK)
		  return MI_ERR;
	  PcdAntennaOff();
	  delay_1ms(2);  
	  PcdAntennaOn();
	  if(M500PcdConfigISOType( 'A' ) != MI_OK)
		  return MI_ERR;

	  return MI_OK;
}

short Readcard_daivce_Init(void)
{
	uint16_t n;
	SPI_GPIO_Init();
	SPI1_Init();
	//RC522_Uart_init(9600);	// 串口初始化为9600
	delay_1ms(500);
	if(InitRc522()!= MI_OK)return MI_ERR;			//读卡器初始化
	WriteRawRC(SerialSpeedReg,0x74);
	WriteRawRC(SerialSpeedReg,0x5a);

//  huart1.Init.BaudRate = 1228800;

//  if (HAL_UART_Init(&huart1) != HAL_OK){
//		rt_kprintf("\rever\r",n);
//	}
	 n=ReadRawRC(SerialSpeedReg);
	
	printf("rc%d\r",n);
	return MI_OK;
}




/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          	  pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
short PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   short status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 
//  unsigned char xTest ;
   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);

//  xTest = ReadRawRC(BitFramingReg);
//  if(xTest == 0x07 )
 //   { LED_GREEN  =0 ;}
 // else {LED_GREEN =1 ;while(1){}}
   SetBitMask(TxControlReg,0x03);

   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

//   if(status  == MI_OK )
//   { printf("ok\r\n");}
//   else {printf("on\r\n") ;}
	 
   if ((status == MI_OK) && (unLen == 0x10))
   {    
		 	 
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];

   }
   else
   {   status = MI_ERR;   }
   return status;
	 
}

/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
short PcdAnticoll(unsigned char *pSnr)
{
    short status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

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

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
short PcdSelect(unsigned char *pSnr)
{
    short status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
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

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
short PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    short status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
short PcdRead(unsigned char addr,unsigned char *pData)
{
    short status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
short PcdWrite(unsigned char addr,unsigned char *pData)
{
    short status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
short PcdHalt(void)
{
    short status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;//MI_OK;
}



/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
short CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    uint8_t Address;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    Address = (Address<<1)&0x7e;
    for (i=0; i<len; i++){  
			WriteRawRC(FIFODataReg, *(pIndata+i));  
	 	}


    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
	return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
short PcdReset(void)
{
    WriteRawRC(CommandReg,PCD_RESETPHASE);
	  delay_100ns(30);

    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
    WriteRawRC(TxAutoReg,0x40); 

    
	  
    return MI_OK;
	
//	return MI_ERR;
}

//////////////////////////////////////////////////////////////////////
//设置RC522的工作方式 
//////////////////////////////////////////////////////////////////////
short M500PcdConfigISOType(unsigned char type)
{
   if (type == 'A')                     //ISO14443_A
   { 
		   
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);   //4F
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
	   WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	   WriteRawRC(TPrescalerReg,0x3E);
	   delay_1ms(10);
       PcdAntennaOn();

		   
   }
   else{ return -1; }
   
   return MI_OK;
}






//    u8 ucAddr, ucReturn;

//    ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;

//    RC522_CS_Enable();

//    SPI_RC522_SendByte ( ucAddr );

//    ucReturn = SPI_RC522_ReadByte ();

//    RC522_CS_Disable();

//    return ucReturn;
/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
    unsigned char RegVal;
	uint32_t timeOut=0;
	  //Address<<=1;
    Address = ((Address<<1) & 0x7e) | 0x80;   //code the first byte
	
	gpio_bit_write(rfid_cs_Port,rfid_cs_Pin,RESET);
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_transmit(SPI1, Address);
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	RegVal=spi_i2s_data_receive(SPI1);
	
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_transmit(SPI1, 0);
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	RegVal=spi_i2s_data_receive(SPI1);
	

	gpio_bit_write(rfid_cs_Port,rfid_cs_Pin,SET);


    return RegVal;
}



short WriteRawRC(unsigned char Address, unsigned char value)
{  
//    unsigned char EchoByte;
	//Address<<=1;
	uint32_t timeOut=0;
  Address = (Address<<1)&0x7e;   //code the first byte
	
	gpio_bit_write(rfid_cs_Port,rfid_cs_Pin,RESET);
	
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_transmit(SPI1, Address);
	
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_receive(SPI1);
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_transmit(SPI1, value);
	timeOut=0;
	while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){timeOut++;delay_us(10);if(timeOut>30000)break;}
	spi_i2s_data_receive(SPI1);
	//spi_i2s_data_receive(SPI1);
	gpio_bit_write(rfid_cs_Port,rfid_cs_Pin,SET);

	return STATUS_SUCCESS;
}


/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
short SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
	  return MI_OK;
}


/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
short ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
	return MI_OK;
} 



/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////




short PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    short status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
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
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
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
                 {   pOutData[i] = ReadRawRC(FIFODataReg);    }

            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   
   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}


/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn(void)
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

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

uint8_t buf[64][16];

uint8_t read_RFID_all(void){  //读出卡内的所有数据
		uint8_t CT[2];	
	uint8_t SN[4];
	 uint8_t i,j;
	uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //密钥
  if(PcdRequest(PICC_REQALL,CT)!=MI_OK)return 0; 				//寻卡
	printf("ok");
	if(PcdAnticoll(SN)!=MI_OK)return 0;            //获取卡ID
	if(PcdSelect(SN)!=MI_OK)return 0;				       //选定此卡
	for(i=0;i<16;i++){
		
	 if(PcdAuthState(0x60,4*i+3,KEY,SN)!=MI_OK)continue ;	//验证密钥
		for(j=0;j<4;j++){
			if(PcdRead(i*4+j,buf[i*4+j])!=MI_OK){
				continue;
			}
		}
		
	}
	printf("\r\n");
	for(i=0;i<64;i++){
		printf("ca%d=",i);
		for(j=0;j<16;j++){
			printf("%x ",buf[i][j]);
		}
		printf("\r\n");
	}
	return 1;	
}


void Read_Card(void)
{
	char str[50];
	uint8_t status;
	uint8_t s=0x00;
	//卡类型
	unsigned char CT[2];
	//卡号
	unsigned char SN[4];
	//存放RFID 
	unsigned char RFID[16];			
	uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	/*尋卡*/
	status = PcdRequest(PICC_REQALL,CT);
	//尋卡成功
	if(status==MI_OK)
	{            
			//delay_1ms(200);
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
			//读卡卡成功
			if(status==MI_OK)
			{
				printf("读卡成功\r\n");
				printf("ID:");
				OLED_Clear();
				OLED_PrintString(0, 0, "succeed",1);
				OLED_Refresh();
			
				Readcard_daivce_Init();
				for(int i=0;i<16;i++){
					printf("%02X ",RFID[i]);
				}
				printf("\r\n");
				WaitCardOff( );
				status=MI_ERR;
				delay_1ms(500);
				OLED_Clear();
				OLED_Refresh();
			}
		}

		printf("等待识别\r\n");
	}
		
	
	printf(".");
}






