//#include "ADC.h"
//#include "systick.h"
//#include "math.h"
#include "ALL.h"


//double ppm;
    
//ADC配置，DMA配置
void ADC_config(void)
{
		dma_single_data_parameter_struct  dma_single_data_parameter;
		rcu_periph_clock_enable(RCU_GPIOC);
		gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);//PA4配置成输入
		gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);//PA5配置成输入
        gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_2);//PA5配置成输入
		//ADC相关时钟配置
		rcu_periph_clock_enable(RCU_GPIOC); //启用 GPIOA 的时钟
		rcu_periph_clock_enable(RCU_DMA1);  //启用 DMA1 的时钟
		rcu_periph_clock_enable(RCU_ADC0);  //初始化 DMA1 的通道 0
		adc_clock_config(ADC_ADCCK_PCLK2_DIV6); //配置 ADC 时钟，将 ADC 时钟设置为 PCLK2 的 1/6

    /* DMA相关配置 DMA1 CH0*/
    dma_deinit(DMA1, DMA_CH0);
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));//设置 DMA 外设地址，这里设置为 ADC0 的数据寄存器地址
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  //设置外设地址是否递增，这里设置为不递增
    dma_single_data_parameter.memory0_addr = (uint32_t)(&adc_value);     //设置 DMA 存储器地址
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;     //设置存储器地址是否递增，这里设置为递增         
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;   //设置外设和存储器的数据宽度，这里设置为 16 位
    dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;                      // 循环模式
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;                              // 设置 DMA 数据传输方向，这里设置为外设到存储器
    dma_single_data_parameter.number = 3;                                                    // 设置 DMA 传输的数据量，这里设置为 2
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;                                  //设置 DMA 通道的优先级，这里设置为高优先级
    dma_single_data_mode_init(DMA1,DMA_CH0,&dma_single_data_parameter);                      //初始化 DMA1 的通道 0
    dma_channel_enable(DMA1, DMA_CH0);           //启用 DMA1 的通道 0                          
	dma_circulation_enable(DMA1, DMA_CH0);       //启用 DMA1 的通道 0 的循环模式
   
		//ADC相关配置
		adc_deinit();      // 复位ADC
		adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);                         //独立模式

		adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);               // 扫描模式
		adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);         // 连续模式
		adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);                  // 右对齐
		adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 3);              //读取2通信

		/* ADC regular channel config */
		adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_144);
		adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_11, ADC_SAMPLETIME_144);
        adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_12, ADC_SAMPLETIME_144);
		adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
		adc_dma_request_after_last_enable(ADC0);
		adc_dma_mode_enable(ADC0);
		adc_enable(ADC0);
		adc_calibration_enable(ADC0);
		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

//uint16_t Vol_Value1,Vol_Value2,Vol_Value3;

//void Show_ADC_Data(void)
//{
//    uint8_t i;
//    float  tempData = 0;
//    for (i = 0; i < 10; i++)
//    {
//        tempData += adc_value[0];
//        delay_1ms(5);
//    }
//    tempData /= 10;

//    
////    Vol_Value1=adc_value[0]*3300/4095;//转换成电压值
////	Vol_Value2=adc_value[1]*3300/4095;//转换成电压值
////    Vol_Value3=adc_value[2]*3300/4095;//转换成电压值
//}

