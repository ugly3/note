/**
************************************************************************
 *    文件名：二维码数据处理
 *      说明：
************************************************************************
**/
#include "two_code.h"



// 函数：从字符串中提取尖括号内的内容
uint8_t extract_data_1(const uint8_t* qr_info, uint8_t* output_buffer) {
    const char* start = strchr((const char*)qr_info, '(');
    const char* end = strchr((const char*)qr_info, ')');
    uint8_t* output_buffer1 =NULL;
    
    if (start == NULL || end == NULL || end <= start) {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return 0;
    }
    int data_len = end - start - 1;
    // 确保不会超出缓冲区大小
    if (data_len >= 50) {
        data_len = 49; // 保留一个字节给结束符
    }
    strncpy((char *)output_buffer1, start + 1, data_len);
    output_buffer1[data_len] = '\0';
    uint8_t len =strlen((char *)output_buffer1);
    uint8_t count =0;
    for(uint8_t i=0;i<len;i++)
    {
        if(((output_buffer1[i] >= 'A' && output_buffer1[i] <= 'Z') || (output_buffer1[i] >= 'a' && output_buffer1[i] <= 'b')) && count <=3)
        {
            output_buffer[count++] = output_buffer1[i];
        }
    }
    return 1;
}

// 函数：从字符串中提取大括号内的内容
uint8_t extract_data_2(const uint8_t* qr_info, uint8_t* output_buffer) {
    const char* start = strchr((const char*)qr_info, '{');
    const char* end = strchr((const char*)qr_info, '}');
    
    if (start == NULL || end == NULL || end <= start) {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return 0;
    }
    
    int data_len = end - start - 1;
    
    // 确保不会超出缓冲区大小
    if (data_len >= 50) {
        data_len = 49; // 保留一个字节给结束符
    }
    
    strncpy((char *)output_buffer, start + 1, data_len);
    output_buffer[data_len] = '\0';
    return 1;
}

// 函数：从字符串中提取大写字母A~Z
uint8_t extract_data_3(const uint8_t* qr_info, uint8_t* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= 'A' && qr_info[j] <= 'Z') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
      if(out_pos!=0)
          return 1;
      else
          return 0;
}
// 函数：从字符串中提取大写字母A~F  (样题1_RFID)
uint8_t extract_data_3_1(const uint8_t* qr_info, uint8_t* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= 'A' && qr_info[j] <= 'F') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
      if(out_pos!=0)
          return 1;
      else
          return 0;
}

// 函数：从字符串中提取字符0~9
uint8_t extract_data_4(const uint8_t* qr_info, uint8_t* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= '0' && qr_info[j] <= '9') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
      if(out_pos!=0)
          return 1;
      else
          return 0;
}

// 函数：从字符串中提取字符1~4      
uint8_t extract_data_5(const uint8_t* qr_info, uint8_t* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= '1' && qr_info[j] <= '4') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
      if(out_pos!=0)
          return 1;
      else
          return 0;
}
/**样题3：RFID处理
 * @brief 统计字母ABCD出现的频次，返回十六进制组合
 * @param qr_info 输入的RFID数据（16字节）
 * @param output_buffer 输出缓冲区（存放4个十六进制频次字符，如"2323"）
 * @return 状态码（1:成功，0:失败）
 */
uint8_t extract_data_6(const uint8_t* qr_info, uint8_t* output_buffer) 
{
    uint8_t count_A = 0, count_B = 0, count_C = 0, count_D = 0;
    uint8_t i;
    uint8_t temp;
    
    if (qr_info == NULL || output_buffer == NULL) {
        return 0; // 参数错误
    }
    
    // 统计每个字母出现的次数
    for (i = 0; i < 16; i++) {
        temp = qr_info[i];
        if (temp == 'A') 
            count_A++;
         else if (temp == 'B') 
            count_B++;
         else if (temp == 'C') 
            count_C++;
         else if (temp == 'D' ) 
            count_D++;
    }
    
    // 将十进制频次转换为十六进制字符
    // 每个频次转换为1位十六进制数（0-15）
    output_buffer[0] = (count_A <= 9) ? (count_A + '0') : (count_A - 10 + 'A');
    output_buffer[1] = (count_B <= 9) ? (count_B + '0') : (count_B - 10 + 'A');
    output_buffer[2] = (count_C <= 9) ? (count_C + '0') : (count_C - 10 + 'A');
    output_buffer[3] = (count_D <= 9) ? (count_D + '0') : (count_D - 10 + 'A');
    output_buffer[4] = '\0';  // 字符串结束符
    
    return 1;
}



