#ifndef __ANDROID_H
#define __ANDROID_H
#include "all_module.h"


typedef struct
{
	bool Android_Main_Car_Start_Flag;   //主车开始启动标志位
	bool Red_State;         // 识别为红灯
	bool Yellow_State;      // 识别为黄灯         
	bool Green_State;      // 识别为绿灯
	bool Two_Code_State;   // 识别二维码
	bool TFT_Licence_Flag;    // TFT车牌识别
	bool TFT_Traffic_Flag;    // TFT交通标志
	bool Control_TFT_Paging;        //安卓回传控制TFT向下翻页
	bool TFT_GraphCount_Flag; //TFT图形数量识别
	bool TFT_GraphColour_Flag; //TFT图形颜色识别
    bool TFT_Graph_CountAndColour_Flag; //TFT图形识别(数量+颜色)
	bool TFT_Mask_Flag;  //TFT口罩识别    
	bool TFT_Words_Flag; //TFT文字识别  
    
    bool TFT_CheXin_erro_Flag;
    bool TFT_CheXin_sucess_flag;
    bool TFT_ChePai_sucess_flag;
    
	void (*xMainCar_Send_Android)(uint8_t*);
}Typedef_Android;

extern Typedef_Android Android_Data;


extern uint8_t Two_Code_Data_Store[7];   //存储识别二维码后得到的数据
extern uint8_t TFT_License_Data_Store[6];   //TFT车牌数据存储
extern uint8_t TFT_Traffic_Data_Store[7];   //TFT交通标志数据存储
extern uint8_t TFT_GraphCount_Data_Store[7]; //TFT图形数量数据存储 
extern uint8_t TFT_GraphColour_Data_Store[7]; //TFT图形颜色数据存储
extern uint8_t TFT_GraphAndColour_Data_Store[7]; //TFT图形(数量+颜色)数据存储
extern uint8_t TFT_Mask_Data_Store[7]; //TFT口罩行人数量
extern uint8_t TFT_Words_Data_Store[7]; //TFT文字识别存储数据

extern uint8_t Identify_Traffic_Light[8];
extern uint8_t Identify_Two_Code_Arr[8];//主车发给安卓请求识别交通灯
extern uint8_t Identify_TFT_License_Arr[8];//主车发给安卓请求识别TFT车牌
extern uint8_t Identify_TFT_Traffic_Arr[8];//主车发给安卓请求识别TFT交通标志
extern uint8_t Identify_TFT_GraphCount_Arr[8];//主车发给安卓请求识别TFT图形
extern uint8_t Identify_TFT_GraphColour_Arr[8];//主车发给安卓请求识别TFT图形颜色
extern uint8_t Identify_TFT_Graph_CountAndColour_Arr[8];//主车发给安卓请求识别TFT图形（数量+颜色）
extern uint8_t Identify_TFT_Mask_Arr[8];//主车发给安卓请求识别TFT口罩行人
extern uint8_t Identify_TFT_Words_Arr[8];//主车发给安卓请求识别TFT文字识别


extern uint8_t Identify_TFT_CheXin_Arr[8];


void xMainCar_Send_Android(uint8_t* Cmd);










#endif
