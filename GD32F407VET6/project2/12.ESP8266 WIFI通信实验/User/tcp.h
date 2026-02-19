#ifndef __TCP_H
#define __TCP_H 			   
#include "gd32f4xx.h"

/*
*以下参数需要用户自行修改才能测试用过
*/

#define User_ESP8266_SSID     "CMCC-DAX7"          //wifi名
#define User_ESP8266_PWD      "13588084246"      //wifi密码

#define User_ESP8266_TCPServer_IP     "192.168.10.9"     //服务器IP
#define User_ESP8266_TCPServer_PORT   "8888"      //服务器端口号


extern volatile uint8_t TcpClosedFlag;  //连接状态标志

void ESP8266_STA_TCPClient_Test(void);

#endif
