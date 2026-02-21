#ifndef __XIAOCHUANG_H
#define __XIAOCHUANG_H
#include "all_module.h"



typedef struct
{
	void (*xSend_To_XiaoChuang_Rouse)(void);
	void (*xSend_Command_To_XiaoChuang)(uint8_t);
	void (*xXiaoChuang_Analyze_Rx_Command)(void);
	void (*xZiDongPingFen_Upload_VoiceCode)(uint8_t);
}XiaoChuang_Typedef;

extern XiaoChuang_Typedef XiaoChuang_Data;
extern uint8_t XiaoChuang_Rx_Byte;

void xSend_To_XiaoChuang_Rouse(void);
void xSend_Command_To_XiaoChuang(uint8_t Cmd);
void xXiaoChuang_Analyze_Rx_Command(void);
void xZiDongPingFen_Upload_VoiceCode(uint8_t data);

#endif
