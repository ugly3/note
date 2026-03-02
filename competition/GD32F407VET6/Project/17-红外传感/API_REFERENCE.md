# API 参考（模块 / 函数说明）

本文档以 API 风格列出工程中主要模块和可调用函数，包含函数签名、参数、返回值、功能说明与示例用法。

---

## 概览
- 位置：工程主要源码分布于 `User/` 与 `Hardware/` 子目录。
- 常用初始化流程（在 `main()` 中）:

```c
systick_config();
LED_config();
Btn_config();
Beep_config();
IIC_config(&hiic1);
OLED_Init(&hiic1);
IR_Init();
```

---

## User 模块

### int main(void)
- 描述：程序入口；完成外设初始化并进入主循环（轮询红外传感并更新 OLED）。
- 返回：无（嵌入式入口函数）。
- 示例：见 `User/main.c`。

---

## SysTick / 延时

文件：`User/systick.c`

### void systick_config(void)
- 描述：配置 SysTick 为 1 kHz（1ms 中断）。
- 参数：无。
- 返回：无。

### void delay_1ms(uint32_t count)
- 描述：阻塞延时 count 毫秒（依赖 SysTick 中断递减全局变量）。
- 参数：`count` — 延时的毫秒数。
- 返回：无。

### void delay_us(uint32_t nus)
- 描述：忙等待微秒延时（基于 SysTick->VAL 读取）。
- 参数：`nus` — 延时的微秒数。
- 返回：无。
- 注意：实现假设系统主频为 168 MHz（ticks = nus * 168），若系统频率不同需调整计算。

### void delay_100ns(uint32_t ns)
- 描述：极短延时（每次主循环用于极短等待）。
- 参数：`ns` — 单位为 100ns 的计数。
- 返回：无。

### void delay_decrement(void)
- 描述：由 `SysTick_Handler` 调用；递减 `delay` 计数并调用 `Beep_pulse()`（用于蜂鸣器周期翻转）。
- 参数：无。
- 返回：无。

---

## 中断向量

文件：`User/gd32f4xx_it.c`

### void SysTick_Handler(void)
- 描述：SysTick 中断服务例程；调用 `delay_decrement()`。
- 参数：无。
- 返回：无。

其他异常处理函数（`HardFault_Handler`, `NMI_Handler` 等）为默认实现（多为进入死循环）。

---

## LED

位置：`Hardware/led/led.h` / `Hardware/led/led.c`

### void LED_config(void)
- 描述：配置 4 个 LED 引脚为推挽输出并初始化为低电平（关闭）。
- 参数：无。
- 返回：无。
- 相关宏：`LED1_GPIO_Port`, `LED1_Pin`, `LED2_GPIO_Port`, ...

示例：

```c
LED_config();
gpio_bit_write(LED1_GPIO_Port, LED1_Pin, SET); // 打开 LED1
```

---

## Button（按键）

位置：`Hardware/button/button.h` / `Hardware/button/button.c`

### void Btn_config(void)
- 描述：配置按键 GPIO（PA0, PA1, PA4, PA5）为上拉输入。
- 参数：无。
- 返回：无。

### uint8_t Btn_Scan(uint8_t mode)
- 描述：按键扫描与去抖，返回按键编号（1-4）或 0 表示无按键按下。
- 参数：`mode` — 若非 0，则复位内部按键上升检测状态（用于连续检测）。
- 返回：按键编号（1..4）或 0。

示例：

```c
uint8_t key = Btn_Scan(1);
if (key == 1) { /* 处理按键1 */ }
```

---

## Beep（蜂鸣器）

位置：`Hardware/beep/beep.h` / `Hardware/beep/beep.c`

### void Beep_config(void)
- 描述：配置蜂鸣器引脚为输出并置低。
- 参数：无。
- 返回：无。

### void Beep_on(void)
- 描述：启用蜂鸣器周期性翻转（由 `Beep_pulse` 在 SysTick 中触发）。
- 参数：无。
- 返回：无。

### void Beep_off(void)
- 描述：停止蜂鸣器翻转并拉低引脚。
- 参数：无。
- 返回：无。

### void Beep_pulse(void)
- 描述：每个 SysTick 周期调用一次（定义在 `beep.c`），当 `Beep_sw` 为 1 时翻转蜂鸣器引脚以产生方波。
- 参数：无。
- 返回：无。

