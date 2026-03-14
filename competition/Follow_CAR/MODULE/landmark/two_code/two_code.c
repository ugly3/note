/**
************************************************************************
*    文件名：二维码数据处理
*      说明：
************************************************************************
**/
#include "two_code.h"

// 函数：从字符串中提取尖括号内的内容
uint8_t extract_data_1(const uint8_t *qr_info, uint8_t *output_buffer)
{
    const char *start = strchr((const char *)qr_info, '<');
    const char *end = strchr((const char *)qr_info, '>');

    if (start == NULL || end == NULL || end <= start)
    {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return 0;
    }
    int data_len = end - start - 1;
    // 确保不会超出缓冲区大小
    if (data_len >= 50)
    {
        data_len = 49; // 保留一个字节给结束符
    }
    strncpy((char *)output_buffer, start + 1, data_len);
    output_buffer[data_len] = '\0';
    return 1;
}

// 函数：从字符串中提取大括号内的内容
uint8_t extract_data_2(const uint8_t *qr_info, uint8_t *output_buffer)
{
    const char *start = strchr((const char *)qr_info, '{');
    const char *end = strchr((const char *)qr_info, '}');

    if (start == NULL || end == NULL || end <= start)
    {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return 0;
    }

    int data_len = end - start - 1;

    // 确保不会超出缓冲区大小
    if (data_len >= 50)
    {
        data_len = 49; // 保留一个字节给结束符
    }

    strncpy((char *)output_buffer, start + 1, data_len);
    output_buffer[data_len] = '\0';
    return 1;
}

// 函数：从字符串中提取大写字母A~Z
uint8_t extract_data_3(const uint8_t *qr_info, uint8_t *output_buffer)
{
    int out_pos = 0;
    for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++)
    {
        if (qr_info[j] >= 'A' && qr_info[j] <= 'Z')
            output_buffer[out_pos++] = qr_info[j];
    }
    output_buffer[out_pos] = '\0';
    if (out_pos != 0)
        return 1;
    else
        return 0;
}

// 函数：从字符串中提取字符0~9
uint8_t extract_data_4(const uint8_t *qr_info, uint8_t *output_buffer)
{
    int out_pos = 0;
    for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++)
    {
        if (qr_info[j] >= '0' && qr_info[j] <= '9')
            output_buffer[out_pos++] = qr_info[j];
    }
    output_buffer[out_pos] = '\0';
    if (out_pos != 0)
        return 1;
    else
        return 0;
}

// 函数：数字0~7、字母A~G提取，然后按照字母在前，数字在后组合起来
void extract_data_5(const uint8_t *input, uint8_t *output_buffer)
{
    char letters[10] = {0};
    char numbers[10] = {0};
    uint8_t letterCount = 0;
    uint8_t numberCount = 0;
    // 遍历输入字符串，提取有效字符
    for (int i = 0; input[i] != '\0'; i++)
    {
        char c = input[i];

        // 检查是否是字母A-G
        if (c >= 'A' && c <= 'G')
        {
            letters[letterCount++] = c;
        }
        // 检查是否是数字0-7
        else if (c >= '0' && c <= '7')
        {
            numbers[numberCount++] = c;
        }
    }
    // 先复制字母，再复制数字到输出
    int index = 0;
    for (int i = 0; i < letterCount; i++)
    {
        output_buffer[index++] = letters[i];
    }
    for (int i = 0; i < numberCount; i++)
    {
        output_buffer[index++] = numbers[i];
    }
    output_buffer[index] = '\0';
}
// 函数：提取出'0' '1'和+-号，循环移位后拆成高八位和低八位，最后转成俩个十进制数
void extract_data_6(const uint8_t *input, uint8_t *output_buffer)
{
    char xl[17] = {0};
    int xl_index = 0;
    char first_special = 0;
    int special_found = 0;

    // 提取0和1序列，同时找到第一个特殊字符
    for (int i = 0; i < 16; i++)
    {
        if (input[i] == '0' || input[i] == '1')
        {
            xl[xl_index++] = input[i];
        }
        else if ((input[i] == '+' || input[i] == '-') && !special_found)
        {
            first_special = input[i];
            special_found = 1;
        }
    }
    xl[xl_index] = '\0';

    //    printf("原始XL: %s\n", xl);  // 调试输出

    // 执行循环移位操作
    int len = strlen(xl); // len = 14
    char shifted[17] = {0};

    if (first_special == '+')
    {
        for (int i = 0; i < len; i++)
        {
            shifted[i] = xl[(i + 2) % len];
        }
    }
    else if (first_special == '-')
    {
        for (int i = 0; i < len; i++)
        {
            shifted[i] = xl[(i - 3 + len) % len];
        }
    }
    //    printf("移位后: %s\n", shifted);  // 调试输出
    //    printf("长度: %d\n", len);        // 调试输出

    // 提取高8位
    unsigned char DH = 0;
    for (int i = 0; i < 8; i++)
    {
        DH = (DH << 1) | (shifted[i] - '0'); // 将字符转换为十进制数字
    }

    // 提取低6位（不是低8位！）
    unsigned char DL = 0;
    for (int i = 8; i < len; i++)
    {                                        // len=14，所以i从8到13，共6位
        DL = (DL << 1) | (shifted[i] - '0'); // 将字符转换为十进制数字
    }
    // 低6位不需要左移，直接保存
    // DL已经是6位的值

    output_buffer[0] = DH;
    output_buffer[1] = DL;
}

