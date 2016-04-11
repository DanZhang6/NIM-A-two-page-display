/********************************************************************
*������
*     ����������ʾ�ӻ����ݱ���
*���ܣ�
*     LEDָʾ�ƣ���Ļ�ϣ���˸������ֱ���´����ݸ��£��������жϵ���˸

*********************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"
#include "stdio.h"
#include "math.h"

uchar receive_buf[57];                      //���մӻ�����
uchar Flag_need_warn[7];                   //��Ҫ������־����ʼ0
uchar State_On_Off[7];                     //ָʾ��״̬��־.��ʼ0

void Warning_Disp(uchar Num,uchar Flag,uchar State);
void Updata_Led(uchar j);

/*************************************
*         ��ʾ�ӻ�������   
*************************************/
void display_b()
{    
    uchar idata j;
    Lcd_Clear();
	for(j=0;j<receive_buf[56];j++)
	{
	   /********��������ʾ**********/ 
	    Txtext(60,42+(j*58),"̽ͷ");
		Tnumber(160,42+(j*58),j+9);
		Txtext(210,42+(j*58),":");
		if(receive_buf[8*j] != 0)
		{
			Tnumber(230,42+(j*58),receive_buf[8*j]); 
			Tnumber(270,42+(j*58),receive_buf[8*j+1]);
			Tnumber(310,42+(j*58),receive_buf[8*j+2]);
			Txtext(350,42+(j*58),".");
			Tnumber(390,42+(j*58),receive_buf[8*j+3]);
			Tnumber(430,42+(j*58),receive_buf[8*j+4]);
			Tnumber(470,42+(j*58),receive_buf[8*j+5]);
 		 }
		 else
		 {
		 	if(receive_buf[8*j+1] != 0)
			{
				Tnumber(270,42+(j*58),receive_buf[8*j+1]);
				Tnumber(310,42+(j*58),receive_buf[8*j+2]);
				Txtext(350,42+(j*58),".");
				Tnumber(390,42+(j*58),receive_buf[8*j+3]);
				Tnumber(430,42+(j*58),receive_buf[8*j+4]);
				Tnumber(470,42+(j*58),receive_buf[8*j+5]);
			}
			else
			{
				Tnumber(310,42+(j*58),receive_buf[8*j+2]);
				Txtext(350,42+(j*58),".");
				Tnumber(390,42+(j*58),receive_buf[8*j+3]);
				Tnumber(430,42+(j*58),receive_buf[8*j+4]);
				Tnumber(470,42+(j*58),receive_buf[8*j+5]);
			}
		 }
	   /**********������λ��ʾ***********/
		if(receive_buf[8*j+6]==1)
		{   
			Txtext(510,42+(j*58),"Bq/L");
		}
		else if(receive_buf[8*j+6]==2)
		{
			Txtext(510,42+(j*58),"KBq/L");
		} 
		else if(receive_buf[8*j+6]==3)
		{
			Txtext(510,42+(j*58),"MBq/L");
		}		
		if(receive_buf[8*j+7] == 1)
		{
		    WarnLed_On(5,42+(j*58));        // �޸�
			Flag_need_warn[j]=1;
	        State_On_Off[j]=1;
		}
		else if(receive_buf[8*j+7] == 0)
		{
		    WarnLed_Off(0,42+(j*58));       // �޸�
			Flag_need_warn[j]=0;
			State_On_Off[j]=0;
		}
	}
}

/***********************************
*          ����˸
************************************/
void RedLed_Flash(void)
{
    uchar idata i;
	for(i=0;i<receive_buf[56];i++)
	{
		Updata_Led(i);
		Warning_Disp(i,Flag_need_warn[i],State_On_Off[i]);
	}
}

/***********************************
*          ����ָʾ��״̬
************************************/
void Updata_Led(uchar j)
{
	if(Flag_need_warn[j])
	{
	    if(State_On_Off[j])
		{
			State_On_Off[j]=0;
		}
		else
		{
			State_On_Off[j]=1;
		}
	}
	else
			State_On_Off[j]=0;
}
/************************************
*          ������ʾ
*************************************/
void Warning_Disp(uchar Num,uchar Flag,uchar State)
{
	if(Flag)
	{
		if(State)
		{
			WarnLed_On(5,42+(Num*58));   // �޸�
			return;
		 }
	}
	WarnLed_Off(0,42+(Num*58));       // �޸�
}