### void Beep_control(uint16_t time_us, uint16_t count)
- 描述：阻塞式产生指定时间与次数的脉冲（用于简单提示音）。
- 参数：`time_us` — 单次高/低时间（微秒）；`count` — 脉冲次数。
- 返回：无。

示例：

```c
Beep_config();
Beep_control(500, 4); // 产生 4 次 500us 的脉冲
```

注意：若需稳定音频输出，优先使用定时器 PWM。

---

## 软件 I2C（IIC）

位置：`Hardware/iic/iic.h` / `Hardware/iic/iic.c`

类型：

```c
typedef struct {
    rcu_periph_enum SDA_RTC;
    uint32_t SDA_PIN;
    uint32_t SDA_Port;
    rcu_periph_enum SCL_RTC;
    uint32_t SCL_PIN;
    uint32_t SCL_Port;
} iic_bus_typ;

extern iic_bus_typ hiic1; // 默认 PB10=SCL, PB11=SDA
```

基础操作：

```c
void IIC_config(iic_bus_typ *hiic);
void IIC_Start(iic_bus_typ *hiic);
void IIC_Stop(iic_bus_typ *hiic);
void IIC_Send_Byte(iic_bus_typ *hiic, uint8_t txd);
uint8_t IIC_Read_Byte(iic_bus_typ *hiic, unsigned char ack);
uint8_t IIC_Wait_Ack(iic_bus_typ *hiic);
void IIC_Ack(iic_bus_typ *hiic);
void IIC_NAck(iic_bus_typ *hiic);

void IIC_Master_Transmit(iic_bus_typ *hiic, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void IIC_Mem_Write(iic_bus_typ *hiic, uint16_t DevAddress, uint16_t MemAddress, uint8_t MemAddSize, uint8_t *pData, uint16_t Size);
uint8_t IIC_Master_Receive(iic_bus_typ *hiic, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
```

说明：
- `IIC_config`：配置 SDA/SCL 引脚为推挽输出并拉低（初始态）。
- `SDA_Mode` 可切换 SDA 为输入/输出以支持读取 ACK。
- `IIC_Wait_Ack` 超时失败时会发送 STOP 并返回 1。

示例：写入设备寄存器

```c
uint8_t buf[] = {0x12, 0x34};
IIC_Mem_Write(&hiic1, 0x78, 0x0001, 1, buf, 2);
```

注意：软件 I2C 对时序敏感，受中断和 CPU 负载影响。

---

## OLED 驱动

位置：`Hardware/OLED/oled.h` / `Hardware/OLED/oled.c`

主要接口：

```c
void OLED_Init(iic_bus_typ *hi2c);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_DrawPixel(uint8_t x, uint8_t y, bool color);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool color);
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color);
void OLED_DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, bool color);
void OLED_PrintChar(uint8_t x, uint8_t y, char ch, bool color);
void OLED_PrintString(uint8_t x, uint8_t y, const char *str, bool color);
void OLED_PrintNumber(uint8_t x, uint8_t y, int32_t num, bool color);
void OLED_DrawBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, bool color);
```

说明：
- 使用内部缓冲 `oled_buffer[OLED_BUFFER_SIZE]`（128x64 / 8 页）。
- `OLED_WriteCommand` 与 `OLED_WriteData` 使用软件 I2C API 发送数据到 I2C 地址 0x78。
- 字库 `ASCII_font_8x16` 位于 `Hardware/OLED/font.h`。

示例：打印并刷新

```c
OLED_Clear();
OLED_PrintString(0, 0, "Hello", 1);
OLED_Refresh();
```

---

## 红外（IR）

位置：`Hardware/ir/ir.h` / `Hardware/ir/ir.c`

### void IR_Init(void)
- 描述：初始化 IR 引脚（PD3）为上拉输入。

### void IR_Test(void)
- 描述：读取 IR 引脚状态并在 OLED 上显示 "somebody"（检测到）或 "nobody"（未检测），并通过 `printf` 输出调试信息。

示例：在主循环中轮询

```c
while (1) {
    IR_Test();
    delay_1ms(10);
}
```

注意：建议使用 EXTI 外部中断替代轮询以节省功耗并获得更快响应。

---

如果你需要，我可以：

- 将此文档翻译为英文；
- 为每个函数生成调用示例代码片段并插入到源码注释中；
- 生成单独的模块 README（例如 `OLED_API.md`、`IIC_API.md`）便于发布。

请选择下一步或直接回复你的偏好。
