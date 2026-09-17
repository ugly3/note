#include "Algorithm.h"


#define MATRIX_SIZE 100

void removeDuplicates(const uint8_t *input, uint8_t *output) {
    bool seen[26] = {false};
    int outIdx = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        uint8_t c = input[i];
        if (c >= 'A' && c <= 'Z' && !seen[c - 'A']) {
            output[outIdx++] = c;
            seen[c - 'A'] = true;
        }
    }
    output[outIdx] = '\0';
}

void buildMatrix(const uint8_t *key, const uint8_t *alphabet, uint8_t matrix[5][5]) {
    uint8_t uniqueKey[27];
    removeDuplicates(key, uniqueKey);
    
    uint8_t remaining[27];
    int remIdx = 0;
    bool inKey[26] = {false};
    
    for (int i = 0; uniqueKey[i] != '\0'; i++) {
        inKey[uniqueKey[i] - 'A'] = true;
    }
    
    for (int i = 0; alphabet[i] != '\0'; i++) {
        if (!inKey[alphabet[i] - 'A']) {
            remaining[remIdx++] = alphabet[i];
        }
    }
    remaining[remIdx] = '\0';
    
    uint8_t fullStr[52];
    strcpy((char*)fullStr, (char*)uniqueKey);
    strcat((char*)fullStr, (char*)remaining);
    
    int idx = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = fullStr[idx++];
        }
    }
}

void buildPlaMatrix(uint8_t plaMatrix[5][5]) {
    const uint8_t *pla = (uint8_t*)"ABCDEFGHIKLMNOPQRSTUVWXYZ";
    int idx = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            plaMatrix[i][j] = pla[idx++];
        }
    }
}

void findPosition(uint8_t matrix[5][5], uint8_t c, int *row, int *col) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void genPla(uint8_t matrix1[5][5], uint8_t matrix2[5][5], uint8_t plaMatrix[5][5], 
            uint8_t c1, uint8_t c2, uint8_t *output) {
    int row1, col1, row2, col2;
    
    findPosition(matrix1, c1, &row1, &col1);
    findPosition(matrix2, c2, &row2, &col2);
    
    output[0] = plaMatrix[row1][col2];
    output[1] = plaMatrix[row2][col1];
    output[2] = '\0';
}

// 将字符串转换为十六进制数值数组，返回长度
int stringToHexBytes(const char *str, uint8_t *hexBytes) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        hexBytes[i] = (uint8_t)str[i];
    }
    return len;
}

// 打印十六进制数值
void printHexBytes(const uint8_t *hexBytes, int len) {
    printf("Plaintext (as hex values): ");
    for (int i = 0; i < len; i++) {
        printf("0x%02X ", hexBytes[i]);
    }
    printf("\n");
}
/********************************************************************************************************/
// YT_1_Algorithm函数返回plaintext的长度
int YT_1_Algorithm(const uint8_t *key1, const uint8_t *key2, const uint8_t *cip, 
         uint8_t *plaintext, uint8_t *plainHex) {
    const uint8_t *matrix = (uint8_t*)"ABCDEFGHIKLMNOPQRSTUVWXYZ";
    
    uint8_t matrix1[5][5];
    uint8_t matrix2[5][5];
    uint8_t plaMatrix[5][5];
    
    buildMatrix(key1, matrix, matrix1);
    buildMatrix(key2, matrix, matrix2);
    buildPlaMatrix(plaMatrix);
    
    uint8_t temp[3];
    
    // 清空plaintext
    plaintext[0] = '\0';
    
    int cip_len = strlen((char*)cip);
    for (int i = 0; i < cip_len; i += 2) {
        if (i + 1 < cip_len) {
            genPla(matrix1, matrix2, plaMatrix, cip[i], cip[i + 1], temp);
            strcat((char*)plaintext, (char*)temp);
        }
    }
    
    // 将明文转换为十六进制数值数组，并获取长度
    int hexLen = stringToHexBytes((char*)plaintext, plainHex);
    
    printf("\nCiphertext: %s\n", cip);
    printf("Plaintext (string): %s\n", plaintext);
    printHexBytes(plainHex, hexLen);
    
    return hexLen;
}

/********************************************************************************************************/
// 一维卷积函数
int YT_3_Algorithm(const char *input_str, const uint8_t *kernel, uint8_t kernel_size, 
                         uint8_t *output, uint8_t output_len) 
{
    uint8_t input_size = strlen(input_str);
    
    // 输入验证
    if (input_size < kernel_size) {
        return -1;
    }

    // Valid卷积的实际输出长度
    uint8_t valid_output_size = input_size - kernel_size + 1;
    
    // 计算Valid卷积
    for (uint8_t i = 0; i < output_len; i++) {
        if (i < valid_output_size) {
            uint16_t sum = 0;
            for (uint8_t j = 0; j < kernel_size; j++) {
                uint8_t digit = input_str[i + j] - '0';
                sum += digit * kernel[j];
            }
            output[i] = (uint8_t)(sum & 0xFF);
        } else {
            output[i] = 0;  // 超出实际长度补0
        }
    }
    
    return valid_output_size;  // 返回实际卷积结果长度
}



