#include "HX711.h"
#include "systick.h"
#include "sys.h"  
//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue  900.5

uint32_t HX711_Buffer;
uint32_t Weight_Maopi;
float Weight_Shiwu;
uint8_t Flag_Error = 0;

void HX711_Init(void)
{
 
    rcu_periph_clock_enable(HX711_RCU_GPIOX);//使能GPIOB时钟
    gpio_mode_set(HX711_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, HX711_PIN_SCK);//输出
    gpio_output_options_set(HX711_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, HX711_PIN_SCK);//推挽输出，50M速度
    
    gpio_mode_set(HX711_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, HX711_PIN_DOUT);//上拉输入
    
    gpio_bit_set(HX711_PORT, HX711_PIN_SCK);
    Get_Maopi();		      //称毛皮重量
}


//****************************************************
//读取HX711
//****************************************************
uint32_t HX711_Read(void)	//增益128
{
	unsigned long count; 
//	unsigned char timeout;
    uint8_t i;    
  	gpio_bit_set(HX711_PORT, HX711_PIN_DOUT);
	delay_1us(20);
  	gpio_bit_reset(HX711_PORT, HX711_PIN_SCK); 
  	count=0; 
  	while(gpio_input_bit_get(HX711_PORT, HX711_PIN_DOUT));
    
  	for(i=0;i<24;i++)
	{ 
	  	gpio_bit_set(HX711_PORT, HX711_PIN_SCK);  
	  	count=count<<1; 
		delay_1us(20);
		gpio_bit_reset(HX711_PORT, HX711_PIN_SCK); 
	  	if(gpio_input_bit_get(HX711_PORT, HX711_PIN_DOUT))
			count++; 
		delay_1us(20);
	} 
 	gpio_bit_set(HX711_PORT, HX711_PIN_SCK);  
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_1us(20);
	gpio_bit_reset(HX711_PORT, HX711_PIN_SCK);   
	return count;
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 


//****************************************************
//称重
//****************************************************
void Get_Weight(uint8_t status)
{
    if(status)
    {
        HX711_Buffer = HX711_Read();
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
        
        if(Weight_Shiwu > 0)			
        {
            
            Weight_Shiwu = (float)(Weight_Shiwu/(float)GapValue); 	//计算实物的实际重量
                                                            //因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
                                                            //当发现测试出来的重量偏大时，增加该数值。
                                                            //如果测试出来的重量偏小时，减小改数值。
        }
        
    }
    else
            Weight_Shiwu=0;
    
}





