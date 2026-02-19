#ifndef OLED_H
#define OLED_H

#include "gd32f4xx.h"
#include "systick.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "iic.h"

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_HEIGHT / 8)

#define OLED_RERST_RTC RCU_GPIOE
#define OLED_RERST_GPIO_Port GPIOE
#define OLED_RERST_Pin GPIO_PIN_15

// OLED 初始化
void OLED_Init(iic_bus_typ *hi2c);

// 清屏
void OLED_Clear(void);

// 刷新屏幕（将缓冲区内容写入 OLED）
void OLED_Refresh(void);

// 绘制点
void OLED_DrawPixel(uint8_t x, uint8_t y, bool color);

// 绘制线
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool color);

// 绘制矩形
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color);

// 绘制圆
void OLED_DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, bool color);

// 显示字符
void OLED_PrintChar(uint8_t x, uint8_t y, char ch, bool color);

// 显示字符串
void OLED_PrintString(uint8_t x, uint8_t y, const char *str, bool color);

// 显示数字
void OLED_PrintNumber(uint8_t x, uint8_t y, int32_t num, bool color);

// 显示位图
void OLED_DrawBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, bool color);

#endif
