/**
************************************************************************
 *    文件名：xiaochuang
 *      说明：小创语音模块
 *     源代码在 bkrc_voice.c 文件中
************************************************************************
**/
#include "xiaochuang.h"

XiaoChuang_Typedef XiaoChuang_Data = 
{
	.xSend_To_XiaoChuang_Rouse = &xSend_To_XiaoChuang_Rouse,
	.xSend_Command_To_XiaoChuang = &xSend_Command_To_XiaoChuang,
	.xXiaoChuang_Analyze_Rx_Command = &xXiaoChuang_Analyze_Rx_Command,
	.xZiDongPingFen_Upload_VoiceCode = &xZiDongPingFen_Upload_VoiceCode, 
};

uint8_t XiaoChuang_be_Awakened_Arr[5] = {0xFA,0xFA,0xFA,0xFA,0xA1};   //小创被唤醒指令
uint8_t XiaoChuang_Rx_Byte = 0x00;  // 小创识别后，传回来的一个字节数据



//给小创发送唤醒词，唤醒小创，在识别前需要先发送这个函数
void xSend_To_XiaoChuang_Rouse(void)
{
	USART6_Send_Length(XiaoChuang_be_Awakened_Arr, 5);
}


/******************************************
功  能：发送指令让小创播报指定词句
参数   语音词条ID  词条内容
              0x01 -> 技能成才  55030100
			  0x02 -> 匠心筑梦  55030200
			  0x03 -> 筑梦扬威  55030300
			  0x04 -> 技行天下  55030400
			  0x05 -> 展行业百技 55030500
			  0x06 -> 树人才新规	 55030600
*******************************************/
void xSend_Command_To_XiaoChuang(uint8_t Cmd)
{
	USART6_Send_Byte(Cmd);
}

//嵌入式智能车开发单元（A/B）向自动评分系统上传语音编号控制指
void xZiDongPingFen_Upload_VoiceCode(uint8_t data)  
{
	uint8_t Temp[8] = {0};
//	uint8_t CheckSum;    //校验和
	Temp[0] = 0xAF;
	Temp[1] = 0x06;
	Temp[2] = data; 
	Temp[3] = 0x02;
	Temp[4] = 0x00;
	Temp[5] = 0x00;
	Temp[6] = 0x01;
	Temp[7] = 0xBB;
	for(uint8_t i=0;i<3;i++)
	{
		Send_ZigbeeData_To_Fifo(Temp,8);
		delay_ms(100);
	}
	delay_ms(300);
}




//小创识别语音，或者通过串口发给小创时，小创回传的命令
//通过串口发给小创命令时，小创回传的前两个字节为 0x55 0x03
//小创通过语音识别，识别后回传的前两个字节为 0x55 0x02
// 0x55 0x01 指令为（初始化完成， 进入识别模式， 退出识别模式， 进入休眠模式， 打开播放， 关闭播放）


// 注意！： 识别成功后，回传指令，在执行任务中最少需要等待 5s及以上才可以得到数据，然后再通过zigbee发送出去

/*
	for(uint8_t i = 0; i < 5; i++)
{
	delay_ms(500);
	delay_ms(500);
}
*/
void xXiaoChuang_Analyze_Rx_Command(void)
{
	 if((voice_falg == 0x01) && (UART6_RxData[0] == 0x55))			// 自定义数据帧接收完毕
	 {
		 if(UART6_RxData[1] == 0x01)    //源文件是config-开发板.ini里面
		 {
			 switch(UART6_RxData[2])
			 {
				 case 0x01:    //   welcome（初始化完成 ）
				 {
					 break;
				 }
				 case 0x02:    //  wakeword （进入识别模式）  被唤醒
				 {
					 break;
				 }
				 case 0x03:    //  bakeoff （退出识别模式）   
				 {
					 break;
				 }
				 case 0x04:    //  exitWake （,唤醒时长结束， 进入休眠模式）
				 {
					 break;
				 }
				 case 0x07:    //  打开播放
				 {
					 break;
				 }
				 case 0x08:     //  关闭播放
				 {
					 break;
				 }
				 default:break;
			 }
		 }
		 
		 else if(UART6_RxData[1] == 0x02)     // //源文件是asrWordlist.txt 里面，小创识别后回传的命令
		 {
			 switch(UART6_RxData[2])
			 {
				 case 0x01:      //识别    
				 { 
					 XiaoChuang_Rx_Byte = 0x01;
					 xZiDongPingFen_Upload_VoiceCode(0x01);
					 break;
				 }
				 case 0x02:     //识别   
				 {
					 XiaoChuang_Rx_Byte = 0x02;
					 xZiDongPingFen_Upload_VoiceCode(0x02);
					 break;
				 }
				 case 0x03:     //识别  
				 {
					 XiaoChuang_Rx_Byte = 0x03;
					 xZiDongPingFen_Upload_VoiceCode(0x03);
					 break;
				 }
				 case 0x04:      //识别  
				 {
					 XiaoChuang_Rx_Byte = 0x04;
					 xZiDongPingFen_Upload_VoiceCode(0x04);
					 break;
				 }
				 case 0x05:     //识别  
				 {
					 XiaoChuang_Rx_Byte = 0x05;
					 xZiDongPingFen_Upload_VoiceCode(0x5);
					 break;
				 }
				 case 0x06:     //识别  
				 {
					 XiaoChuang_Rx_Byte = 0x06;
					 xZiDongPingFen_Upload_VoiceCode(0x06);
					 break;
				 }
				 case 0x07:    
				 {
					 XiaoChuang_Rx_Byte = 0x07;
					 xZiDongPingFen_Upload_VoiceCode(0x07);
					 break;
				 }
				 default:break;
			 }
		 }
		 
		 else if(UART6_RxData[1] == 0x03)     // //源文件是serialTTS.txt里面，通过发送指令让小创播报，然后回传命令
		 {
			 switch(UART6_RxData[2])
			 {
				 case 0x01:   
				 {
					 break;
				 }
				 case 0x02:   
				 {
					 break;
				 }
				 case 0x03:   
				 {
					 break;
				 }
				 case 0x04:   
				 {
					 break;
				 }
				 case 0x05:   
				 {
					 break;
				 }
				 case 0x06:   
				 {
					 break;
				 }
				 default:break;
			 }
		 }
	 }
	 voice_falg = 0x00;
}

















