#include "step_motor.h"



//步进电机转动的速度0~100
uint8_t speed = 100;		



uint8_t step_pulse[8]={0x12,0x02,0x06,0x04,0x0c,0x08,0x18,0x20};

//反转
void Reverse(void)
{
	for(int i=7;i>=0;i--){
		hc595_WriteByte(step_pulse[i]);
		delay_1ms(102-speed);
	}
}

//正转
void Corotation(void)
{	
	for(int i=0;i<8;i++){
		hc595_WriteByte(step_pulse[i]);
		delay_1ms(102-speed);
	}
	
	
	//AC组

}

void STOP(void)
{
	
	hc595_WriteByte(0xff);
}



