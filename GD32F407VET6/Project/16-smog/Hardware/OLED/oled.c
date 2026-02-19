#include "oled.h"
#include "font.h"
static iic_bus_typ *hi2c_oled;
static uint8_t oled_buffer[OLED_BUFFER_SIZE];

// OLED I2C 地址
#define OLED_I2C_ADDR 0x78

// OLED 命令
#define OLED_CMD 0x00
#define OLED_DATA 0x40

void OLED_pin_init(void){
	
	rcu_periph_clock_enable(OLED_RERST_RTC);
	gpio_mode_set(OLED_RERST_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, OLED_RERST_Pin);
	gpio_output_options_set(OLED_RERST_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, OLED_RERST_Pin);	
	gpio_bit_write(OLED_RERST_GPIO_Port, OLED_RERST_Pin, SET);
	
}


// 发送命令
static void OLED_WriteCommand(uint8_t cmd) {
    uint8_t buf[2] = {OLED_CMD, cmd};
		IIC_Master_Transmit(hi2c_oled, OLED_I2C_ADDR, buf , 2);
}

// 发送数据
static void OLED_WriteData(uint8_t *data, uint16_t size) {
		IIC_Mem_Write(hi2c_oled, OLED_I2C_ADDR, OLED_DATA, 1, data , size);
}

// OLED 初始化
void OLED_Init(iic_bus_typ *hi2c) {
    hi2c_oled = hi2c;
		OLED_pin_init();
		gpio_bit_write(OLED_RERST_GPIO_Port, OLED_RERST_Pin, SET);
		delay_us(50);
		gpio_bit_write(OLED_RERST_GPIO_Port, OLED_RERST_Pin, RESET);
	  delay_us(50);
		gpio_bit_write(OLED_RERST_GPIO_Port, OLED_RERST_Pin, SET);
		delay_us(50);
    // 初始化命令
    OLED_WriteCommand(0xAE);  // 关闭显示
    OLED_WriteCommand(0xD5);  // 设置显示时钟分频
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);  // 设置多路复用率
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3);  // 设置显示偏移
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);  // 设置显示起始行
    OLED_WriteCommand(0x8D);  // 电荷泵设置
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0x20);  // 内存地址模式
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0xA1);  // 段重映射
    OLED_WriteCommand(0xC8);  // 扫描方向
    OLED_WriteCommand(0xDA);  // 设置 COM 引脚硬件配置
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);  // 设置对比度
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);  // 设置预充电周期
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);  // 设置 VCOMH 电压倍率
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA4);  // 显示内容不跟随 RAM
    OLED_WriteCommand(0xA6);  // 正常显示
    OLED_WriteCommand(0xAF);  // 打开显示

    OLED_Clear();
    OLED_Refresh();
}

// 清屏
void OLED_Clear(void) {
	int i;
	for(i=0;i<OLED_BUFFER_SIZE;i++){
		oled_buffer[i]=0;
	}
}

// 刷新屏幕
void OLED_Refresh(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_WriteCommand(0xB0 + page);  // 设置页地址
        OLED_WriteCommand(0x00);        // 设置列地址低 4 位
        OLED_WriteCommand(0x10);        // 设置列地址高 4 位

        OLED_WriteData(&oled_buffer[page * OLED_WIDTH], OLED_WIDTH);
    }
}

// 绘制点
void OLED_DrawPixel(uint8_t x, uint8_t y, bool color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;
    if (color) {
        oled_buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y % 8));
    } else {
        oled_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}

// 绘制线
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        OLED_DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// 绘制矩形
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color) {
    for (uint8_t i = x; i < x + width; i++) {
        OLED_DrawPixel(i, y, color);
        OLED_DrawPixel(i, y + height - 1, color);
    }
    for (uint8_t j = y; j < y + height; j++) {
        OLED_DrawPixel(x, j, color);
        OLED_DrawPixel(x + width - 1, j, color);
    }
}

// 绘制圆
void OLED_DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, bool color) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        OLED_DrawPixel(x0 + x, y0 + y, color);
        OLED_DrawPixel(x0 + y, y0 + x, color);
        OLED_DrawPixel(x0 - y, y0 + x, color);
        OLED_DrawPixel(x0 - x, y0 + y, color);
        OLED_DrawPixel(x0 - x, y0 - y, color);
        OLED_DrawPixel(x0 - y, y0 - x, color);
        OLED_DrawPixel(x0 + y, y0 - x, color);
        OLED_DrawPixel(x0 + x, y0 - y, color);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

// 显示字符
void OLED_PrintChar(uint8_t x, uint8_t y, char ch, bool color) {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t line = ASCII_font_8x16[ch - 32][i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                OLED_DrawPixel(x + j, y + i, color);
            }
        }
    }
}


// 显示字符串
void OLED_PrintString(uint8_t x, uint8_t y, const char *str, bool color) {
    while (*str) {
        OLED_PrintChar(x, y, *str++, color);
        x += 8;
        if (x >= OLED_WIDTH - 8) {
            x = 0;
            y += 8;
        }
    }
}

// 显示数字
void OLED_PrintNumber(uint8_t x, uint8_t y, int32_t num, bool color) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", num);
    OLED_PrintString(x, y, buffer, color);
}

// 显示位图
void OLED_DrawBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, bool color) {
    for (uint8_t j = 0; j < height; j++) {
        for (uint8_t i = 0; i < width; i++) {
            if (bitmap[j * width / 8 + i / 8] & (1 << (i % 8))) {
                OLED_DrawPixel(x + i, y + j, color);
            }
        }
    }
}






