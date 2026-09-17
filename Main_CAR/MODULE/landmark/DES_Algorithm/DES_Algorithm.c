#include "DES_Algorithm.h"



/* ================= DES 标准置换表 ================= */

/* 初始置换表 IP */
static const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

/* 逆初始置换表 IP-1 */
static const int IP_1[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

/* 扩展置换表 E */
static const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

/* P 盒置换 */
static const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

/* S 盒 (8 个，每个 4 行 16 列) */
static const int S[8][4][16] = {
    { {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
      {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
      {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
      {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} },
    { {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
      {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
      {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
      {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9} },
    { {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
      {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
      {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
      {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12} },
    { {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
      {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
      {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
      {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14} },
    { {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
      {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
      {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
      {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3} },
    { {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
      {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
      {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
      {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13} },
    { {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
      {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
      {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
      {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12} },
    { {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
      {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
      {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
      {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11} }
};

/* 密钥置换选择 1 (PC-1) - 64 位->56 位 */
static const int PC_1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

/* 密钥置换选择 2 (PC-2) - 56 位->48 位 */
static const int PC_2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

/* 每轮左移位数 */
static const int SHIFT_SCHEDULE[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/* ================= DES 上下文 ================= */
typedef struct {
    uint8_t subkeys[16][6];  // 16 个子密钥，每个 48 位 (6 字节)
} DES_Context;

/* ================= 位操作辅助函数 ================= */

/* 从 64 位块中获取指定位 (位索引 0-63, 0 为 MSB) */
static inline int get_bit64(const uint8_t *block, int pos) {
    return (block[pos >> 3] >> (7 - (pos & 7))) & 1;
}

/* 设置 64 位块中指定位 */
static inline void set_bit64(uint8_t *block, int pos, int val) {
    if (val)
        block[pos >> 3] |= (1 << (7 - (pos & 7)));
    else
        block[pos >> 3] &= ~(1 << (7 - (pos & 7)));
}

/* 从 56 位密钥中获取指定位 */
static inline int get_bit56(const uint8_t *key, int pos) {
    return (key[pos >> 3] >> (7 - (pos & 7))) & 1;
}

/* 设置 56 位密钥中指定位 */
static inline void set_bit56(uint8_t *key, int pos, int val) {
    if (val)
        key[pos >> 3] |= (1 << (7 - (pos & 7)));
    else
        key[pos >> 3] &= ~(1 << (7 - (pos & 7)));
}

/* 通用置换函数 */
static void permute(const uint8_t *in, uint8_t *out, const int *table, int n_bits) {
    memset(out, 0, (n_bits + 7) >> 3);
    for (int i = 0; i < n_bits; i++) {
        set_bit64(out, i, get_bit64(in, table[i] - 1));
    }
}

/* ================= 密钥调度 ================= */

/* 28 位左循环移位 */
static void left_rotate_28(uint8_t *key, int shift) {
    uint8_t temp[4] = {0};
    for (int i = 0; i < shift; i++) {
        set_bit56(temp, i, get_bit56(key, i));
    }
    for (int i = 0; i < 28 - shift; i++) {
        set_bit56(key, i, get_bit56(key, i + shift));
    }
    for (int i = 0; i < shift; i++) {
        set_bit56(key, 28 - shift + i, get_bit56(temp, i));
    }
}

/* 生成 16 个子密钥 */
static void des_key_schedule(const uint8_t *key, DES_Context *ctx) {
    uint8_t k56[7] = {0};
    uint8_t c[4] = {0};
    uint8_t d[4] = {0};
    uint8_t cd[7] = {0};

    permute(key, k56, PC_1, 56);

    for (int i = 0; i < 28; i++) {
        set_bit56(c, i, get_bit56(k56, i));
        set_bit56(d, i, get_bit56(k56, i + 28));
    }

    for (int round = 0; round < 16; round++) {
        left_rotate_28(c, SHIFT_SCHEDULE[round]);
        left_rotate_28(d, SHIFT_SCHEDULE[round]);

        for (int i = 0; i < 28; i++) {
            set_bit56(cd, i, get_bit56(c, i));
            set_bit56(cd, i + 28, get_bit56(d, i));
        }

        permute(cd, ctx->subkeys[round], PC_2, 48);
    }
}

/* ================= F 函数 ================= */

static void des_f(const uint8_t *r, uint8_t *out, const uint8_t *subkey) {
    uint8_t expanded[6] = {0};
    uint8_t sbox_out[4] = {0};

    permute(r, expanded, E, 48);

    for (int i = 0; i < 6; i++) {
        expanded[i] ^= subkey[i];
    }

    for (int i = 0; i < 8; i++) {
        uint8_t six_bits = 0;
        for (int b = 0; b < 6; b++) {
            if (get_bit64(expanded, i * 6 + b)) {
                six_bits |= (1 << (5 - b));
            }
        }

        int row = ((six_bits & 0x20) >> 4) | (six_bits & 0x01);
        int col = (six_bits >> 1) & 0x0F;
        int val = S[i][row][col];

        for (int b = 0; b < 4; b++) {
            set_bit64(sbox_out, i * 4 + b, (val >> (3 - b)) & 1);
        }
    }

    permute(sbox_out, out, P, 32);
}

/* ================= DES 加解密核心 ================= */

static void des_crypt(const uint8_t *input, uint8_t *output, const DES_Context *ctx, int decrypt) {
    uint8_t ip_out[8] = {0};
    uint8_t l[4] = {0}, r[4] = {0};
    uint8_t l_temp[4] = {0};
    uint8_t f_out[4] = {0};
    uint8_t pre_final[8] = {0};

    permute(input, ip_out, IP, 64);

    memcpy(l, ip_out, 4);
    memcpy(r, ip_out + 4, 4);

    for (int round = 0; round < 16; round++) {
        memcpy(l_temp, l, 4);

        int key_idx = decrypt ? (15 - round) : round;

        des_f(r, f_out, ctx->subkeys[key_idx]);

        memcpy(l, r, 4);
        for (int i = 0; i < 4; i++) {
            r[i] = l_temp[i] ^ f_out[i];
        }
    }

    memcpy(pre_final, r, 4);
    memcpy(pre_final + 4, l, 4);

    permute(pre_final, output, IP_1, 64);
}

/* ================= 业务逻辑函数 ================= */

/* 十六进制字符串转字节数组 */
static int hex_to_bytes(const char *hex, uint8_t *bytes, int max_bytes) {
    int len = strlen(hex);
    if (len % 2 != 0) return -1;
    int byte_len = len / 2;
    if (byte_len > max_bytes) byte_len = max_bytes;
    
    for (int i = 0; i < byte_len; i++) {
        unsigned int val;
        sscanf(hex + 2 * i, "%2x", &val);
        bytes[i] = (uint8_t)val;
    }
    return byte_len;
}

/* ================= 对外接口函数 ================= */

/**
 * @brief YT_4_Algorithm - DES 解密并提取字母
 * @param input_str   输入密文 (十六进制字符串)
 * @param kernel      密钥 (字符串)
 * @param output      输出缓冲区 (存放提取的字母 ASCII 码)
 * @param output_len  输出缓冲区长度
 * @return            成功返回提取的字母数量，失败返回 -1
 */
int YT_4_Algorithm(const uint8_t *input_str, const uint8_t *kernel, 
                   uint8_t *output, uint8_t output_len) 
{
    uint8_t cipher_bytes[32] = {0};
    uint8_t key_bytes[8] = {0};
    uint8_t plain_bytes[32] = {0};

    // 1. 十六进制密文转字节
    int cipher_len = hex_to_bytes((const char *)input_str, cipher_bytes, sizeof(cipher_bytes));
    if (cipher_len <= 0 || cipher_len % 8 != 0) {
        return -1;
    }

    // 2. 密钥处理 (DES 需要 8 字节)
    int key_len = strlen((const char *)kernel);
    if (key_len > 8) key_len = 8;
    memcpy(key_bytes, kernel, key_len);
    for (int i = key_len; i < 8; i++) key_bytes[i] = 0;

    // 3. DES 密钥调度
    DES_Context ctx;
    des_key_schedule(key_bytes, &ctx);

    // 4. DES 解密
    int block_count = cipher_len / 8;
    for (int i = 0; i < block_count; i++) {
        des_crypt(cipher_bytes + (i * 8), plain_bytes + (i * 8), &ctx, 1);
    }

    // 5. 提取英文字母
    int code_idx = 0;
    for (int i = 0; i < cipher_len && code_idx < output_len; i++) {
        char c = plain_bytes[i];
        if (isalpha(c)) {
            output[code_idx++] = (uint8_t)c;
        }
    }

    return code_idx;
}