//样题5：RFID数据处理,取出{}内有效数据，数字在前，字母在后，从小到大排序，取出重复数据，输出三个十六进制位
uint8_t extract_data_7(const uint8_t* qr_info, uint8_t* output_buffer) {
    const char* start = strchr((const char*)qr_info, '{');
    const char* end = strchr((const char*)qr_info, '}');
    
    if (start == NULL || end == NULL || end <= start) {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return 0;
    }
    int data_len = end - start - 1;
    // 确保不会超出缓冲区大小
    if (data_len >= 50) {
        data_len = 49; // 保留一个字节给结束符
    }
    // 临时存储提取的有效数据
    char temp_data[50] = {0};
    strncpy(temp_data, start + 1, data_len);
    temp_data[data_len] = '\0';
    
    // 统计数字和字母的出现次数
    int digit_count[10] = {0};  // 0-9
    int letter_count[6] = {0};   // A-F (对应索引0-5)
    
    // 遍历有效数据，统计每个字符出现的次数
    for (int i = 0; i < data_len; i++) {
        char c = temp_data[i];
        if (c >= '0' && c <= '9') {
            digit_count[c - '0']++;
        } else if (c >= 'A' && c <= 'F') {
            letter_count[c - 'A']++;
        }
    }

    // 收集不重复的数字和字母（只保留出现次数为1的）
    char unique_digits[10] = {0};
    char unique_letters[6] = {0};
    int digit_pos = 0;
    int letter_pos = 0;
    
    // 收集出现次数为1的数字（按0-9顺序）
    for (int i = 0; i < 10; i++) {
        if (digit_count[i] == 1) {
            unique_digits[digit_pos++] = '0' + i;
        }
    }
    // 收集出现次数为1的字母（按A-F顺序）
    for (int i = 0; i < 6; i++) {
        if (letter_count[i] == 1) {
            unique_letters[letter_pos++] = 'A' + i;
        }
    }
    // 按照数字在前，字母在后的顺序重新排列
    char sorted_data[20] = {0};
    int sorted_pos = 0;
    // 先复制数字
    for (int i = 0; i < digit_pos; i++) {
        sorted_data[sorted_pos++] = unique_digits[i];
    }
    // 再复制字母
    for (int i = 0; i < letter_pos; i++) {
        sorted_data[sorted_pos++] = unique_letters[i];
    }
    
    uint8_t code[3];
    
    // 将字符两两组合成十六进制值
    int code_pos = 0;
    // 只处理偶数个字符，如果有奇数个字符，最后一个舍弃
    int pair_count = sorted_pos / 2;  // 计算可以组成多少对
    for (int i = 0; i < pair_count * 2; i += 2) {
        uint8_t high, low;
        
        // 第一个字符（高位）
        if (sorted_data[i] >= '0' && sorted_data[i] <= '9') {
            high = sorted_data[i] - '0';
        } else {
            high = sorted_data[i] - 'A' + 10;
        }
        
        // 第二个字符（低位）
        if (sorted_data[i+1] >= '0' && sorted_data[i+1] <= '9') {
            low = sorted_data[i+1] - '0';
        } else {
            low = sorted_data[i+1] - 'A' + 10;
        }
        code[code_pos++] = (high << 4) | low;
    }
    // 将结果复制到输出缓冲区
    memcpy(output_buffer, code, 3);
    return 3; // 返回生成的字节数
}
/*************************************************************************************************/
char current_data[200] ={0};
/*
 * 2026样题1解析二维码数据
 */
void YT1_parse_two_codes(void) 
{
    // 第一步：识别A3（找包含<>的二维码）
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        char *data = (char *)Two_Code_Init_Data_Store[i];
//        char *output = NULL;
        if(strlen((char *)Two_Code_Init_Data_Store[i])==0)//无数据直接跳过
            continue;
        // 找到<>，提取中间的两个字符
        char *start = strchr(data, '<');
        char *end = strchr(data, '>');
        int len = end - start - 1;
        
        if ((start && end) && (end > start) && len==2)
        {
             Two_Code_Data_parsed_Store3[0] = start[1];
             Two_Code_Data_parsed_Store3[1] = start[2];
             Two_Code_Data_parsed_Store3[2] = '\0';
        }
        else    //没有尖括号情况或有尖括号但不是俩个字节
        {
            if(strlen((char *)Two_Code_Data_parsed_Store1)==0)
            {
                memcpy(current_data, Two_Code_Init_Data_Store[i], sizeof(Two_Code_Init_Data_Store[i]));
                extract_data_3(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);
            }
            else if(strcmp((char *)Two_Code_Init_Data_Store[i],current_data)==0)
                continue;
            else
                extract_data_3(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);
        }
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}

