#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "all_module.h"


int YT_1_Algorithm(const uint8_t *key1, const uint8_t *key2, const uint8_t *cip, 
         uint8_t *plaintext, uint8_t *plainHex);

int YT_3_Algorithm(const char *input_str, const uint8_t *kernel, uint8_t kernel_size, 
                         uint8_t *output, uint8_t output_len);

#endif





