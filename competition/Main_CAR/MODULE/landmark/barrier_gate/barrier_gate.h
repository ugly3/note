#ifndef __BARRIER_GATE_H
#define __BARRIER_GATE_H
#include "all_module.h"


typedef struct
{
	uint8_t Barrier_Gate_OPEN;
	uint8_t Barrier_Gate_CLOSE;
	uint8_t (*xBarrier_Ack_State)(void);
	void (*xBarrier_Open_Close)(uint8_t);
	void (*xBarrier_Up_Dowm)(uint8_t);
	void (*xBarrier_Licence_Tx)(char*);
}Barrier_Tyepdef;

extern Barrier_Tyepdef Barrier_Data;






void xBarrier_Open_Close(uint8_t OC);
void xBarrier_Up_Dowm(uint8_t UD);
void xBarrier_Licence_Tx(char* licence);
uint8_t xBarrier_Ack_State(void);



#endif
