#include "hx711.h"

unsigned long Weight_Maopi = 0;   //去皮



int GapValue = 1103;	   //传感器曲率   当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。该值可以为小数，例如 429.5 等。



void HX711_init(void){

  rcu_periph_clock_enable(HX711_SCL_RTC);

	gpio_mode_set(HX711_SCL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, HX711_SCL_Pin);
	gpio_output_options_set(HX711_SCL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, HX711_SCL_Pin);	
	gpio_bit_write(HX711_SCL_GPIO_Port, HX711_SCL_Pin, SET);
	
	rcu_periph_clock_enable(HX711_SDA_RTC);
	gpio_mode_set(HX711_SDA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, HX711_SDA_Pin);
	Get_Maopi();
}

unsigned long HX711_Read(void)
{
	uint32_t Count = 0;
	uint8_t i = 0;
	uint16_t time_out=0;
//	GPIO_HX711_SCK_Out();
	HX711_SCK_LOW;
	while(HX711_DOUT_IN){
		delay_1ms(1);
		time_out++;
		if(time_out==2000)return 0;
	}
	
	for(i=0;i<24;i++)
	{
		
		HX711_SCK_HIGH;
		Count=Count<<1;
		HX711_SCK_LOW;
		if(HX711_DOUT_IN)Count++;
	}
	HX711_SCK_HIGH;
	Count=Count^0x800000;
	HX711_SCK_HIGH;

	return Count;
}
/****************************************************************
功  能：获取毛皮重量
参  数：无
返回值：无
****************************************************************/
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

/****************************************************************
功  能：称重
参  数：无
返回值：无
****************************************************************/

long Get_Weight(void)
{
	unsigned long HX711_Buffer = 0;
	long  Weight_object = 0;

	HX711_Buffer = HX711_Read();

	Weight_object = HX711_Buffer;
	//Weight_object = Weight_object - Weight_Maopi;	   		// 获取实物的AD采样数值。

	if(Weight_object> 0)
	{
		Weight_object= ((float)Weight_object / GapValue); 	// 计算实物的实际重量	
	}
	else
	{
		Weight_object = 0;
	}

	
	return Weight_object;
}



