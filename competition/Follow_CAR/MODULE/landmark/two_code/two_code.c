/**
************************************************************************
 *    文件名：二维码数据处理
 *      说明：
************************************************************************
**/
#include "two_code.h"



// 函数：从字符串中提取尖括号内的内容
void extract_data_1(const uint8_t* qr_info, char* output_buffer) {
    const char* start = strchr((const char*)qr_info, '<');
    const char* end = strchr((const char*)qr_info, '>');
    
    if (start == NULL || end == NULL || end <= start) {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return;
    }
    int data_len = end - start - 1;
    // 确保不会超出缓冲区大小
    if (data_len >= 50) {
        data_len = 49; // 保留一个字节给结束符
    }
    strncpy(output_buffer, start + 1, data_len);
    output_buffer[data_len] = '\0';
}

// 函数：从字符串中提取大括号内的内容
void extract_data_2(const uint8_t* qr_info, char* output_buffer) {
    const char* start = strchr((const char*)qr_info, '{');
    const char* end = strchr((const char*)qr_info, '}');
    
    if (start == NULL || end == NULL || end <= start) {
        output_buffer[0] = '\0'; // 无效格式，输出空字符串
        return;
    }
    
    int data_len = end - start - 1;
    
    // 确保不会超出缓冲区大小
    if (data_len >= 50) {
        data_len = 49; // 保留一个字节给结束符
    }
    
    strncpy(output_buffer, start + 1, data_len);
    output_buffer[data_len] = '\0';
}

// 函数：从字符串中提取大写字母
void extract_data_3(const uint8_t* qr_info, char* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= 'A' && qr_info[j] <= 'Z') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
}

// 函数：从字符串中提取字符0~9
void extract_data_4(const uint8_t* qr_info, char* output_buffer) 
{
     int out_pos = 0;
     for (int j = 0; qr_info[j] != '\0' && out_pos < 49; j++) 
     {
         if (qr_info[j] >= '0' && qr_info[j] <= '9') 
             output_buffer[out_pos++] = qr_info[j];
      }
     output_buffer[out_pos] = '\0';
}

/*************************************************************************************************/






//char current_data[200] ={0};
///*
// * 2026样题1解析二维码数据
// */
//void YT1_parse_two_codes(void) 
//{
//    // 第一步：识别A3（找包含<>的二维码）
//    for (int i = 0; i < Two_Code_Count; i++) 
//    {
//        char *data = (char *)Two_Code_Init_Data_Store[i];
//        char *output = NULL;
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
//            if(strlen(Two_Code_Data_parsed_Store1)==0)
//            {
//                strcpy(current_data, (char *)Two_Code_Init_Data_Store[i]);
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
//void YT2_parse_two_codes(void) 
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
//void YT3_parse_two_codes(void) 
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
//void YT4_parse_two_codes(void) 
//{
//   char temp_data1[50] = {0};
//    char temp_data2[50] = {0};
//    if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])!=0)//同时扫描到俩个二维码
//    {
//        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
//        extract_data_1(Two_Code_Init_Data_Store[1], temp_data2);    //提取尖括号内的内容
//        Two_Code_Init_Data_Store[0][0] = '\0';
//        Two_Code_Init_Data_Store[1][0] = '\0';

//        // 比较数据长度，确定哪个是二维码(1)和二维码(2)
//        int len1 = strlen(temp_data1);
//        int len2 = strlen(temp_data2);

//         if (len1 >= len2) 
//         {
//            strcpy(Two_Code_Data_parsed_Store1, temp_data1);
//            strcpy(Two_Code_Data_parsed_Store2, temp_data2);
//         } 
//         else 
//         {
//            strcpy(Two_Code_Data_parsed_Store1, temp_data2);
//            strcpy(Two_Code_Data_parsed_Store2, temp_data1);
//         }
//    }
//    else if(strlen((char *)Two_Code_Init_Data_Store[0])!=0 && strlen((char *)Two_Code_Init_Data_Store[1])==0)//扫描到俩个二维码其中一个
//    {
//        extract_data_1(Two_Code_Init_Data_Store[0], temp_data1);    //提取尖括号内的内容
//        Two_Code_Init_Data_Store[0][0] = '\0';
//        if(strlen(temp_data1)>strlen(Two_Code_Data_parsed_Store1) && strlen(Two_Code_Data_parsed_Store1)!=0)
//        {
//          strcpy(Two_Code_Data_parsed_Store2, Two_Code_Data_parsed_Store1);
//          strcpy(Two_Code_Data_parsed_Store1, temp_data1);
//        }
//        else if(strlen(temp_data1)<strlen(Two_Code_Data_parsed_Store1))
//          strcpy(Two_Code_Data_parsed_Store2, temp_data1);
//        
//        else
//            strcpy(Two_Code_Data_parsed_Store1, temp_data1);
//    }
//}


///*
// * 2026样题5解析二维码数据
// */
//void YT5_parse_two_codes(void) 
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
//            if(strlen(Two_Code_Data_parsed_Store1)==0)
//            {
//                strcpy(current_data, (char *)Two_Code_Init_Data_Store[i]);
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








