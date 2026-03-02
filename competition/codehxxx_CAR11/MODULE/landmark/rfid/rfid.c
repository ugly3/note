/**
************************************************************************
 *    文件名：rfid
 *      说明：RFID
************************************************************************
**/
#include "rfid.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>   //isdigit(*p)判断*p是否为0~9，是的话返回1
#include <stdlib.h>  // 包含 atoi 所需的头文件
#include <math.h>


/*
注意：  数据块有16个扇区，一个扇区有4个数据块，且数据块是从0开始算起：
那么第一扇区的数据块为 0，1，2，3；   第二扇区的数据块为 4，5，6，7 ；三8，9， 10，11； 四12，13，14，15；五16，17，18，19；六20,21,22,23; 
并且每个扇区的最后一个数据块不能读写，因为此数据块是密钥块

例如2023广东省赛区题目需要读第五扇区的第2个数据块，那么需要写入 （4*（5-1）+2）=18   ；5代表扇区，3代表数据块
                                                                        
*/


extern char READ_RFID[16];		// 存放RFID读取的数据
extern uint8_t WRITE_RFID[16];	//存放RFID写入的数据 (默认"0123456789ABCDEF")
extern uint8_t KEY_A[6];   // A密钥(默认 0xff,0xff,0xff,0xff,0xff,0xff)
extern uint8_t KEY_B[6];	// B密钥(默认 0xff,0xff,0xff,0xff,0xff,0xff)


int card2_sector_block;      //卡2的地址
char card2_position[3];      //卡2在地图上的坐标

uint8_t RFID_Angle_or_shizilukou_Flag = 0;//为1时寻迹识别到位于十字路口或十字路口的RFID卡


//还未完成
uint16_t Distance = 200;//码盘前进设定距离
bool Check_Distance = 0;//为1时开始检测距离，为0时不检测
bool RFID_Read_Flag = 0; //为1时不能读取RFID卡，超出码盘设定的距离才能置为0
uint8_t RFID_Num=1;

/*相关命令集在rc522.h文件*/


//卡解密相关变量
int pos = 0; // 重置解析位置
unsigned int hex_codes[3] = {0x00,0x00,0x00};//无线充电站开启码


RFID_Typedef RFID_Data = 
{
	.xRFID_Track_Read = &xRFID_Track_Read,
    .xRFID_Track_Read_L=&xRFID_Track_Read_L,
	.xRFID_Track_Write = &xRFID_Track_Write,
	.xRFID_Write = &xRFID_Write,
	.xRFID_Read = &xRFID_Read
};


/*
保存读取RFID提取的数字
注意：这个卡一共 `16`个扇区，一个扇区 `4`个地址块，一共 `0~63`个地址块
*/


/*
RFID循迹
参数：速度
*/
void RFID_Track(uint8_t speed)
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 1;         // 循迹标志位
    Car_Spend = speed;      // 速度值
	Control(Car_Spend, Car_Spend);  // 电机驱动函数
//    while(Stop_Flag != 0x01);         //循迹完成的时候 Stop_Flag 为1         //不需要等待循迹完成	 
}

/*
注意：
需要注意寻卡寻迹有时候会寻到卡蜂鸣器也响但是没读取成功【即debug屏幕没显示读取到的数据或者看LED2,3有没有亮】，
这个属于假读取，原因是因为你读取到卡时RFID卡已经错过底部感应区，
所以此时读是读不到东西【解决方法是减慢寻卡寻迹的速度或者寻到卡后后退一点距离】

当前稳定的解决方案是寻卡寻迹速度设置为15【20都经常读取不到】 
*/

/* 
注：块地址（范围：0~63）一共16个扇区，一共 16*4=64数据块
每一块地址的最后一位都不能写，例如0~3，3地址不能写； 4~7，7地址不可以； 否则会读写不进去；

扇区数据块计算公式，  例如第1扇区第0数据块：  （(1-1)*4）+ 0 = 0；
 第2扇区第1数据块：  ((2-1)*4) + 1 = 5;
 第3扇区第2数据块：  ((3-1)*4) + 2 = 10;
 第5扇区第3数据块：  ((5-1)*4) + 3 = 19;无效块
 第6扇区第0数据块： (（6-1）*4) + 0 = 20;
 16扇区第3数据块：  ((16-1)*4) + 3 = 63;
 
数据块分别为 0,1,2,3总共四个，一个扇区4个数据块
*/

