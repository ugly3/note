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
#define RTC_CLOCK_SOURCE_LXTAL //配置时钟源
#define BKP_VALUE    0x32F1

rtc_parameter_struct   rtc_initpara;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;
/*!
    \brief      RTC configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_pre_config(void)
{
    #if defined (RTC_CLOCK_SOURCE_IRC32K) 
          rcu_osci_on(RCU_IRC32K);
          rcu_osci_stab_wait(RCU_IRC32K);
          rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);
  
          prescaler_s = 0x13F;
          prescaler_a = 0x63;
    #elif defined (RTC_CLOCK_SOURCE_LXTAL)
          rcu_osci_on(RCU_LXTAL);
          rcu_osci_stab_wait(RCU_LXTAL);
          rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);   
          prescaler_s = 0xFF;
          prescaler_a = 0x7F;
    #else
    #error RTC clock source should be defined.
    #endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}
void rtc_show_time(void)
{
    rtc_current_time_get(&rtc_initpara);  
}
/*!
    \brief      use hyperterminal to setup RTC time and alarm
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_setup(void)
{
    /* setup RTC time value */
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

		rtc_initpara.hour = 00;    
		rtc_initpara.minute = 00;
		rtc_initpara.second = 00;
   
    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)){    
    }else{
        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }
}
uint8_t aShowTime[16] = "hh:ms:ss";//定义时分秒
uint8_t aShowDate[16] = "dd-mm-yyyy";//定义年月日
int main(void)
{


    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();
    rtc_pre_config();//配置RTC
		rtc_setup(); //设置时间2016年4月30日，0点0分0秒
    
    while(1)
		{

				delay_1ms(500);//500ms等待
        rtc_show_time();//读取时间
			  sprintf((char *)aShowTime, "%2x:%2x:%2x", rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
        sprintf((char *)aShowDate, "%2x-%2x-%2x", rtc_initpara.month, rtc_initpara.date, 0x2000 + rtc_initpara.year);
    
	 }
}
