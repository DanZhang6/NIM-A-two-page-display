<<<<<<< HEAD
/*************************************************
*������
*     ��Ƭ������2����
*���ܣ�
*     ͨ������2����ָ���Һ��������ʾ

************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

#define RELOAD_COUNT  0xB8        //22.1184MHz,12T,SMOD=0,9600bps//70(4800),B8(9600),DC(19200),EE(38400),F4(57600),FA(115200)

/*************************
*   ����1��ʼ��
**************************/
/*void serial_port_one_initial()
{
    SCON = 0x50;              //0101,0000 8λ�ɱ䲨���ʣ�����żУ��λ
    BRT = RELOAD_COUNT;       //BRTR = 1, S1BRS = 1, EXTRAM = 1 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    ES = 1;                   //�������ж�
    EA = 1;                   //�����ж�
}*/

/*************************
*   ����2��ʼ��
**************************/
void serial_port_two_initial()
{
    S2CON = 0x50;             //0101,0000 8λ�ɱ䲨���ʣ�����żУ��λ,�������
    BRT = RELOAD_COUNT;       // BRTR = 1, S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    IE2 = 0x01;               //������2�ж�,ES2=1
    EA = 1;                   //�����ж�
}

/**************************
*   ����1�ж�
***************************/
/*void UART_one_Interrupt_Receive(void) interrupt 4
{
    if(TI)
    {
      return;
	}  
}*/

/**************************
*   ����2�ж�
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
*������
*     ��Ƭ������2����
*���ܣ�
*     ͨ������2����ָ���Һ��������ʾ

************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

#define RELOAD_COUNT  0xfa        //22.1184MHz,12T,SMOD=0,115200bps

/*************************
*   ����1��ʼ��
**************************/
/*void serial_port_one_initial()
{
    SCON = 0x50;              //0101,0000 8λ�ɱ䲨���ʣ�����żУ��λ
    BRT = RELOAD_COUNT;       //BRTR = 1, S1BRS = 1, EXTRAM = 1 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    ES = 1;                   //�������ж�
    EA = 1;                   //�����ж�
}*/

/*************************
*   ����2��ʼ��
**************************/
void serial_port_two_initial()
{
    S2CON = 0x50;             //0101,0000 8λ�ɱ䲨���ʣ�����żУ��λ,�������
    BRT = RELOAD_COUNT;       // BRTR = 1, S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
    AUXR = 0x15;              // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
    IE2 = 0x01;               //������2�ж�,ES2=1
    EA = 1;                   //�����ж�
}

/**************************
*   ����1�ж�
***************************/
/*void UART_one_Interrupt_Receive(void) interrupt 4
{
    if(TI)
    {
      return;
	}  
}*/

/**************************
*   ����2�ж�
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
