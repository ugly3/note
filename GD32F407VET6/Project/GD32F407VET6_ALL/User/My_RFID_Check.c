#include "My_RFID_Check.h"
#include "oled.h"
#include "Key.h"   
#include "Buzz.h"
#include "RC522.h"
#include "systick.h"
#include "stdio.h"
#include "Task_Cycle.h"


int cardnumber=-1,tempcard;
char sto[20];

//读卡函数，读卡并获取卡编号
void RFID_Check(void)									
{
	cardnumber = Read_Card();	//获取卡编号
	if(cardnumber == 0)			//如果为0，表示“卡片错误”，系统中没有这张卡
	{
        cardnumber=-1;
        OLED_Clear();
		OLED_ShowCHinese(34,2,35);//卡
        OLED_ShowCHinese(51,2,36);//片
        OLED_ShowCHinese(68,2,49);//错
        OLED_ShowCHinese(85,2,50);//误
        Buzze_roll(3);		//蜂鸣器发出警报
        delay_1ms(1500);
        Set_Buzze(0);       //蜂鸣器关闭
        WaitCardOff();		//等待卡片移开
        OLED_Clear();
	}
	else if(cardnumber==1||cardnumber==2||cardnumber==3||cardnumber == 4)			//如果卡编号为1-4，说明是系统中的4张卡
	{	
        OLED_Clear();
		OLED_ShowCHinese(34,2,51);//解
		OLED_ShowCHinese(51,2,52);//锁
        OLED_ShowCHinese(68,2,53);//成
        OLED_ShowCHinese(85,2,54);//功
        
        Buzze_roll(1);
        door_status=1; 
        task2_is_start=1;
        delay_1ms(200);
//        WaitCardOff();		//等待卡片移开
        OLED_Clear();
	}	
}

 