void extract_data_7(const uint8_t *input, uint8_t *output_buffer)
{
    int i = 0;
    int letter_index = 0;     // 记录找到的字母位置
    int digit_index = 0;      // 记录找到的数字位置
    uint8_t letters[3] = {0}; // 存储3个字母
    uint8_t digits[3] = {0};  // 存储3个数字

    // 清空输出缓冲区
    output_buffer[0] = '\0';

    // 第一次遍历：收集所有A-F字母和0-9数字
    int len = strlen((const char *)input);
    for (i = 0; i < len; i++)
    {
        if (input[i] >= 'A' && input[i] <= 'F' && letter_index < 3)
        {
            letters[letter_index++] = input[i];
        }
        else if (input[i] >= '0' && input[i] <= '9' && digit_index < 3)
        {
            digits[digit_index++] = input[i];
        }
    }

    // 如果找到了3个字母和3个数字，按照XYXYXY格式组合
    if (letter_index == 3 && digit_index == 3)
    {
        output_buffer[0] = letters[0]; // X
        output_buffer[1] = digits[0];  // Y
        output_buffer[2] = letters[1]; // X
        output_buffer[3] = digits[1];  // Y
        output_buffer[4] = letters[2]; // X
        output_buffer[5] = digits[2];  // Y
        output_buffer[6] = '\0';
    }
}

/**
 * @brief 统计 8 字节数据中 1 的个数，并按奇偶性输出：
 *        - 奇数 -> 1
 *        - 偶数 -> 2
 *
 * @param data  指向 8 字节数据的指针
 * @return uint8_t 输出值（1 或 2）
 */
uint8_t TwoCode_ParityFrom8Bytes(const uint8_t *data)
{
    if (data == NULL)
        return 2;

    uint8_t count = 0;
    for (int i = 0; i < 8; i++)
    {
        uint8_t byte = data[i];
        // 统计该字节中 1 的个数
        // GNU C 提供 __builtin_popcount，但这里手动统计以保证兼容性
        for (int bit = 0; bit < 8; bit++)
        {
            count += (byte >> bit) & 0x01;
        }
    }

    return (count % 2) ? 1 : 2;
}

// ---------- 计算表达式并对 0x1000000 取余 ---------- //
static const char *TwoCode_SkipSpaces(const char *p)
{
    while (*p == ' ' || *p == '\t')
        p++;
    return p;
}

static uint32_t TwoCode_ModPow(uint32_t base, uint32_t exp)
{
    const uint32_t MOD = 0x1000000;
    uint32_t result = 1;
    base %= MOD;
    while (exp)
    {
        if (exp & 1)
            result = (uint32_t)(((uint64_t)result * base) % MOD);
        base = (uint32_t)(((uint64_t)base * base) % MOD);
        exp >>= 1;
    }
    return result;
}

static uint32_t TwoCode_ParseExpression(const char **p);
static uint32_t TwoCode_ParseTerm(const char **p);
static uint32_t TwoCode_ParseFactor(const char **p);

static uint32_t TwoCode_ParseNumber(const char **p)
{
    uint32_t value = 0;
    const char *s = *p;

    while (*s >= '0' && *s <= '9')
    {
        value = value * 10 + (uint32_t)(*s - '0');
        s++;
    }

    *p = s;
    return value % 0x1000000;
}

