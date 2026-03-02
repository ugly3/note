#ifndef _SMG_H
#define _SMG_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "74hc595.h"

extern unsigned char Tab[];


void LED595SendData(uint8_t wei,unsigned char OutData);

#endif  /* _SERVO_H */

