#include "I2C_Key.h"
//#include "ssd1315.h"  // 引入SSD1315驱动库[^4^]
#include "String.h"
#include "oled.h"
#include "systick.h"

// CH455G I2C地址（7位地址模式）
#define CH455G_I2C_ADDR      0x24  
#define CH455G_I2C_MASK      0x3E  // 命令掩码
#define CH455G_GET_KEY       0x0700  // 获取按键状态的命令
#define I2C_PORT             I2C0

// 按键状态定义
#define KEY_STATE_RELEASED      0
#define KEY_STATE_PRESSED       1


// 按键矩阵大小（4x4矩阵）
#define ROWS                 4
#define COLS                 4

// 密码相关定义
#define PASSWORD_LENGTH      6
#define MAX_PASSWORD_ATTEMPTS 3

// 按键状态数组
uint8_t key_state[ROWS][COLS] = {0};

// 按键信息表
typedef struct {
    uint8_t row;            // 按键所在的行
    uint8_t col;            // 按键所在的列
    char key_char;          // 按键对应的字符
} KeyInfo;

// 按键信息表
KeyInfo key_info[ROWS][COLS] = {
    {{0, 0, '1'}, {0, 1, '2'}, {0, 2, '3'}, {0, 3, 'A'}},
    {{1, 0, '4'}, {1, 1, '5'}, {1, 2, '6'}, {1, 3, 'B'}},
    {{2, 0, '7'}, {2, 1, '8'}, {2, 2, '9'}, {2, 3, 'C'}},
    {{3, 0, '*'}, {3, 1, '0'}, {3, 2, '#'}, {3, 3, 'D'}}
};

// 密码相关变量
char password[PASSWORD_LENGTH + 1] = "123456"; // 当前密码
char input_password[PASSWORD_LENGTH + 1] = {0}; // 用户输入的密码
char new_password[PASSWORD_LENGTH + 1] = {0}; // 新密码
uint8_t input_index = 0; // 输入索引
uint8_t door_flag = 0; // 解锁标志
uint8_t BUzze_flage = 0; // 错误标志
uint8_t change_password_mode = 0; // 修改密码模式标志
uint8_t password_attempts = 0; // 密码尝试次数
char display_str[PASSWORD_LENGTH + 1] = {0}; // 显示当前输入的密码

// I2C初始化函数
void key_i2cx_config(void) {
    i2c_deinit(I2C_PORT);
    i2c_clock_config(I2C_PORT, 100000, I2C_DTCY_2);  // 配置I2C时钟为100kHz
    i2c_mode_addr_config(I2C_PORT, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, CH455G_I2C_ADDR<<1);
    i2c_enable(I2C_PORT);  // 使能I2C
    i2c_ack_config(I2C_PORT, I2C_ACK_ENABLE);  // 启用ACK
}

// 从CH455G读取按键状态
uint8_t CH455G_ReadKey(void) {
    unsigned char keycode[1];
    unsigned short cmd_data1 = (unsigned char)((CH455G_GET_KEY >> 7) & CH455G_I2C_MASK) | 0x01;
    // 发送写命令（指定读取地址）
    i2c_start_on_bus(I2C_PORT);
    while (!i2c_flag_get(I2C_PORT, I2C_FLAG_SBSEND));OLED_ShowString(10,1,(u8*)"2");
    i2c_master_addressing(I2C_PORT, CH455G_I2C_ADDR<<1, I2C_TRANSMITTER);
    while (!i2c_flag_get(I2C_PORT, I2C_FLAG_ADDSEND));OLED_ShowString(18,1,(u8*)"3");
    i2c_flag_clear(I2C_PORT, I2C_FLAG_ADDSEND);
    i2c_data_transmit(I2C_PORT, cmd_data1); // 发送寄存器地址
    while (!i2c_flag_get(I2C_PORT, I2C_FLAG_TBE));OLED_ShowString(24,1,(u8*)"4");

    delay_1ms(200);
    // 切换到读模式
    i2c_start_on_bus(I2C_PORT);
    while (!i2c_flag_get(I2C_PORT, I2C_FLAG_SBSEND));OLED_ShowString(32,1,(u8*)"5");
    i2c_master_addressing(I2C_PORT, CH455G_I2C_ADDR<<1, I2C_RECEIVER); // 读模式地址
    while (!i2c_flag_get(I2C_PORT, I2C_FLAG_ADDSEND));OLED_ShowString(40,6,(u8*)"6");
    i2c_flag_clear(I2C_PORT, I2C_FLAG_ADDSEND);

    // 读取数据
    keycode[0] = i2c_data_receive(I2C_PORT);

    // 发送停止条件
    i2c_stop_on_bus(I2C_PORT);
    OLED_ShowString(48,6,(u8*)"7");
    return keycode[0];
    
}

