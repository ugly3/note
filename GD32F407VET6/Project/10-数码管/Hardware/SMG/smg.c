#include "smg.h"

unsigned char Tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
void LED595SendData(uint8_t wei,unsigned char OutData)
{
	uint16_t data=0;
	
	
	if(wei==0)data=0xFE00;
	else if(wei==1)data=0xFD00;
	else if(wei==2)data=0xFB00;
	else if(wei==3)data=0xF700;
	data|=(OutData&0x00ff);
	
	hc595_WriteByte16(data);
	
	//HC595_Send_16Bit(&hc1,data);
}