/*
十字路口开始寻一段卡（一整段不能用）
参数1：速度
参数1：前进时间
参数3：要读取卡的数据块
*/
void xRFID_Track_Read_L(uint8_t speed,uint16_t time,uint8_t card)
{
    track_time_Start=0;
    track_time_Start=1;//开始计时
    RFID_Track(speed);   
    Stop_Flag = 0;   
	while(Stop_Flag != 1)        
	{
        if(track_time_ms>=time)
        {
            Send_UpMotor(0,0);      //寻卡成功后停车
            track_time_ms=0;
            track_time_Start=0;
            Stop_Flag=1;
            Track_Flag=0;
            break;
        }
        
		if(PcdRequest(PICC_REQALL,CT) == MI_OK)  //调用寻卡函数寻卡   参数：1.0x52（寻天线内全部卡）  2. 将得到的卡片类型代码赋值到CT数组
		{
			Send_UpMotor(0,0);      //寻卡成功后停车
            track_time_ms=0;
            track_time_Start=0;
			Track_Flag = 0;         //停止循迹
            Stop_Flag=1;
              delay_ms(500);              //延时500ms后让车更稳定
              RC522(card,RFID_Read);      //读卡  
            
              if(RFID_Num == 1)  
              { 
                  RFID_Num++;
                  memcpy(READ_RFID1, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给READ_RFID1
                  memcpy(Judge_READ_RFID1, Judge_READ_RFID, sizeof(Judge_READ_RFID));//将读取到的数据传给Judge_READ_RFID1
                  Send_InfoData_To_Fifo("ADDR1\n",7);
              }
               else if(RFID_Num == 2)   
              {
                  RFID_Num++;
                  memcpy(READ_RFID2, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第二张卡
                  memcpy(Judge_READ_RFID2, Judge_READ_RFID, sizeof(Judge_READ_RFID));//将读取到的数据传给Judge_READ_RFID2
                  Send_InfoData_To_Fifo("ADDR2\n",7);
              }
              else 
              {
                  memcpy(READ_RFID3, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第三张卡
                  Send_InfoData_To_Fifo("ADDR3\n",7);   
              }
              delay_ms(300);
              break;
		}
	}//while以外
    delay_ms(500);
    Roadway_Flag_clean();
    
}


/*
功能：单一段黑线，最多连续寻2张卡
参数1：速度
参数2：卡1的数据块
参数3：卡2的数据块
参数4：卡3的数据块
*/
void xRFID_Track_Read(uint8_t speed,uint8_t card1,uint8_t card2,uint8_t card3)
{
    Roadway_Flag_clean();//清除所有标志位
    uint8_t youka = 0;   //循迹时卡序号的情况，0代表无卡
    Roadway_Flag_clean();//清除所有标志位
    
    if(0 == RFID_Angle_or_shizilukou_Flag)//寻迹没有识别到十字路口或RFID卡放在十字路口的情况
    {
        RFID_Track(speed);  //RFID循迹  
    }
    while(Stop_Flag != 1)        //在循迹中寻卡，若寻迹识别到十字路口或RFID卡放在十字路口的情况则跳出while
    {
        if(1 == RFID_Angle_or_shizilukou_Flag)//寻迹识别到RFID卡放在十字路口的情况
        {
            Stop_Flag = 1;
            Send_UpMotor(0,0);
            Track_Flag = 0;
            break;
        }
        
        if(PcdRequest(PICC_REQALL,CT) == MI_OK)  //调用寻卡函数寻卡   参数：1.0x52（寻天线内全部卡）  2. 将得到的卡片类型代码赋值到CT数组
        {
            Send_UpMotor(0,0);      //寻卡成功后停车
            Track_Flag = 0;
              delay_ms(500);              //延时500ms后让车更稳定
            
              if(RFID_Num == 1)   
              { 
                  RC522(card1,RFID_Read);
                  RFID_Num++;
                  memcpy(READ_RFID1, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第一张卡 
                  memcpy(Judge_READ_RFID1, Judge_READ_RFID, sizeof(Judge_READ_RFID));//将读取到的数据传给Judge_READ_RFID1
                  Send_InfoData_To_Fifo("ADDR1\n",7);
              }
              else if(RFID_Num == 2)   
              {
                  RC522(card2,RFID_Read);
                  RFID_Num++;
                  memcpy(READ_RFID2, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第二张卡 
                  memcpy(Judge_READ_RFID2, Judge_READ_RFID, sizeof(Judge_READ_RFID));//将读取到的数据传给Judge_READ_RFID2
                  Send_InfoData_To_Fifo("ADDR2\n",7);
              }
              else 
              {
                  RC522(card3,RFID_Read);
                  memcpy(READ_RFID3, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第三张卡
                  Send_InfoData_To_Fifo("ADDR3\n",7);   
              }
              youka=1;
            break;
          }
      }
     
      if(youka==1)
      {
        RFID_Track(speed);      //循卡2
        delay_ms(500);          //让车在500ms内先不寻卡，避开卡1
        while(Stop_Flag != 1)        //在循迹中寻卡，若寻迹识别到十字路口或RFID卡放在十字路口的情况则跳出while
        {
            if(1 == RFID_Angle_or_shizilukou_Flag)//寻迹识别到RFID卡放在十字路口的情况
            {
                Stop_Flag = 1;
                Send_UpMotor(0,0);
                Track_Flag = 0;
                break;
            }
            
            if(PcdRequest(PICC_REQALL,CT) == MI_OK)  //调用寻卡函数寻卡   参数：1.0x52（寻天线内全部卡）  2. 将得到的卡片类型代码赋值到CT数组
            {
                Send_UpMotor(0,0);      //寻卡成功后停车
                Track_Flag = 0;
                  delay_ms(500);        //延时500ms后让车更稳定     
                     
                  if(RFID_Num == 2)   //读取第二张卡，将数据给第二张卡
                  {
                      RFID_Num++;
                      RC522(card2,RFID_Read); 
                      memcpy(READ_RFID2, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第二张卡 
                      Send_InfoData_To_Fifo("ADDR2\n",7);
                  }
                  else 
                  {     
                      RC522(card3,RFID_Read); 
                      memcpy(READ_RFID3, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第三张卡
                      Send_InfoData_To_Fifo("ADDR3\n",7);   
                  }
                  Roadway_Flag_clean();
                  youka=2;
                break;
              }
          }  
        if(youka==2)
        {
            Motor_Data.xCAR_Track_Go();//卡2与卡1在同一段被检测到，但卡2不在十字路口附近，则直接普通循迹到路口
        }
      }
      
      if(1 == RFID_Angle_or_shizilukou_Flag && youka!=2)
      {
          if(RFID_Num==1)
          {
              RFID_Data.xRFID_Read(card1);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card1);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card1);
              Roadway_Flag_clean();
          }
          else if(RFID_Num==2)
          {
              RFID_Data.xRFID_Read(card2);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card2);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card2);
              Roadway_Flag_clean();
          }
          else
          {
              RFID_Data.xRFID_Read(card3);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card3);
              Motor_Data.xCAR_Go(40,200);
              RFID_Data.xRFID_Read(card3);
              Roadway_Flag_clean();
          }
      }
    
}



/*
RFID寻卡+写卡
参数1：循迹速度
参数2：需要写的数据块
*/
void xRFID_Track_Write(uint8_t speed,uint8_t card)
{
	RFID_Track(speed);
	while(Stop_Flag != 0x01)        //在循迹中寻卡，若循迹完成则跳出while
	{
		if(PcdRequest(PICC_REQALL,CT) == MI_OK)     //寻卡成功
		{
			Send_UpMotor(0,0);      //寻卡成功后停车
			Track_Flag = 0;
			for(char i=0;i<10;i++)     //停车后报警10次
           {			
               MP_SPK = 1;             //打开蜂鸣器
               delay_ms(100); 
			   MP_SPK = 0;             //关闭蜂鸣器
			   delay_ms(100);
		   }
           RC522(card,RFID_Write);     //写卡
		   
           delay_ms(500);              //延时500ms后让车更稳定
#if RFID_CARD_2
           delay_ms(500);
		   RFID_Track(speed);   //如果有两张RFID卡，则继续循迹
#endif      
           break;		   
        } 			
	}
}

/*
定点写卡
参数：地址块（0~63）
*/
void xRFID_Write(uint8_t add)
{
	PcdRequest(PICC_REQALL,CT);        //先寻卡，再写卡
	RC522(add,RFID_Write);
}


/*
功能：定点读卡
参数：地址块（0~63）
*/
void xRFID_Read(uint8_t add)
{
    uint8_t Buf[17]={0};
	if(PcdRequest(PICC_REQALL,CT)== MI_OK)        
    {
         RC522(add,RFID_Read);
        if(RFID_Num == 1)   
        { 
            RFID_Num++;
            memcpy(READ_RFID1, READ_RFID, sizeof(READ_RFID)); //将读取到的数据传给第一张卡
            sprintf((char*)Buf,"%s \r\n",READ_RFID1);  
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
          
            Send_InfoData_To_Fifo("ADDR1\n",7);
        }
        else if(RFID_Num == 2)   
        {
            RFID_Num++;
            memcpy(READ_RFID2, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第二张卡 
            sprintf((char*)Buf,"%s \r\n",READ_RFID2);  
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
            
            Send_InfoData_To_Fifo("ADDR2\n",7);
        }
        else    
        {
            memcpy(READ_RFID3, READ_RFID, sizeof(READ_RFID));//将读取到的数据传给第三张卡
            sprintf((char*)Buf,"%s \r\n",READ_RFID3);  
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
            
            Send_InfoData_To_Fifo("ADDR3\n",7);   
        }
    }
}

void YT5_parseCard1(const char* card1Data, char* formula, char* position)    //解析卡1数据，提取公式和坐标
{
	const char* p = card1Data;
	char temp[128] = {0}; // 临时存储过滤后的字符串
	int index = 0;
	// 过滤干扰字符
	while (*p) 
	{
			if (isalnum(*p) || strchr("()+-*/^%CD", *p)) 
			{
					temp[index++] = *p;
			}
			p++;
	}
	 temp[index] = '\0';
	 // 区分公式和坐标
    char* token = strtok(temp, "()");
    if (token) 
		{
        strcpy(position, token); // 坐序点
        token = strtok(NULL, "");
        if (token) 
				{
            strcpy(formula, token); // 公式
        }
    }
}

// 简单的表达式解析器（支持 +, -, *, /）
double eval(const char* expr) {
    int result = 0;
    int currentNumber = 0;
    char currentOp = '+';
    int i = 0;

    while (expr[i] != '\0') {
        if (isdigit(expr[i])) {
            currentNumber = currentNumber * 10 + (expr[i] - '0');
        } else if (strchr("+-*/", expr[i])) {
            switch (currentOp) {
                case '+': result += currentNumber; break;
                case '-': result -= currentNumber; break;
                case '*': result *= currentNumber; break;
                case '/': result /= currentNumber; break;
            }
						currentNumber = 0;
            currentOp = expr[i];
        }
        i++;
    }

    // 处理最后一个数字
    switch (currentOp) {
        case '+': result += currentNumber; break;
        case '-': result -= currentNumber; break;
        case '*': result *= currentNumber; break;
        case '/': result /= currentNumber; break;
    }
		 return result;
}


// 计算公式结果
double calculateFormula(const char* formula, int a, int b, int c, int d) 
{
    // 使用递归解析和计算公式
    // 这里假设公式格式正确，不包含嵌套括号
    int result = 0;
    char temp[128];
    strcpy(temp, formula);
		// 替换变量为实际值
    char* token = strstr(temp, "a");
    while (token) {
        *token = '3'; // a = 3
        token = strstr(token + 1, "a");
    }

    token = strstr(temp, "b");
    while (token) 
			{
        *token = '1'; // b = 1
        token = strstr(token + 1, "b");
    }
		 token = strstr(temp, "c");
    while (token) {
        *token = '2'; // c = 2
        token = strstr(token + 1, "c");
    }

    token = strstr(temp, "d");
    while (token) {
        *token = '5'; // d = 5
        token = strstr(token + 1, "d");
    }

    // 计算公式结果
    result = eval(temp); // 假设 eval 函数可以解析并计算公式
    return result;
}
// 确定卡2的位置
void determineCard2Position(const char* position, char* sortedPosition) 
{
    char temp[3];
    sscanf(position, "%[^0-9]%d", temp, (int*)&sortedPosition[1]);
    sortedPosition[0] = temp[0];
    sortedPosition[2] = '\0';
}
// 读取卡2数据（假设函数）
void readCard2Data(int sector, char* data) 
{
    // 模拟读取卡2数据
    // 实际应用中需要根据硬件接口实现
    static const char* mockData = "0123456789ABCDEF"; // 示例数据
    strcpy(data, mockData);
}
void YT5_Handle_Card1_Data(char *data)
{
	const char* card1Data = "！(d*$c^4(a+b)C)";
    char formula[128] = {0};
    char position[3] = {0};
    char sortedPosition[3] = {0};
    char card2Data[17] = {0};

    // 解析卡1数据
    YT5_parseCard1(card1Data, formula, position);

    // 确定卡2的位置
    determineCard2Position(position, sortedPosition);

    // 计算公式结果
    double M = calculateFormula(formula, 3, 1, 2, 5); // a=3, b=1, c=2, d=5
		// 读取卡2数据
    readCard2Data(M, card2Data);
		
    // 打印结果
//    printf("Formula: %s\n", formula);
//    printf("Position: %s\n", position);
//    printf("Sorted Position: %s\n", sortedPosition);
//    printf("M (Formula Result): %.f\n", M);
//    printf("Card2 Data: %s\n", card2Data);
		uint8_t Buf[30];
		sprintf((char*)Buf,"Formula:%s \r\n",formula); 
		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));	
		
		sprintf((char*)Buf,"Position:%s \r\n",position);
		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
		
		sprintf((char*)Buf,"Sorted Position:%s \r\n",sortedPosition);
		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
		
		sprintf((char*)Buf,"M (Formula Result):%.f \r\n",M);
		Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
		
		sprintf((char*)Buf,"Card2 Data:%s \r\n",card2Data);  
    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
    
}





/*
功能：样题1卡1的数据处理
*/
void parseCard1(const char* card1Data, int* sector_block,  char* position) // 函数：解析卡1数据
{
    // 提取有效数据（如“62”）
    const char *p = card1Data;
     char numStr[3]; // 假设最多两位数字加上一个终止符
     size_t i = 0;

    while(*p!='<')
    {
       while (isdigit(*p) && i < 2) { // 读取最多两位数字
        numStr[i++] = *p++;
        }  
         p++;   
    }
    numStr[i] = '\0'; // 字符串结束符
    
    
    *sector_block = atoi(numStr); // 将字符串转换为整数
    *sector_block = (*sector_block/10 - 1) * 4 + (*sector_block%10)+1;

    // 提取位置信息（如“D3”）
    if (sscanf(card1Data,"%*[^<]<%[^>]>%*s", position) != 1) 
    {
        Send_InfoData_To_Fifo("Failed position\r\n",17);
        position[0] = '\0'; // 设置为空字符串
        return;
    }
    
    // char tempChar; // 临时变量用于存储%c匹配的字符
    // if (sscanf(position,"%c%d", &tempChar, block) != 2 ) {
    //     printf("Failed to extract block number or invalid block number\n");
    //     *block = -1; // 设置为无效值
    //     return;
    // }
    // printf("Extracted block: %d\n", *block); // 调试输出

}
void YT1_Handle_Card1_Data(char *data)
{
   uint8_t Buf[30];
   
    parseCard1(data, &card2_sector_block, card2_position);
    
    sprintf((char*)Buf,"%s\r\n",card2_position);  
    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
    
    sprintf((char*)Buf,"%d\r\n",card2_sector_block);  
    Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
}




/*
功能：样题2卡1的数据处理
        提取公式和坐标
*/

double parseExpression(const char *expr, double values[26]);

// 解析数字
double parseNumber(const char *expr) {
    double result = 0.0;
    while (isdigit(expr[pos])) {
        result = result * 10 + (expr[pos++] - '0');
    }
    return result;
}

// 解析因子（数字、变量或括号表达式）
double parseFactor(const char *expr, double values[26]) {
    double result = 0.0;
    if (isdigit(expr[pos])) {
        result = parseNumber(expr);
    } else if (expr[pos] == '(') {
        pos++; // 跳过左括号
        result = parseExpression(expr, values);
        if (expr[pos] != ')') {
//            printf("Error: Missing closing parenthesis\n");
            return -1; // 返回错误状态
        }
        pos++; // 跳过右括号
    } else if (islower(expr[pos])) {
        char var = expr[pos++];
        result = values[var - 'a'];
    } else {
//        printf("Error: Invalid character at position %d\n", pos);
        return -1; // 返回错误状态
    }
    return result;
}

// 解析指数运算
double parseExponent(const char *expr, double values[26]) {
    double result = parseFactor(expr, values);
    while (expr[pos] == '^') {
        pos++; // 跳过 '^'
        double right = parseFactor(expr, values);
        result = pow(result, right);
    }
    return result;
}
// 解析项（乘法和除法）
double parseTerm(const char *expr, double values[26]) {
    double result = parseExponent(expr, values);
    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos++];
        double right = parseExponent(expr, values);
        if (op == '*') {
            result *= right;
        } else {
            if (right == 0) {
                return -1;
            }
            result /= right;
        }
    }
    return result;
}