void RFID_ADD(void)	        //录入卡片
{
//    uint8_t i;
//    uint8_t str[30];

	uint8_t status;
	uint8_t s=0x00;//地址快
	//卡类型
	unsigned char CT[2];
	//卡号
	unsigned char SN[4];
	//存放RFID 
	unsigned char RFID[5];			
	uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    /*尋卡*/
	status = PcdRequest(PICC_REQALL,CT);
	//尋卡成功
	if(status==MI_OK)
	{            
			delay_1ms(200);
			status=MI_ERR;
			/*防冲撞*/
			status = PcdAnticoll(SN);
	}
	//防衝撞成功
	if (status==MI_OK)
	{   
			status=MI_ERR;	
			status =PcdSelect(SN);

	}
    //选卡成功
	if(status==MI_OK)
	{
		status=MI_ERR;
		status =PcdAuthState(0x60,s,KEY,SN);
        //验证成功
		if(status==MI_OK)		
		{
			status=MI_ERR;
			//读卡
			status=	PcdRead(s,RFID);
			//讀卡成功
            
            if(status==MI_OK)
            {
                if(UI0[0] == 0xFF && UI0[1] == 0xFF && UI0[2] == 0xFF && UI0[3] == 0xFF)      tempcard = 0;	//判断系统各卡数据区是否为空，为空才能写入新卡
                else if(UI1[0] == 0xFF && UI1[1] == 0xFF && UI1[2] == 0xFF && UI1[3] == 0xFF) tempcard = 1;
                else if(UI2[0] == 0xFF && UI2[1] == 0xFF && UI2[2] == 0xFF && UI2[3] == 0xFF) tempcard = 2;
                else if(UI3[0] == 0xFF && UI3[1] == 0xFF && UI3[2] == 0xFF && UI3[3] == 0xFF) tempcard = 3;
                else tempcard = 4;
            
                if(RFID[0]==UI0[0] && RFID[1]==UI0[1] && RFID[2]==UI0[2] && RFID[3]==UI0[3])	//判断新卡是否已经录入
                {
                    tempcard = 5;
                }
                if(RFID[0]==UI1[0] && RFID[1]==UI1[1] && RFID[2]==UI1[2] && RFID[3]==UI1[3])
                {
                    tempcard = 5;
                }
                if(RFID[0]==UI2[0] && RFID[1]==UI2[1] && RFID[2]==UI2[2] && RFID[3]==UI2[3])
                {
                    tempcard = 5;
                }
                if(RFID[0]==UI3[0] && RFID[1]==UI3[1] && RFID[2]==UI3[2] && RFID[3]==UI3[3])
                {
                    tempcard = 5;
                }
            
            switch(tempcard)
            {
                case 0:
                    UI0[0] = RFID[0];	//将新卡数据写入UI0[]数组
                    UI0[1] = RFID[1];
                    UI0[2] = RFID[2];
                    UI0[3] = RFID[3];
//                    FLASH_W(FLASH_ADDR1,UI0[0],UI0[1],UI0[2],UI0[3]);	//将新卡数据存入flash
                    OLED_ShowCHinese(17,2,35);//卡
                    OLED_ShowCHinese(34,2,42);//一
                    OLED_ShowCHinese(51,2,33);//录
                    OLED_ShowCHinese(68,2,34);//入
                    OLED_ShowCHinese(85,2,53);//成
                    OLED_ShowCHinese(102,2,54);//功
                
                    Buzze_roll(1);//写卡成功，蜂鸣器响一声
//                    WaitCardOff();	//等待卡片移走
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();
//                    for(int i =0;i<4;i++)
//                        printf("%x",UI0[i]);
//                        printf("\r\n");
                    break;
                case 1:
                    UI1[0] = RFID[0];
                    UI1[1] = RFID[1];
                    UI1[2] = RFID[2];
                    UI1[3] = RFID[3];
//                    FLASH_W(FLASH_ADDR2,UI1[0],UI1[1],UI1[2],UI1[3]);
                    OLED_ShowCHinese(17,2,35);//卡
                    OLED_ShowCHinese(34,2,55);//二
                    OLED_ShowCHinese(51,2,33);//录
                    OLED_ShowCHinese(68,2,34);//入
                    OLED_ShowCHinese(85,2,53);//成
                    OLED_ShowCHinese(102,2,54);//功
                
                    Buzze_roll(1);
//                    WaitCardOff();
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();

//                    for(int i =0;i<4;i++)
//                        printf("%x",UI1[i]);
//                        printf("\r\n");
                    break;
                case 2:
                    UI2[0] = RFID[0];
                    UI2[1] = RFID[1];
                    UI2[2] = RFID[2];
                    UI2[3] = RFID[3];
//                    FLASH_W(FLASH_ADDR3,UI2[0],UI2[1],UI2[2],UI2[3]);
                    OLED_ShowCHinese(17,2,35);//卡
                    OLED_ShowCHinese(34,2,56);//三
                    OLED_ShowCHinese(51,2,33);//录
                    OLED_ShowCHinese(68,2,34);//入
                    OLED_ShowCHinese(85,2,53);//成
                    OLED_ShowCHinese(102,2,54);//功
                    
                    Buzze_roll(1);
//                    WaitCardOff();
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();

//                    for(int i =0;i<4;i++)
//                        printf("%x",UI2[i]);
//                        printf("\r\n");
                    break;
                case 3:
                    UI3[0] = RFID[0];
                    UI3[1] = RFID[1];
                    UI3[2] = RFID[2];
                    UI3[3] = RFID[3];
//                    FLASH_W(FLASH_ADDR4,UI3[0],UI3[1],UI3[2],UI3[3]);
                    OLED_ShowCHinese(17,2,35);//卡
                    OLED_ShowCHinese(34,2,57);//四
                    OLED_ShowCHinese(51,2,33);//录
                    OLED_ShowCHinese(68,2,34);//入
                    OLED_ShowCHinese(85,2,53);//成
                    OLED_ShowCHinese(102,2,54);//功
                    Buzze_roll(1);
//                    WaitCardOff();
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();

//                    for(i =0;i<4;i++)
//                        printf("%x",UI3[i]);
//                        printf("\r\n");
                    break;
                case 4:
                    OLED_ShowCHinese(17,2,58);//只
                    OLED_ShowCHinese(34,2,59);//能
                    OLED_ShowCHinese(51,2,33);//录
                    OLED_ShowCHinese(68,2,34);//入
                    OLED_ShowCHinese(85,2,57);//四
                    OLED_ShowCHinese(102,2,60);//张
                    OLED_ShowCHinese(119,2,35);//卡
                    Buzze_roll(3);
//                    WaitCardOff();
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();
                    break;
                case 5:
//                    OLED_ShowCHinese(20,6,"卡片已录过");
                    OLED_ShowCHinese(17,2,35);//卡
                    OLED_ShowCHinese(34,2,36);//片
                    OLED_ShowCHinese(51,2,61);//已
                    OLED_ShowCHinese(68,2,33);//录
                    OLED_ShowCHinese(85,2,62);//过
                    Buzze_roll(3);
                    WaitCardOff();
                    delay_1ms(1500);
                    task9_1_is_start=0;
                    task9_2_is_start=0;
                    OLED_Clear();
                    break;
                default:
                    break;
              }
           }
        }
        
    }
    else
    {
        PcdReset();												//复位RC522
        PcdAntennaOff(); 	 									//关天线 		
        PcdAntennaOn();  										//开天线
    }
}