static uint32_t TwoCode_ParsePrimary(const char **p)
{
    const char *s = TwoCode_SkipSpaces(*p);

    if (*s == '(')
    {
        s++;
        uint32_t v = TwoCode_ParseExpression(&s);
        s = TwoCode_SkipSpaces(s);
        if (*s == ')')
            s++;
        *p = s;
        return v;
    }

    // 处理一元 + / -
    if (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        uint32_t v = TwoCode_ParsePrimary(&s);
        *p = s;
        if (op == '-')
            return (uint32_t)((0x1000000 - (v % 0x1000000)) % 0x1000000);
        return v;
    }

    uint32_t num = TwoCode_ParseNumber(&s);
    *p = s;
    return num;
}

static uint32_t TwoCode_ParseFactor(const char **p)
{
    uint32_t base = TwoCode_ParsePrimary(p);
    const char *s = TwoCode_SkipSpaces(*p);

    if (*s == '^')
    {
        s++;
        uint32_t exp = TwoCode_ParseFactor(&s); // 右结合
        base = TwoCode_ModPow(base, exp);
    }

    *p = s;
    return base % 0x1000000;
}

static uint32_t TwoCode_ParseTerm(const char **p)
{
    uint32_t value = TwoCode_ParseFactor(p);
    const char *s = TwoCode_SkipSpaces(*p);

    while (*s == '*' || *s == '/' || *s == '%')
    {
        char op = *s;
        s++;
        uint32_t rhs = TwoCode_ParseFactor(&s);

        if (op == '*')
            value = (uint32_t)(((uint64_t)value * rhs) % 0x1000000);
        else if (op == '/')
            value = (rhs == 0) ? value : (value / rhs);
        else // %
            value = (rhs == 0) ? value : (value % rhs);

        s = TwoCode_SkipSpaces(s);
    }

    *p = s;
    return value % 0x1000000;
}

static uint32_t TwoCode_ParseExpression(const char **p)
{
    uint32_t value = TwoCode_ParseTerm(p);
    const char *s = TwoCode_SkipSpaces(*p);

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        uint32_t rhs = TwoCode_ParseTerm(&s);
        if (op == '+')
            value = (value + rhs) % 0x1000000;
        else
            value = (value + 0x1000000 - (rhs % 0x1000000)) % 0x1000000;

        s = TwoCode_SkipSpaces(s);
    }

    *p = s;
    return value % 0x1000000;
}

static void TwoCode_NormalizeExpression(const char *src, char *dst, size_t dstSize)
{
    // 将常见的全角符号转换为半角符号，确保解析器可以识别
    // 支持： （ ） ＋ － ＊ ／ ％ ＾
    size_t idx = 0;
    for (size_t i = 0; src[i] != '\0' && idx + 1 < dstSize;)
    {
        unsigned char c = (unsigned char)src[i];
        if (c < 0x80)
        {
            dst[idx++] = (char)c;
            i++;
            continue;
        }

        // 处理 UTF-8 全角符号（3 字节）
        if ((unsigned char)src[i] == 0xEF && (unsigned char)src[i + 1] == 0xBC)
        {
            unsigned char c2 = (unsigned char)src[i + 2];
            switch (c2)
            {
            case 0x88: // （
                dst[idx++] = '(';
                break;
            case 0x89: // ）
                dst[idx++] = ')';
                break;
            case 0x8B: // ＋
                dst[idx++] = '+';
                break;
            case 0x8D: // －
                dst[idx++] = '-';
                break;
            case 0x8A: // ＊
                dst[idx++] = '*';
                break;
            case 0x8F: // ／
                dst[idx++] = '/';
                break;
            case 0x85: // ％
                dst[idx++] = '%';
                break;
            case 0x86: // ＾
                dst[idx++] = '^';
                break;
            default:
                // 未知全角，忽略或复制原字节
                dst[idx++] = src[i];
                break;
            }
            i += 3;
            continue;
        }

        // 非 ASCII，直接丢弃
        i++;
    }
    dst[idx] = '\0';
}

uint32_t TwoCode_CalcExpressionMod(const char *expr)
{
    if (expr == NULL)
        return 0;

    char normalized[256];
    TwoCode_NormalizeExpression(expr, normalized, sizeof(normalized));

    const char *p = normalized;
    uint32_t result = TwoCode_ParseExpression(&p);
    return result % 0x1000000;
}

