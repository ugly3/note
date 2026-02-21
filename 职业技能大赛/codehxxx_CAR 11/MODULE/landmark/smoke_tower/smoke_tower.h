#ifndef __SMOKE_TOWER_H
#define __SMOKE_TOWER_H
#include "all_module.h"

typedef struct
{
	void (*SmokeTower_Infrared_Open)(void);
}SmokeTower_Typedef;

extern SmokeTower_Typedef SmokeTower_Data;
extern uint8_t SmokeTower_Infrared[6];
void SmokeTower_Infrared_Open(void);

#endif
