#ifndef __KEY_MATRIX_H
#define __KEY_MATRIX_H
#include "gd32f4xx.h"
#include "systick.h"

#define u8 uint8_t

/*****************************************************************************
 ** 声明 全局变量
****************************************************************************/
typedef struct {
//    uint32_t GPIOx;
//    uint32_t mode;
//    uint32_t pull_up_down;
//    uint32_t Pinx
    uint32_t GPIOx;
    uint32_t Pinx;
//	uint32_t 	GPIOx;	//所用的GPIO端口
//	uint32_t 	Pinx;	//所用的Pin引脚	
}GPIO_GROUP;

typedef struct 
{
	uint8_t 	res_flag;			// 检测结果  0：没有按下	1：有按下
	uint8_t 	res_row;			// 第几行
	uint8_t		res_col;			// 第几列
	uint8_t		res;				// 1-16 	从左到右从上到下
}xKEY_MATRIX_TypeDef;


extern xKEY_MATRIX_TypeDef xKEY_MATRIX;

/*****************************************************************************
 ** 声明  全局函数
****************************************************************************/
void KEY_MATRIX_Init(void);	// 按键矩阵初始化函数

void KEY_MATRIX_Scan(void);	// 按键矩阵扫描函数

#endif