void RFID_DELETE(void)	        //删除卡片
{
    switch (select_card)
    {      
        case 1:
        {   
            if(1==OLED_CONFIRM)	//在删卡模式下按下3键，删除对应的卡片
            {
                OLED_Clear();
//                FLASH_Clear(FLASH_ADDR1);	
                UI0[0]=0xFF;
                UI0[1]=0xFF;
                UI0[2]=0xFF;
                UI0[3]=0xFF;
                
                OLED_ShowCHinese(17,2,35); //卡
                OLED_ShowCHinese(34,2,42); //一
                OLED_ShowCHinese(51,2,61); //已
                OLED_ShowCHinese(68,2,37); //删
                OLED_ShowCHinese(85,2,38); //除
                
                select_card=0;
                task9_1_is_start=0;
                task9_2_is_start=0;
                OLED_CONFIRM=0;
                Buzze_roll(1);	//删除成功后蜂鸣器响一声
                OLED_SELECT=OLED_SELECT_flag;
                delay_1ms(1000);
                OLED_Clear();
            }
        }break;
        case 2:
        {
            if(1==OLED_CONFIRM)
            {
                OLED_Clear();
//                FLASH_Clear(FLASH_ADDR2);
                UI1[0]=0xFF;
                UI1[1]=0xFF;
                UI1[2]=0xFF;
                UI1[3]=0xFF;

                OLED_ShowCHinese(17,2,35); //卡
                OLED_ShowCHinese(34,2,55); //二
                OLED_ShowCHinese(51,2,61); //已
                OLED_ShowCHinese(68,2,37); //删
                OLED_ShowCHinese(85,2,38); //除
                
                select_card=0;
                task9_1_is_start=0;
                task9_2_is_start=0;
                OLED_CONFIRM=0;
                Buzze_roll(1);	//删除成功后蜂鸣器响一声
                OLED_SELECT=OLED_SELECT_flag;
                delay_1ms(1000);
                OLED_Clear();
                
            }
        }break;
        case 3:
        {
            if(1==OLED_CONFIRM)
            {
                OLED_Clear();
//                FLASH_Clear(FLASH_ADDR3);
                UI2[0]=0xFF;
                UI2[1]=0xFF;
                UI2[2]=0xFF;
                UI2[3]=0xFF;

                OLED_ShowCHinese(17,2,35); //卡
                OLED_ShowCHinese(34,2,56); //三
                OLED_ShowCHinese(51,2,61); //已
                OLED_ShowCHinese(68,2,37); //删
                OLED_ShowCHinese(85,2,38); //除
                
                select_card=0;
                task9_1_is_start=0;
                task9_2_is_start=0;
                OLED_CONFIRM=0;
                Buzze_roll(1);	//删除成功后蜂鸣器响一声
                OLED_SELECT=OLED_SELECT_flag;
                delay_1ms(1000);
                OLED_Clear();
                
            }
        }break;
        case 4:
        {
            if(1==OLED_CONFIRM)
            {
                OLED_Clear();
//                FLASH_Clear(FLASH_ADDR4);
                UI3[0]=0xFF;
                UI3[1]=0xFF;
                UI3[2]=0xFF;
                UI3[3]=0xFF;

                OLED_ShowCHinese(17,2,35); //卡
                OLED_ShowCHinese(34,2,57); //四
                OLED_ShowCHinese(51,2,61); //已
                OLED_ShowCHinese(68,2,37); //删
                OLED_ShowCHinese(85,2,38); //除
                
                select_card=0;
                task9_1_is_start=0;
                task9_2_is_start=0;
                OLED_CONFIRM=0;
                Buzze_roll(1);	//删除成功后蜂鸣器响一声
                OLED_SELECT=OLED_SELECT_flag;
                delay_1ms(1000);
                OLED_Clear();
                
            }
        }break;
        default:break;
    }
}

