#ifndef __SMOKE_TOWER_H
#define __SMOKE_TOWER_H
#include "all_module.h"

typedef struct
{
    uint8_t SmokeTower_Weizhi[2];
	void (*SmokeTower_Infrared_Open)(void);
    void (*xSmokeTower_Zigbee_Open)(uint8_t *);
}SmokeTower_Typedef;

extern SmokeTower_Typedef SmokeTower_Data;
extern uint8_t SmokeTower_Infrared[6];
void SmokeTower_Infrared_Open(void);
void xSmokeTower_Zigbee_Open(uint8_t *str);    
#endif


