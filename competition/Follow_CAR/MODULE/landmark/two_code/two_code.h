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

#endif
