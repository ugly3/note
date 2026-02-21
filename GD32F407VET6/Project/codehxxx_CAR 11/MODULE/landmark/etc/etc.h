#ifndef __ETC_H
#define __ETC_H
#include "all_module.h"

#define ETC_Middle 1	//ETC在中间
#define ETC_Cross 0	//ETC在十字路口

typedef struct
{
	uint8_t Left_Gate_Up;
	uint8_t Left_Gate_Dowm;
	uint8_t Right_Gate_Up;
	uint8_t Right_Gate_Dowm;
    uint8_t ETC_Car_Start;
	void (*xETC_Init_Angle_Adjust)(uint8_t);
	void (*xETC_Pass)(void);
    void (*xETC_Pass_RFID)(uint8_t,uint8_t,uint8_t);
}ETC_Typedef;

extern ETC_Typedef ETC_Data;




void xETC_Init_Angle_Adjust(uint8_t AngleAdjust);
void xETC_Pass(void);
void xETC_Pass_RFID(uint8_t card1,uint8_t card2,uint8_t card3);




#endif