// 解析表达式（加法和减法）
double parseExpression(const char *expr, double values[26]) {
    double result = parseTerm(expr, values);
    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos++];
        double right = parseTerm(expr, values);
        if (op == '+') 
        {
            result += right;
        } else {
            result -= right;
        }
    }
    return result;
}
// 判断是否是有效字符
int isValidChar(char c)
{
    return ((c >= 'a' && c <= 'e') || (c >= '0' && c <= '9') || c == 'C' || c == 'D' || c == 'E' ||
            c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')');
}
// 判断是否是坐标字符（大写字母或数字）
int isCoordChar(char c) 
{
    return (c == 'C' || c == 'D' || c == 'E' || (c >= '0' && c <= '9'));
}
void extractFormulaAndCoord(const char *input, char *formula, char *coord) 
{
    int i = 0,  k = 0;
    while (input[i] != '\0') {
        if (isValidChar(input[i])) //有效数据
        {
            if (isCoordChar(input[i])) //位置信息
            {
                if(input[i]>= '0' && input[i] <= '9'){
                    coord[1] = input[i];          
                } else {
                    coord[0] = input[i];
                }
            } 
            else//公式
            {
                formula[k++] = input[i];
            }
        }
        i++;
    }
    coord[2] = '\0'; // 结束坐标字符串
    formula[k] = '\0'; // 结束公式字符串
}



