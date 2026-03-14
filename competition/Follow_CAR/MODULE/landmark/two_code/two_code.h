#ifndef __TWO_CODE_H
#define __TWO_CODE_H
#include "all_module.h"

extern uint8_t Two_Code_Count;

void YT1_parse_two_codes(void);
void YT2_parse_two_codes(void);
void YT3_parse_two_codes(void);
void YT4_parse_two_codes(void);
void YT5_parse_two_codes(void);

/**
 * @brief 统计 8 字节数据中 1 的个数，并按奇偶性输出：
 *         - 奇数 -> 1
 *         - 偶数 -> 2
 *
 * @param data  指向 8 字节数据的指针
 * @return uint8_t 输出值（1 或 2）
 */
uint8_t TwoCode_ParityFrom8Bytes(const uint8_t *data);

/**
 * @brief 计算给定算式的结果（支持 +-*/^()%），然后对 0x1000000 取余
 *
 * @param expr 输入算式字符串（以 '\0' 结尾）
 * @return uint32_t 结果值（已经对 0x1000000 取余）
 */
uint32_t TwoCode_CalcExpressionMod(const char *expr);

/**
 * @brief 计算算式并将结果转换为十六进制字符串（带 0x 前缀）
 *
 * @param expr 输入算式字符串
 * @param out  输出缓存，必须至少能容纳 9 字符（"0x" + 6 位十六进制 + '\0'）
 */
void TwoCode_CalcExpressionHex(const char *expr, char *out);

#endif
