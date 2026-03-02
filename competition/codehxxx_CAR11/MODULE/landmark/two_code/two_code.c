/**
************************************************************************
 *    文件名：二维码数据处理
 *      说明：
 *  通信方式：Wifi通信
************************************************************************
**/
#include "two_code.h"


/*
 * 2026样题1解析二维码数据
 */
void YT1_parse_two_codes(void) 
{
    char current_data[200];
    // 第一步：识别A3（找包含<>的二维码）
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        char *data = (char *)Two_Code_Init_Data_Store[i];
        char *output = NULL;
        if(strlen((char *)Two_Code_Init_Data_Store[i])==0)//无数据直接跳过
            continue;
        // 查找<>位置
        char *start = strchr(data, '<');
        char *end = strchr(data, '>');
        
        if ((start && end) && (end > start))
        {
            // 找到<>，提取中间的两个字符
            int len = end - start - 1;
            if (len == 2) 
            {  // 正好两个字符
                Two_Code_Data_parsed_Store3[0] = start[1];
                Two_Code_Data_parsed_Store3[1] = start[2];
                Two_Code_Data_parsed_Store3[2] = '\0';
            }
        }
        else 
        {
            if(strlen(Two_Code_Data_parsed_Store1)==0)
            {
                strcpy(current_data, (char *)Two_Code_Init_Data_Store[i]);
                output = Two_Code_Data_parsed_Store1;
            }
            else if(strcmp((char *)Two_Code_Init_Data_Store[i],current_data)==0)
                continue;
            else
                output = Two_Code_Data_parsed_Store2;;
            int out_pos = 0;

            // 提取所有大写字母
            for (int j = 0; data[j] != '\0' && out_pos < 49; j++) 
            {
                if (data[j] >= 'A' && data[j] <= 'Z') {
                    output[out_pos++] = data[j];
                }
            }
            output[out_pos] = '\0';
        }

    }

}

/*
 * 2026样题2解析二维码数据
 */
void YT2_parse_two_codes(void) 
{
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        char *data = (char *)Two_Code_Init_Data_Store[i];
        char *output = NULL;
        if(Two_Code_Init_Data_Store[i][0] == 0x01)
            output = Two_Code_Data_parsed_Store1;
        else if(Two_Code_Init_Data_Store[i][0] == 0x02)
            output = Two_Code_Data_parsed_Store2;
        else if(Two_Code_Init_Data_Store[i][0] == 0x03)
            output = Two_Code_Data_parsed_Store3;
        else
            continue;  // 无效的ID，跳过
        
        char *start = strchr(data, '<');
        char *end = strchr(data, '>');
        if ((start && end) && (end > start))
        {
            int len = end - start - 1;
            for (int j = 0; j < len; j++) 
            {
                output[j] = start[j + 1];  // start[0]是'<'，所以从start[1]开始
            }
            output[len] = '\0';  // 添加字符串结束符 
        }
        else
        {
            output[0] = '\0';  // 如果没有<>，设为空字符串
        }
        
    }
}


/*
 * 2026样题3解析二维码数据
 */
void YT3_parse_two_codes(void) 
{
    for (int i = 0; i < Two_Code_Count; i++) 
    {
        char *data = (char *)Two_Code_Init_Data_Store[i];
        char *output = NULL;
        if (strstr(data, "NO1") != NULL) 
            output = Two_Code_Data_parsed_Store1;
        else if (strstr(data, "NO2") != NULL) 
            output = Two_Code_Data_parsed_Store2;
        else 
            continue;
        
        char *start = strchr(data, '{');
        char *end = strchr(data, '}');
        if ((start && end) && (end > start))
        {
            int len = end - start - 1;
            for (int j = 0; j < len; j++) 
            {
                output[j] = start[j + 1];  // start[0]是'<'，所以从start[1]开始
            }
            output[len] = '\0';  // 添加字符串结束符 
        }
        else
        {
            output[0] = '\0';  // 如果没有<>，设为空字符串
        }
    }
    
}














