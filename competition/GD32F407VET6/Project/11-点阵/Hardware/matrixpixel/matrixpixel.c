#include "matrixpixel.h"




void matrixSendData(uint8_t wei,unsigned char OutData)
{
	uint16_t data=0X8000;
	
	data>>=wei;
	data=~data;
	data&=0xff00;
	data|=(OutData&0x00ff);

	hc595_WriteByte16(data);

}


