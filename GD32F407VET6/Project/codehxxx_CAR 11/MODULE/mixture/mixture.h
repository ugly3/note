#ifndef __MIXTURE_H
#define __MIXTURE_H
#include "all_module.h"





//函数指针，主要用于函数
typedef struct
{
	uint8_t (*xGet_CheckSum)(uint8_t,uint8_t,uint8_t,uint8_t);
	void (*xKey_Read)(void);
	void (*xTIM2_Init)(void);
	void (*xTIM3_Init)(void);
	void (*xCAR_KeyRun_Function)(void);
	void (*xMixture_Zigbe_RecceiveError_Handler)(uint8_t,uint8_t);
	void (*xMixture_Debug_Data)(uint8_t);
	void (*xBubble_Sort)(uint16_t*,uint8_t);
	void (*xTba_Both_Led)(uint8_t);
}Mixture_Typedef;



/*按键*/
typedef struct
{
	uint8_t Trg;    
	uint8_t Cont;
	uint8_t S1_value;
	uint8_t S2_value;
	uint8_t S3_value;
	uint8_t S4_value;
	bool S1_Flag;
	bool S2_Flag;
	bool S3_Flag;
	bool S4_Flag;	
}Key_Typedef;

extern Key_Typedef KeyData;   
extern Mixture_Typedef Mixture_Data;

void xKey_Read(void);
void xTIM2_Init(void);
void xTIM3_Init(void);
void xCAR_KeyRun_Function(void);
uint8_t xGet_CheckSum(uint8_t main_com,uint8_t sub_com1,uint8_t sub_com2,uint8_t sub_com3);
void xMixture_Zigbe_RecceiveError_Handler(uint8_t Cmd1,uint8_t Cmd2);
void xMixture_Debug_Data(uint8_t num);
void xBubble_Sort(uint16_t*arr,uint8_t len);
void xTba_Both_Led(uint8_t swch);

extern void xHexStringToGraphic(const char *hexString, char *graphicArray);
#endif
