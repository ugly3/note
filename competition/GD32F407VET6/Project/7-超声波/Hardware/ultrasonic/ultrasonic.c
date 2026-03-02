#include "ultrasonic.h"


void Ultrasonic_init(void){
	rcu_periph_clock_enable(Trig_RTC);

	gpio_mode_set(Trig_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, Trig_Pin);
	gpio_output_options_set(Trig_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Trig_Pin);	
	gpio_bit_write(Trig_GPIO_Port, Trig_Pin, RESET);
	
	rcu_periph_clock_enable(ECHO_RTC);
	gpio_mode_set(ECHO_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, ECHO_Pin);
}


uint16_t Ultrasonic_read(void){
	uint16_t distance_cm;
	gpio_bit_write(Trig_GPIO_Port, Trig_Pin, SET);
	delay_us(10);
	gpio_bit_write(Trig_GPIO_Port, Trig_Pin, RESET);
	uint32_t outTime=0;
	while(gpio_input_bit_get(ECHO_GPIO_Port,ECHO_Pin)==0){
		delay_us(1);
		outTime++;
		if(outTime==33000)return 0xffff;
	}
	outTime=0;
	while(gpio_input_bit_get(ECHO_GPIO_Port,ECHO_Pin)==1){
		delay_us(1);
		outTime++;
		if(outTime==33000)return 0xffff;
	}
	distance_cm=outTime*0.017;
	
	

	return distance_cm;
}

