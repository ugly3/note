#include "ShuMaGuan.h"
#include "systick.h"

// unsigned char  fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
// unsigned char  segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
// unsigned char  disbuf[4]={0,0,0,0};

unsigned char  LED_0F[17] = //低电平显示（共阳）
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    .
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0x8F,0x7F
};





void ShuMaGuan_Display_Init(void)
{
    rcu_periph_clock_enable(ShuMaGuan_RTC);//使能时钟
    gpio_mode_set(ShuMaGuan_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SCLK_PIN|RCLK_PIN|DIO_PIN);//配置成输出
    gpio_output_options_set(ShuMaGuan_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SCLK_PIN|RCLK_PIN|DIO_PIN);//配置成推挽输出，50M速度
}


void LED_OUT(unsigned char X)
{
	unsigned char i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) 
		{
			gpio_bit_write(ShuMaGuan_GPIO_Port, DIO_PIN, SET);
		}else 
		{
			gpio_bit_write(ShuMaGuan_GPIO_Port, DIO_PIN, RESET);
		}
		X<<=1;
        //移位寄存器时钟，上升沿有效（先拉低再拉高电平），数据将移入移位寄存器
		gpio_bit_write(ShuMaGuan_GPIO_Port, SCLK_PIN, RESET);
		gpio_bit_write(ShuMaGuan_GPIO_Port, SCLK_PIN, SET);
	}
}


void ShuMaGuan_Display(int16_t Data)
{
	unsigned char  *led_table;          // 查表指针
	unsigned char i,Data1,Data2,Data3,Data4;
    
    Data1=(Data/1000)%10;//千位
    Data2=(Data/100)%10;//百位
    Data3=(Data/10)%10;//十位
    Data4=Data%10;//个位
    
	//左边算起第1位显示
	led_table = LED_0F + Data1;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x08);		//发送位选信号，选择第1位数码管

	//存储寄存器时钟信号，或者可以理解为锁存器，当高电平时，移位寄存器的数据会复制到锁存寄存器中，并将最终结果推送到并行数据输出脚位
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);//管脚输出低
  delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);//管脚输出高
    
	//左边算起第2位显示
	led_table = LED_0F + Data2; //指针操作
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x04);		

	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
  delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
	//左边算起第3位显示
	led_table = LED_0F + Data3;
	i = *led_table&0X7F;//

	LED_OUT(i);			
	LED_OUT(0x02);	

	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
  delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
	//左边算起第4位显示
	led_table = LED_0F + Data4;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x01);		

	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
	delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
}


void ShuMaGuan_OFF(void)
{
	LED_OUT(0xFF);			
	LED_OUT(0x08);		//发送位选信号，选择第1位数码管
	//存储寄存器时钟信号，或者可以理解为锁存器，当高电平时，移位寄存器的数据会复制到锁存寄存器中，并将最终结果推送到并行数据输出脚位
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);//管脚输出低
    delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);//管脚输出高
    
	//左边算起第2位显示
	LED_OUT(0xFF);		
	LED_OUT(0x04);		
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
    delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
	//左边算起第3位显示
	LED_OUT(0xFF);			
	LED_OUT(0x02);	
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
    delay_1ms(0xFF);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
    
	//左边算起第4位显示
	LED_OUT(0xFF);			
	LED_OUT(0x01);		
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, RESET);
	delay_1ms(1);
	gpio_bit_write(ShuMaGuan_GPIO_Port, RCLK_PIN, SET);
}
















