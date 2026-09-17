#ifndef __TWO_CODE_H
#define __TWO_CODE_H
#include "all_module.h"

void YT1_parse_two_codes(void);
void YT2_parse_two_codes(void);
void YT3_parse_two_codes(void);
void YT4_parse_two_codes(void);
void YT5_parse_two_codes(void);

extern uint8_t Two_Code_Count;

uint8_t extract_data_1(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取尖括号内的内容
uint8_t extract_data_2(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取大括号内的内容
uint8_t extract_data_3(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取大写字母A~Z
uint8_t extract_data_3_1(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取大写字母A~F
uint8_t extract_data_4(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取字符0~9
uint8_t extract_data_5(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：从字符串中提取字符1~4
uint8_t extract_data_6(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：统计字母ABCD出现的频次，返回十六进制组合
uint8_t extract_data_7(const uint8_t* qr_info, uint8_t* output_buffer);// 函数：取出{}内有效数据，数字在前，字母在后，从小到大排序，取出重复数据，输出三个十六进制位
#endif