// 按键扫描函数
void I2c_key_scan(void) {
    uint8_t key_data[2] = {0};
    uint8_t row, col;

    // 读取按键状态
    key_data[0] = CH455G_ReadKey(); // 读取第一个字节
    key_data[1] = CH455G_ReadKey();
    OLED_ShowString(56,6,(u8*)"8");
    // 更新按键状态数组
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            uint8_t mask = (1 << (row * COLS + col));
            if ((key_data[0] & mask) == 0) {
                key_state[row][col] = KEY_STATE_PRESSED;
            } else {
                key_state[row][col] = KEY_STATE_RELEASED;
            }
        }
    }
}

// 初始化I2C按键模块
void I2C_Key_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOB); // 使能GPIOB时钟
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_8); // 复用功能4
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_9); // 复用功能4
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_8); // PB10配置成I2C1
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    rcu_periph_clock_enable(RCU_I2C0); // I2C0时钟使能
    key_i2cx_config(); // 配置I2C
}
// 处理密码输入
void process_password_input(void) 
{
    uint8_t row,col;
    for ( row = 0; row < ROWS; row++) 
    {
        for ( col = 0; col < COLS; col++) 
        {
            if (key_state[row][col] == KEY_STATE_PRESSED) 
            {
                char pressed_char = key_info[row][col].key_char;

                if (change_password_mode) 
                {
                    // 修改密码模式
                    if (pressed_char == '*' && input_index == PASSWORD_LENGTH) 
                    {
                        // 确认新密码
                        strncpy(password, new_password, PASSWORD_LENGTH);
                        password[PASSWORD_LENGTH] = '\0';
//                        oled_display_password("Password changed");
                        OLED_ShowString(20,6,(u8*)"Password changed");
                        change_password_mode = 0;
                        memset(new_password, 0, PASSWORD_LENGTH + 1);
                        input_index = 0;
                    } 
                    else if (pressed_char == '#' && input_index > 0) 
                    {
                        // 删除上一个输入的字符
                        input_index--;
                        new_password[input_index] = '\0';
                    } 
                    else if (input_index < PASSWORD_LENGTH) 
                    {
                        // 输入新密码字符
                        new_password[input_index] = pressed_char;
                        input_index++;
                    }
                } 
                else // 正常解锁模式
                {
                    
                    if (pressed_char == '*' && input_index == PASSWORD_LENGTH)  // 确认输入的密码
                    {
                       
                        input_password[input_index] = '\0';
                        if (strcmp(input_password, password) == 0) 
                        {
                            door_flag = 1;
//                            oled_display_password("Door unlocked");
                            OLED_ShowString(20,0,(u8*)"Door unlocked");
                        } 
                        else 
                        {
                            BUzze_flage = 1;
//                            oled_display_password("Wrong password");
                            OLED_ShowString(10,0,(u8*)"Wrong password");
                            password_attempts++;
                            if (password_attempts >= MAX_PASSWORD_ATTEMPTS) 
                            {
//                                oled_display_password("Enter new password");
                                OLED_ShowString(10,4,(u8*)"Enter new password");
                                change_password_mode = 1;
                                password_attempts = 0;
                            }
                        }
                        // 重置输入
                        input_index = 0;
                        memset(input_password, 0, PASSWORD_LENGTH + 1);
                    } 
                    else if (pressed_char == '#' && input_index > 0) 
                    {
                        // 删除上一个输入的字符
                        input_index--;
                        input_password[input_index] = '\0';
                    } 
                    else if (input_index < PASSWORD_LENGTH) 
                    {
                        // 输入密码字符
                        input_password[input_index] = pressed_char;
                        input_index++;
                    }
                }

                
                strncpy(display_str, change_password_mode ? new_password : input_password, input_index);
//                oled_display_password(display_str);//OLED实时显示输入的密码
                    OLED_ShowString(20,2,(u8*)display_str);
                // 清除按键状态
                key_state[row][col] = KEY_STATE_RELEASED;
            }
        }
    }
}