/*
 * 2026样题2解析二维码数据
 */
void YT2_parse_two_codes(void) 
{
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        if(Two_Code_Init_Data_Store[i][0] == 0x00)
            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);  //提取尖括号内的内容
//        else if(Two_Code_Init_Data_Store[i][0] == 0x01)
//            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);  //提取尖括号内的内容  
//        else if(Two_Code_Init_Data_Store[i][0] == 0x03)
//            extract_data_1(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store3);  //提取尖括号内的内容
        else
            continue;  // 无效的ID，跳过
        
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}


/*
 * 2026样题3解析二维码数据
 */
void YT3_parse_two_codes(void) 
{
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        if (strstr((char *)Two_Code_Init_Data_Store[i], "N01") != NULL) 
            extract_data_2(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);    //提取大括号里的内容
        else if (strstr((char *)Two_Code_Init_Data_Store[i], "N02") != NULL) 
            extract_data_2(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);    //提取大括号里的内容
        else 
            continue;
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}



/*
 * 2026样题4解析二维码数据
 */
void YT4_parse_two_codes(void) 
{
    uint8_t temp_data1[50] = {0};
    uint8_t temp_data2[50] = {0};
    if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])!=0)//同时扫描到俩个二维码
    {
        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
        extract_data_1(Two_Code_Init_Data_Store[1], temp_data2);    //提取尖括号内的内容
        Two_Code_Init_Data_Store[0][0] = '\0';
        Two_Code_Init_Data_Store[1][0] = '\0';

        // 比较数据长度，确定哪个是二维码(1)和二维码(2)
        int len1 = strlen((char *)temp_data1);
        int len2 = strlen((char *)temp_data2);

         if (len1 >= len2) 
         {
            memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
            memcpy(Two_Code_Data_parsed_Store2, temp_data2, sizeof(temp_data2));
         } 
         else 
         {
            memcpy(Two_Code_Data_parsed_Store1, temp_data2, sizeof(temp_data2));
            memcpy(Two_Code_Data_parsed_Store2, temp_data1, sizeof(temp_data1));
         }
    }
    else if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])==0)//扫描到俩个二维码其中一个
    {
        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
        Two_Code_Init_Data_Store[0][0] = '\0';
        if(strlen((char *)temp_data1)>strlen((char *)Two_Code_Data_parsed_Store1) && strlen((char *)Two_Code_Data_parsed_Store1)!=0)
        {
          memcpy(Two_Code_Data_parsed_Store2, Two_Code_Data_parsed_Store1, sizeof(Two_Code_Data_parsed_Store1));
          memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
        }
        else if(strlen((char *)temp_data1)<strlen((char *)Two_Code_Data_parsed_Store1))
          memcpy(Two_Code_Data_parsed_Store2, temp_data1, sizeof(temp_data1));
        
        else
            memcpy(Two_Code_Data_parsed_Store1, temp_data1, sizeof(temp_data1));
    }
}


/*
 * 2026样题5解析二维码数据
 */
void YT5_parse_two_codes(void) 
{
    // 第一步：识别A3
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        if(strlen((char *)Two_Code_Init_Data_Store[i])==0)//无数据直接跳过
            continue;
        
        int out_pos = 0;
        for (int j = 0; Two_Code_Init_Data_Store[i][j] != '\0' && out_pos < 49; j++) //计算字符‘0’~‘9’的个数
        {
            if (Two_Code_Init_Data_Store[i][j] >= '0' && Two_Code_Init_Data_Store[i][j] <= '9') 
                out_pos++;
        }
        if(out_pos == 2)//二维码3
            extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store3);
        
        else if(out_pos>=4)  
        {
            if(strlen((char *)Two_Code_Data_parsed_Store1)==0)
            {
                memcpy(current_data, Two_Code_Init_Data_Store[i], sizeof(Two_Code_Init_Data_Store[i]));
                extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store1);
            }
            else if(strcmp((char *)Two_Code_Init_Data_Store[i],current_data)==0)
                continue;
            else
                extract_data_4(Two_Code_Init_Data_Store[i],Two_Code_Data_parsed_Store2);
        }
        Two_Code_Init_Data_Store[i][0] = '\0';
    }
}








