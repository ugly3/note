/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "systick.h"

uint16_t adc_value[2];


//ADC配置，DMA配置
void ADC_config(void)
{
		dma_single_data_parameter_struct  dma_single_data_parameter;
		rcu_periph_clock_enable(RCU_GPIOA);
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);//PA4配置成输入
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);//PA5配置成输入
		//ADC相关时钟配置
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_DMA1);
		rcu_periph_clock_enable(RCU_ADC0);
		adc_clock_config(ADC_ADCCK_PCLK2_DIV6);

    /* DMA相关配置 DMA1 CH0*/
    dma_deinit(DMA1, DMA_CH0);
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(&adc_value);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;                         // 存储器地址是否为增量模式
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;                        // 循环模式
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;                                // 外设到存储器
    dma_single_data_parameter.number = 2;                                                      // DMA缓存大小
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1,DMA_CH0,&dma_single_data_parameter);
    dma_channel_enable(DMA1, DMA_CH0);
		dma_circulation_enable(DMA1, DMA_CH0);        
   
		//ADC相关配置
		adc_deinit();      // 复位ADC
		adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);                         //独立模式

		adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);               // 扫描模式
		adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);         // 连续模式
		adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);                  // 右对齐
		adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);              //读取2通信

		/* ADC regular channel config */
		adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_144);
		adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_5, ADC_SAMPLETIME_144);
		adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
		adc_dma_request_after_last_enable(ADC0);
		adc_dma_mode_enable(ADC0);
		adc_enable(ADC0);
		adc_calibration_enable(ADC0);
		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}


uint16_t Vol_Value1,Vol_Value2;
int main(void)
{

  systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
	ADC_config();	//配置ADC
	while(1)
	{
		 
				Vol_Value1=adc_value[0]*3300/4095;//转换成电压值
		  	Vol_Value2=adc_value[1]*3300/4095;//转换成电压值

		
	}
}
