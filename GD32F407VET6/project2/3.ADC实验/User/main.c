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
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
   /* reset ADC */
    adc_deinit();
//    /* ADC mode config */
	  adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);//独立模式
    /* ADC continous function enable */
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE); //连续转换
    /* ADC data alignment config */
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
      /* ADC resolusion 6B */
    /* ADC channel length config */
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 1);//规则通道
    /* ADC regular channel config */
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_144);
    /* ADC trigger config */
	  adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);//不采用外部触发
    /* enable ADC interface */
    adc_enable(ADC1); //使能 
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
}
uint16_t Vol_Value,adc_value;
int main(void)
{

    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOB时钟
		//配置PA4 ADC引脚
		rcu_periph_clock_enable(RCU_GPIOA);
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);//PA4配置成输入
		/* enable ADC1 clock */
		rcu_periph_clock_enable(RCU_ADC1);
		/* config ADC1 clock */
		adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
		//配置ADC
		adc_config();
	  adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);//启动
	
	while(1)
	{
		     adc_flag_clear(ADC1,ADC_FLAG_EOC);
        while(SET != adc_flag_get(ADC1,ADC_FLAG_EOC)){//查询转换完成
        }
		    adc_value = ADC_RDATA(ADC1);//读取ADC值
				Vol_Value=adc_value*3300/4095;//转换成电压值

		
	}
}
