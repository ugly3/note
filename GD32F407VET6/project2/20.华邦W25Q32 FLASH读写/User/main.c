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
uint32_t FlashJedecid,FlashDeviceid;//FLASH ID变量
extern void Flash_TestDataInit(void);
extern uint32_t spi_flash_read_id(void);
extern uint16_t SFLASH_ReadID(void);
uint8_t ReadBuff[10],WriteBuff[10]={0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};//读写存储

extern void Flash_ReadSomeBytes(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte);
extern void Flash_WriteSomeBytes(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
extern void Flash_WriteSR(uint8_t _ucByte);
extern uint8_t Flash_ReadSR(void);
/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI2);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI2, &spi_init_struct);
    spi_enable(SPI2);	
   // spi_nss_internal_high(SPI0);

}
int main(void)
{


    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
    rcu_periph_clock_enable(RCU_GPIOC);//使能GPIOC时钟	
		rcu_periph_clock_enable(RCU_GPIOD);//使能GPIOD时钟	
		rcu_periph_clock_enable(RCU_SPI2);//使能SPI2时钟 
		gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_10);//复用功能6
		gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_11);//复用功能6
		gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_12);//复用功能6
	  gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12); /* SPI2 GPIO config:SCK/PC10, MISO/PC11, MOSI/PC12 */
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
	  gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0); /* PD0 CS */
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);	
	
		spi_config();//SPI初始化
		FlashJedecid = spi_flash_read_id();//读取Jedecid
		FlashDeviceid=SFLASH_ReadID();//读取Device ID
		/* USER CODE END 2 */
		Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
		Flash_WriteSR(0x42);//解除保护
		delay_1ms(100);
		Flash_ReadSR();//读状态寄存器
		Flash_WriteSomeBytes(WriteBuff,0,8);//把WriteBuff数组中的内容写入FLASH 0地址
		delay_1ms(100);
		Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
		while(1);
}