//样题2卡的处理
void YT2_Handle_Card1_Data(char *data)
{
    uint8_t Buf[30];
    char formula[20];
    
    double anzuo_send_tuxing[26]={2,2,3,1};//安卓发来的数据(要求按字母的顺序发过来),a,b,c,d

    // 提取公式和坐标
    extractFormulaAndCoord(data, formula, card2_position);

    double result = parseExpression(formula, anzuo_send_tuxing);//结果这个数有用
//    if (result == -1) {
//        Send_InfoData_To_Fifo("erro\r\n",4); 
//    }
//    else 
//    {        
        sprintf((char*)Buf,"%s\r\n",formula);  
        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
        
        sprintf((char*)Buf,"%f\r\n",result);  
        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
        
        sprintf((char*)Buf,"%s\r\n",card2_position);  
        Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
//    }
}



/*
功能：样题3卡的数据处理
        提取无线充电站的开启码
*/

#define MAX_DATA_LENGTH 16
#define MAX_HEX_VALUES 16
#define NUM_HEX_CODES 3  // 期望生成的十六进制码数量

// 检查是否为有效卡
int is_valid_card(const char* card_data) 
{
    return strcmp(card_data, "CARD01") == 0;
}
// 去除重复字符（重复的字符完全移除）
void remove_duplicates(char* values, int* count) 
{
    int seen[MAX_HEX_VALUES] = {0};  // 标记字符出现次数
    char result[MAX_HEX_VALUES];     // 存储最终结果
    int result_count = 0;

    // 统计每个字符的出现次数
    for (int i = 0; i < *count; i++) {
        char c = values[i];
        int index = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;
        seen[index]++;
    }

    // 保留只出现一次的字符
    for (int i = 0; i < *count; i++) {
        char c = values[i];
        int index = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;
        if (seen[index] == 1) {
            result[result_count++] = c;
        }
    }

    // 将结果复制回原数组
    for (int i = 0; i < result_count; i++) {
        values[i] = result[i];
    }
    values[result_count] = '\0';  // 确保字符串以 '\0' 结尾
    *count = result_count;  // 更新字符数量
}
// 排序：数字在前，字母在后
void sort_values(char* values, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                char temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
}
// 转换为十六进制码
void convert_to_hex_codes(char* values, int count, unsigned int* hex_codes) {
    int index = 0;  // 当前处理的十六进制码索引
    int hex_value = 0;  // 当前十六进制值

    for (int i = 0; i < count; i++) {
        hex_value = hex_value * 16 + (values[i] >= '0' && values[i] <= '9' ? values[i] - '0' :
                                      values[i] >= 'A' && values[i] <= 'F' ? values[i] - 'A' + 10 : 0);
        if ((i + 1) % 2 == 0 || i == count - 1) {  // 每两个字符或最后一个字符
            hex_codes[index++] = hex_value;
            hex_value = 0;
        }
    }
}


