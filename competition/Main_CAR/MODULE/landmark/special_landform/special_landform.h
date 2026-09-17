#ifndef __SPECIAL_LANDFORM_H
#define __SPECIAL_LANDFORM_H
#include "all_module.h"


typedef struct
{
	uint8_t (*xSpecial_LandForm_Ack_State)(void);
	void (*xSpecial_LandForm_Pass)(void);
}Special_LandForm_Typedef;

extern Special_LandForm_Typedef Special_LanForm_Data;


uint8_t xSpecial_LandForm_Ack_State(void);
void xSpecial_LandForm_Pass(void);




#endif