void TwoCode_CalcExpressionHex(const char *expr, char *out)
{
    if (out == NULL)
        return;
    uint32_t v = TwoCode_CalcExpressionMod(expr);
    // 0x + 6 hex digits + '\0'
    sprintf(out, "0x%06x", v);
}

/*******************************************************从车二维码数据处理**********************************************/

void YT1_parse_two_codes(void)
{
    // 第一步：识别A3（找包含--/的二维码）
    for (int i = 0; i < Two_Code_Count; i++)
    {
        char *data = (char *)Two_Code_Init_Data_Store[i];
        //        char *output = NULL;
        if (strlen((char *)Two_Code_Init_Data_Store[i]) == 0) // 无数据直接跳过
            continue;
        // 找到--/，提取中间的两个字符
        char *start = strstr(data, "--/");
        start += 3;
        char *end = strstr(start, "--/");

        if ((start && end) && (end > start))
        {
            int j = 0;
            for (j = 0; start < end; start++)
                if ((*start >= '0' && *start <= '9') || (*start >= 'A' && *start <= 'F'))
                {
                    Two_Code_Data_parsed_Store2[j++] = *start;
                }
            Two_Code_Data_parsed_Store2[j] = '\0';
        }
        else // 没有--/括号情况或有--/括号
        {
            extract_data_5(Two_Code_Init_Data_Store[i], Two_Code_Data_parsed_Store1);
        }
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}
void YT2_parse_two_codes(void)
{
    for (int i = 0; i < Two_Code_Count; i++)
    {
        if (strlen((char *)Two_Code_Init_Data_Store[i]) == 0) // 无数据直接跳过
            continue;
        extract_data_5(Two_Code_Init_Data_Store[i], Two_Code_Data_parsed_Store1);
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}
void YT3_parse_two_codes(void)
{
    for (int i = 0; i < Two_Code_Count; i++)
    {
        if (strlen((char *)Two_Code_Init_Data_Store[i]) == 0) // 无数据直接跳过
            continue;
        else if (strlen((char *)Two_Code_Init_Data_Store[i]) == 3)
            memcpy(Two_Code_Data_parsed_Store2, Two_Code_Init_Data_Store[i], sizeof(Two_Code_Init_Data_Store[i]));
        else
            extract_data_6(Two_Code_Init_Data_Store[i], Two_Code_Data_parsed_Store1);
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}
void YT4_parse_two_codes(void)
{
    extract_data_7(Two_Code_Init_Data_Store[0], Two_Code_Data_parsed_Store1);
}

void YT5_parse_two_codes(void)
{
    extract_data_5(Two_Code_Init_Data_Store[0], Two_Code_Data_parsed_Store1);
}

/*******************************************************主车二维码数据处理**********************************************/
// char current_data[200] ={0};
///*
// * 2026样题1解析二维码数据
// */
// void YT1_parse_two_codes(void)
//{
//    // 第一步：识别A3（找包含<>的二维码）
//    for (int i = 0; i < Two_Code_Count; i++)
//    {
//        char *data = (char *)Two_Code_Init_Data_Store[i];
////        char *output = NULL;
//        if(strlen((char *)Two_Code_Init_Data_Store[i])==0)//无数据直接跳过
//            continue;
//        // 找到<>，提取中间的两个字符
//        char *start = strchr(data, '<');
//        char *end = strchr(data, '>');
//        int len = end - start - 1;
//
//        if ((start && end) && (end > start) && len==2)
//        {
//             Two_Code_Data_parsed_Store3[0] = start[1];
//             Two_Code_Data_parsed_Store3[1] = start[2];
//             Two_Code_Data_parsed_Store3[2] = '\0';
//        }
//        else    //没有尖括号情况或有尖括号但不是俩个字节
//        {
//            if(strlen((char *)Two_Code_Data_parsed_Store1)==0)
//            {
//                memcpy(current_data, Two_Code_Init_Data_Store[i], sizeof(Two_Code_Init_Data_Store[i]));
//                extract_data_3(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);
//            }
//            else if(strcmp((char *)Two_Code_Init_Data_Store[i],current_data)==0)
//                continue;
//            else
//                extract_data_3(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);
//        }
//        Two_Code_Init_Data_Store[i][0] = '\0';
//    }
//}

///*
// * 2026样题2解析二维码数据
// */
// void YT2_parse_two_codes(void)
//{
//    for (int i = 0; i < Two_Code_Count; i++)
//    {
//        if(Two_Code_Init_Data_Store[i][0] == 0x01)
//            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);  //提取尖括号内的内容
//        else if(Two_Code_Init_Data_Store[i][0] == 0x02)
//            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);  //提取尖括号内的内容
//        else if(Two_Code_Init_Data_Store[i][0] == 0x03)
//            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store3);  //提取尖括号内的内容
//        else
//            continue;  // 无效的ID，跳过
//
//        Two_Code_Init_Data_Store[i][0] = '\0';
//    }
//}

///*
// * 2026样题3解析二维码数据
// */
// void YT3_parse_two_codes(void)
//{
//    for (int i = 0; i < Two_Code_Count; i++)
//    {
//        if (strstr((char *)Two_Code_Init_Data_Store[i], "N01") != NULL)
//            extract_data_2(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);    //提取大括号里的内容
//        else if (strstr((char *)Two_Code_Init_Data_Store[i], "N02") != NULL)
//            extract_data_2(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);    //提取大括号里的内容
//        else
//            continue;
//        Two_Code_Init_Data_Store[i][0] = '\0';
//    }
//}

///*
// * 2026样题4解析二维码数据
// */
// void YT4_parse_two_codes(void)
//{
//    uint8_t temp_data1[50] = {0};
//    uint8_t temp_data2[50] = {0};
//    if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])!=0)//同时扫描到俩个二维码
//    {
//        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
//        extract_data_1(Two_Code_Init_Data_Store[1], temp_data2);    //提取尖括号内的内容
//        Two_Code_Init_Data_Store[0][0] = '\0';
//        Two_Code_Init_Data_Store[1][0] = '\0';

//        // 比较数据长度，确定哪个是二维码(1)和二维码(2)
//        int len1 = strlen((char *)temp_data1);
//        int len2 = strlen((char *)temp_data2);

//         if (len1 >= len2)
//         {
//            memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
//            memcpy(Two_Code_Data_parsed_Store2, temp_data2, sizeof(temp_data2));
//         }
//         else
//         {
//            memcpy(Two_Code_Data_parsed_Store1, temp_data2, sizeof(temp_data2));
//            memcpy(Two_Code_Data_parsed_Store2, temp_data1, sizeof(temp_data1));
//         }
//    }
//    else if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])==0)//扫描到俩个二维码其中一个
//    {
//        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
//        Two_Code_Init_Data_Store[0][0] = '\0';
//        if(strlen((char *)temp_data1)>strlen((char *)Two_Code_Data_parsed_Store1) && strlen((char *)Two_Code_Data_parsed_Store1)!=0)
//        {
//          memcpy(Two_Code_Data_parsed_Store2, Two_Code_Data_parsed_Store1, sizeof(Two_Code_Data_parsed_Store1));
//          memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
//        }
//        else if(strlen((char *)temp_data1)<strlen((char *)Two_Code_Data_parsed_Store1))
//          memcpy(Two_Code_Data_parsed_Store2, temp_data1, sizeof(temp_data1));
//
//        else
//            memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
//    }
//}

///*
// * 2026样题5解析二维码数据
// */
// void YT5_parse_two_codes(void)
//{
//    // 第一步：识别A3
//    for (int i = 0; i < Two_Code_Count; i++)
//    {
//        if(strlen((char *)Two_Code_Init_Data_Store[i])==0)//无数据直接跳过
//            continue;
//
//        int out_pos = 0;
//        for (int j = 0; Two_Code_Init_Data_Store[i][j] != '\0' && out_pos < 49; j++)
//        {
//            if (Two_Code_Init_Data_Store[i][j] >= '0' && Two_Code_Init_Data_Store[i][j] <= '9')
//                out_pos++;
//        }
//        if(out_pos == 2)
//            extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store3);
//
//        else if(out_pos>=4)
//        {
//            if(strlen((char *)Two_Code_Data_parsed_Store1)==0)
//            {
//                memcpy(current_data, Two_Code_Init_Data_Store[i], sizeof(Two_Code_Init_Data_Store[i]));
//                extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);
//            }
//            else if(strcmp((char *)Two_Code_Init_Data_Store[i],current_data)==0)
//                continue;
//            else
//                extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);
//        }
//        Two_Code_Init_Data_Store[i][0] = '\0';
//    }
//}
