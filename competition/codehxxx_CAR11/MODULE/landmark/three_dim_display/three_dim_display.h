#ifndef __THREE_DIM_DISPLAY_H
#define __THREE_DIM_DISPLAY_H
#include "all_module.h"

/*************************************立体显示标志物的命令集****************************************/
//图形显示
#define  rectangle  1   //矩形
#define  circle     2   //圆形
#define  triangle   3   //三角形
#define  rhombus    4   //菱形
#define  five_star  5   //五角星

//颜色显示
#define  red        1   //红色
#define  green      2   //绿色
#define  blue       3   //蓝色
#define  yellow     4   //黄色
#define  pinse      5   //品色
#define  cyan       6   //青色
#define  black      7   //黑色
#define  white      8   //白色

//交通警示牌显示
/*
前方学校，减速慢行    1
前方施工，禁止通行    2
塌方路段，注意安全    3
追尾危险，保持车距    4
严禁，酒后驾车！      5
严禁，乱扔垃圾！      6
*/

//交通标志显示
#define  straight        1   //直行
#define  turn_left       2   //左转
#define  turn_right      3   //右转
#define  turn_round      4   //掉头
#define  no_straight     5   //禁止直行
#define  no_pass         6   //禁止通行



//自定义文本清空显示
#define  End_Add_NoClear    1   //结束累加不清除现有显示内容
#define  End_Add_Clear      2   //结束累加并清除现有显示内容

/***************************************************************************************************/


typedef struct 
{
	void (*xThree_Dim_Display_licence_coord)(char*,char,char);
	void (*xThree_Dim_Display_Distance)(uint16_t);
	void (*xThree_Dim_Display_Graph)(uint8_t);
	void (*xThree_Dim_Display_Colour)(uint8_t);
	void (*xThree_Dim_Display_Traffic_Caution)(uint8_t);
	void (*xThree_Dim_Display_Traffic_Flag)(uint8_t);
	void (*xThree_Dim_Display_Default)(void);
	void (*xThree_Dim_Display_RGB_Colour)(uint8_t,uint8_t,uint8_t);
	void (*xThree_Dim_Display_Custom_Empty)(uint8_t);
	void (*xThree_Dim_Display_Custom_Add)(uint8_t*);
    void(*xThree_Dim_Display_Custom_Add_2)(char*);
}Three_Dim_Display_Typedef;

extern Three_Dim_Display_Typedef Three_Dim_Display_Data;


void xThree_Dim_Display_licence_coord(char*str,char x,char y);
void xThree_Dim_Display_Distance(uint16_t dis);
void xThree_Dim_Display_Graph(uint8_t type);
void xThree_Dim_Display_Colour(uint8_t type);
void xThree_Dim_Display_Traffic_Caution(uint8_t type);
void xThree_Dim_Display_Traffic_Flag(uint8_t type);
void xThree_Dim_Display_Default(void);
void xThree_Dim_Display_RGB_Colour(uint8_t R,uint8_t G,uint8_t B);
void xThree_Dim_Display_Custom_Empty(uint8_t type);
void xThree_Dim_Display_Custom_Add(uint8_t*str);
void xThree_Dim_Display_Custom_Add_2(char *p);	
#endif
