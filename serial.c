<<<<<<< HEAD
/*************************************************
*描述：
*     单片机串口2设置
*功能：
*     通过串口2发送指令，在液晶屏上显示

************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

#define RELOAD_COUNT  0xB8        //22.1184MHz,12T,SMOD=0,9600bps//70(4800),B8(9600),DC(19200),EE(38400),F4(57600),FA(115200)

/*************************
*   串口1初始化
**************************/
/*void serial_port_one_initial()
{
    SCON = 0x50;              //0101,0000 8位可变波特率，无奇偶校验位
    BRT = RELOAD_COUNT;       //BRTR = 1, S1BRS = 1, EXTRAM = 1 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    ES = 1;                   //允许串口中断
    EA = 1;                   //开总中断
}*/

/*************************
*   串口2初始化
**************************/
void serial_port_two_initial()
{
    S2CON = 0x50;             //0101,0000 8位可变波特率，无奇偶校验位,允许接收
    BRT = RELOAD_COUNT;       // BRTR = 1, S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    IE2 = 0x01;               //允许串口2中断,ES2=1
    EA = 1;                   //开总中断
}

/**************************
*   串口1中断
***************************/
/*void UART_one_Interrupt_Receive(void) interrupt 4
{
    if(TI)
    {
      return;
	}  
}*/

/**************************
*   串口2中断
***************************/
void UART_two_Interrupt_Receive(void) interrupt 8
{
    uchar data k = 0;
    k = S2CON ;
    k = k&0x10;              //S2TI
    if(k == 1)       
    {
     return;    
    }
}

=======
/*************************************************
*描述：
*     单片机串口2设置
*功能：
*     通过串口2发送指令，在液晶屏上显示

************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

#define RELOAD_COUNT  0xfa        //22.1184MHz,12T,SMOD=0,115200bps

/*************************
*   串口1初始化
**************************/
/*void serial_port_one_initial()
{
    SCON = 0x50;              //0101,0000 8位可变波特率，无奇偶校验位
    BRT = RELOAD_COUNT;       //BRTR = 1, S1BRS = 1, EXTRAM = 1 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    ES = 1;                   //允许串口中断
    EA = 1;                   //开总中断
}*/

/*************************
*   串口2初始化
**************************/
void serial_port_two_initial()
{
    S2CON = 0x50;             //0101,0000 8位可变波特率，无奇偶校验位,允许接收
    BRT = RELOAD_COUNT;       // BRTR = 1, S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    IE2 = 0x01;               //允许串口2中断,ES2=1
    EA = 1;                   //开总中断
}

/**************************
*   串口1中断
***************************/
/*void UART_one_Interrupt_Receive(void) interrupt 4
{
    if(TI)
    {
      return;
	}  
}*/

/**************************
*   串口2中断
***************************/
void UART_two_Interrupt_Receive(void) interrupt 8
{
    uchar data k = 0;
    k = S2CON ;
    k = k&0x10;              //S2TI
    if(k == 1)       
    {
     return;    
    }
}

>>>>>>> origin/master
