#include "key_matrix.h"

static void KEY_ROW_Init(uint32_t GPIOx ,uint32_t Pinx);
static void KEY_COL_Init(uint32_t GPIOx, uint32_t Pinx);

//GPIO_GROUP GPIO_KEY_ROW[4] = {GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10};
//GPIO_GROUP GPIO_KEY_COL[4] = {GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14};
GPIO_GROUP GPIO_KEY_ROW[4] = {0};
GPIO_GROUP GPIO_KEY_COL[4] = {0};
uint32_t ROW_pin[4] = {GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10};
uint32_t COL_pin[4] = {GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14};
xKEY_MATRIX_TypeDef xKEY_MATRIX;
//控制某行电压
static void Activate_ROW(uint8_t row, uint8_t status);
//按键矩阵全部行的电压全部设置
static void Activate_ALL_ROW(uint8_t status);
/******************************************************************************
 * 函  数： KEY_MATRIX_Init
 * 功  能： 按键矩阵初始化    
 * 参  数： 无
 ******************************************************************************/  
void KEY_MATRIX_Init(void)
{
	u8 i = 0;
	//开启时钟
    rcu_periph_clock_enable(RCU_GPIOE);//使能GPIOB时钟
//	RCC_APB2PeriphClockCmd(KEY_MATRIX_APB2PERIPH,ENABLE);	
	
	//配置引脚
	//KEY ROW  	推挽输出电压
	for(i = 0; i < 4; i++) 	KEY_ROW_Init(GPIO_KEY_ROW[i].GPIOx=GPIOE,GPIO_KEY_ROW[i].Pinx=ROW_pin[i]);
	
	//KEY COL	下拉输入
	for(i = 0; i < 4; i++)	KEY_COL_Init(GPIO_KEY_COL[i].GPIOx=GPIOE,GPIO_KEY_COL[i].Pinx=COL_pin[i]);
}


static void KEY_ROW_Init(uint32_t GPIOx ,uint32_t Pinx)
{
//    gpio_mode_set();
//    gpio_output_options_set();
    gpio_mode_set(GPIOx, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, Pinx);//输出
	gpio_output_options_set(GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,Pinx);//推挽输出，50M速度
}
static void KEY_COL_Init(uint32_t GPIOx, uint32_t Pinx)
{
    gpio_mode_set(GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, Pinx);//输出
//	gpio_output_options_set(GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,Pinx);//推挽输出，50M速度
}

/******************************************************************************
 * 函  数： Activate_ROW
 * 功  能： 按键矩阵行电压变化
 * 参  数： 无
 ******************************************************************************/ 
void Activate_ROW(uint8_t row, uint8_t status)
{
	if (status) 	gpio_bit_set(GPIO_KEY_ROW[row].GPIOx, GPIO_KEY_ROW[row].Pinx);
	else 			gpio_bit_reset(GPIO_KEY_COL[row].GPIOx, GPIO_KEY_COL[row].Pinx);
}

/******************************************************************************
 * 函  数： Activate_ROW
 * 功  能： 按键矩阵全部行电压一起变化
 * 参  数： 无
 ******************************************************************************/ 
void Activate_ALL_ROW(uint8_t status)
{
	int row;
	for (row = 0; row < 4; row++) 		Activate_ROW(row, status);
}

/******************************************************************************
 * 函  数： KEY_MATRIX_Scan
 * 功  能： 从按键矩阵中读取按下按键的位置数据       
 * 参  数： 无
 * 说  明： 调用后，获取到的数据，保存到结构体xKEY_MATRIX中
 ******************************************************************************/  
void KEY_MATRIX_Scan(void)
{
	u8 row = 0,col = 0;
	//所有行电平置零复位
	Activate_ALL_ROW(0);
	//结构体res_flag清零
	xKEY_MATRIX.res_flag = 0;
	//每行电压变化 每次电压变化完检查列是否有读取到电压
	//当读取到的时候即按键按下
	for(row = 0; row < 4 ; row++)
	{
		//行电压变化
		Activate_ROW(row, 1);
		delay_1ms(1);
//        delay_ms
		for(col = 0; col < 4; col++)
		{
			//检测到高电平
			if(gpio_input_bit_get(GPIO_KEY_COL[col].GPIOx,GPIO_KEY_COL[col].Pinx) == 1)
			{
				// 结构体赋值
				xKEY_MATRIX.res_flag = 1;
				xKEY_MATRIX.res_row = row + 1;
				xKEY_MATRIX.res_col = col + 1;
				xKEY_MATRIX.res = row * 4 + col + 1;
			} 
		}
		Activate_ROW(row, 0);
	}
	//所有行电平置零复位
	Activate_ALL_ROW(0);
}