// 处理卡数据 
void process_card_data(const char* card_data, unsigned int* hex_codes) {
    char temp_values[MAX_HEX_VALUES];
    int count = 0;

    // 使用 sscanf 提取 {} 内的数据
    char extracted_data[MAX_DATA_LENGTH];
    if (sscanf(card_data, "%*[^{]{%[^}]", extracted_data) == 1) {
        // 提取有效数据
        for (int i = 0; extracted_data[i] != '\0'; i++) {
            if (isxdigit(extracted_data[i])) {
                temp_values[count++] = extracted_data[i];
            }
        }
    }

    // 去除重复字符
    remove_duplicates(temp_values, &count);

    // 排序
    sort_values(temp_values, count);

    // 转换为十六进制码
    convert_to_hex_codes(temp_values, count, hex_codes);
}

void YT3_Handle_Card1_Data(void)
{
    uint8_t Buf[30];
    

    // 检查卡1是否有效
    if (is_valid_card(Judge_READ_RFID1)) {
        process_card_data(READ_RFID1, hex_codes);
      
        for (int i = 0; i < 3; i++) {
            sprintf((char*)Buf,"0x%X ",hex_codes[i]);  
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
        }
        Send_InfoData_To_Fifo("\r\n",2);
    }

    // 检查卡2是否有效
    if (is_valid_card(Judge_READ_RFID2)) {
        process_card_data(READ_RFID2, hex_codes);
        
        for (int i = 0; i < 3; i++) {
            sprintf((char*)Buf,"0x%X ",hex_codes[i]);  
            Send_InfoData_To_Fifo((char*)Buf,strlen((char*)Buf));
        }
        Send_InfoData_To_Fifo("\r\n",2);
    }
    
}











