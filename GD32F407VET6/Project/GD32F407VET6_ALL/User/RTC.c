#include "RTC.h"
#include "gd32f4xx_libopt.h"


#define RTC_CLOCK_SOURCE_LXTAL //配置时钟源
#define BKP_VALUE    0x32F1

rtc_parameter_struct   rtc_initpara;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;

void rtc_pre_config(void)
{
    // 清除所有复位标志
    rcu_all_reset_flag_clear();

    // 启用备份域访问
    pmu_backup_write_enable();
    
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

void rtc_setup(void)
{
    /* setup RTC time value */
//    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x25;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_MAR;//三月
    rtc_initpara.date = 0x08;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_PM;

		rtc_initpara.hour = 0x14;    
		rtc_initpara.minute = 00;
		rtc_initpara.second = 00;
   
    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)){    
    }else{
        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }
}



void MyRTC_Init(void)
{
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    rtc_pre_config();//配置RTC
	rtc_setup(); //设置时间2025年03月08日，14点0分0秒
}
   
void RTC_Show_Time(void)
{
    rtc_show_time();//读取时间
}


