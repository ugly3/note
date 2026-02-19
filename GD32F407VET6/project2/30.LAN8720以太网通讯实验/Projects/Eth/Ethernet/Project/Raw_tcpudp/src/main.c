/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "gd32f4xx.h"
#include "netconf.h"
#include "main.h"
#include "lwip/tcp.h"
#include "lwip/lwip_timers.h"
#include "hello_gigadevice.h"
#include "udp_echo.h"
#include "tcp_client.h"

#define SYSTEMTICK_PERIOD_MS  10

__IO uint32_t g_localtime = 0; /* for creating a time reference incremented by 10ms */
uint32_t g_timedelay;
extern struct tcp_pcb *tcp_active_pcbs;
/***********************************************************************
函数名称：Check_TCP_Connect(void)
功    能：检查连接 
***********************************************************************/
struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;

	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
		if(cpcb -> state == ESTABLISHED)  //如果得到应答，则证明已经连接上
		{
			break;							   	
		}
	}
	return cpcb;	
}
unsigned char tcp_data[] = "tcp 客户端实验!\n";
struct tcp_pcb *pcb;
int main(void)
{
 
    /* setup ethernet system(GPIOs, clocks, MAC, DMA, systick) */ 
    enet_system_setup();
    /* initilaize the LwIP stack */
    lwip_stack_init();
     tcp_client_init();//tcp连接，对方端口号是8888，IP地址是192.168.1.11
    while(1){
			delay_10ms(50);
				pcb = Check_TCP_Connect();//查询是否连接
		   if(pcb!=0)
			 {
				 tcp_write(pcb, tcp_data,sizeof(tcp_data), 1);//发送数据    
			 }
#ifndef USE_ENET_INTERRUPT
        /* check if any packet received */
        if(enet_rxframe_size_get()){ 
            /* process received ethernet packet */
            lwip_pkt_handle();
        }
#endif /* USE_ENET_INTERRUPT */

        lwip_periodic_handle(g_localtime);

    }
}

/*!
    \brief      after the netif is fully configured, it will be called to initialize the function of telnet, client and udp
    \param[in]  netif: the struct used for lwIP network interface
    \param[out] none
    \retval     none
*/
void lwip_netif_status_callback(struct netif *netif)
{
    if((netif->flags & NETIF_FLAG_UP) != 0){
        /* initilaize the helloGigadevice module telnet 8000 */
        hello_gigadevice_init();
        /* initilaize the tcp client: echo 10260 */
        tcp_client_init();
        /* initilaize the udp: echo 1025 */
        udp_echo_init();
    }
}

/*!
    \brief      insert a delay time
    \param[in]  ncount: number of 10ms periods to wait for
    \param[out] none
    \retval     none
*/
void delay_10ms(uint32_t ncount)
{
    /* capture the current local time */
    g_timedelay = g_localtime + ncount;  

    /* wait until the desired delay finish */  
    while(g_timedelay > g_localtime){
    }
}

/*!
    \brief      updates the system local time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void time_update(void)
{
    g_localtime += SYSTEMTICK_PERIOD_MS;
}


int fputc(int ch, FILE *f)
{
    return ch;
}