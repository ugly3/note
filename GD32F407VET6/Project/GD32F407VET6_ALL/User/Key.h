#ifndef __KEY_H
#define __KEY_H
#include "gd32f4xx.h"                   // Device header


#define KEY_RCU_GPIOX   RCU_GPIOE
#define ROW_PORT GPIOE
#define COL_PORT GPIOE
#define NO_KEY 0xFF

// 定义键盘行和列的GPIO端口和引脚
#define ROW1_PIN GPIO_PIN_8
#define ROW2_PIN GPIO_PIN_9
#define ROW3_PIN GPIO_PIN_10
#define ROW4_PIN GPIO_PIN_11
#define COL1_PIN GPIO_PIN_12
#define COL2_PIN GPIO_PIN_13
#define COL3_PIN GPIO_PIN_14
#define COL4_PIN GPIO_PIN_15




//三个独立按键
#define KEY_tree_RCU_GPIOX     RCU_GPIOE
#define KEY_tree_PORT          GPIOE
#define KEY_tree_PIN1          GPIO_PIN_11    
#define KEY_tree_PIN2          GPIO_PIN_12 
#define KEY_tree_PIN3          GPIO_PIN_13

void key_tree_init(void);
uint8_t key_tree_scanf(void);
void Key_tree_Manage(void);


uint8_t keyscanf(void);
void COL_Di_and_ROW_Gao(void);
void COL_Gao_and_ROW_Di(void);
void OLED_view(void);    
    
    
extern unsigned char KeyCode[];
extern int OLED_SELECT_flag;   //上一次选择参数
extern int OLED_SELECT;      
extern uint8_t OLED_CONFIRM;
extern uint8_t LED_Speed_flag;
extern uint8_t task1_is_start;
extern uint8_t task2_is_start;
extern uint8_t task3_is_start;
extern uint8_t task4_is_start;
extern uint8_t task5_is_start;
extern uint8_t task6_is_start;
extern uint8_t task7_is_start;
extern uint8_t task8_is_start;
extern uint8_t task9_is_start;
extern uint8_t task10_is_start;
extern uint8_t task11_is_start;
extern uint8_t task12_is_start;

extern uint8_t task9_1_is_start;
extern uint8_t task9_2_is_start;
extern uint8_t task9_3_is_start;

extern uint8_t RFID_Stop;
extern uint8_t select_card;

extern uint32_t LightData_yuzhi;












#endif

