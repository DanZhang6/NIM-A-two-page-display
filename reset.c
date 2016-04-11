/***************************************************
* ������
       ��λ����
* ���ܣ�
*      ��ʱ�䣬̽ͷ�������������÷��ص���ʼ��״̬��

****************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

void ReSet()
{
	 Clear();
	 Lcd_Clear(); 
	 Txtext(188,216,"�� �� �� ��");	
	 IE0 = 0;                                     //���ⲿ�ж�0��0    
	 ES = 0;                                      //��ֹ�����ж�
	 IE2 = 0x00;                                  //��ֹ����2�ж�
	 Incdata = YuThouth[0];
	 Incdataright = 1;								//���ư������ӱ�־?
	 Inctime = 1;                                 //ʱ�䰴�����ӱ�־
	 Incinput = 8 ;                               //Ĭ����ʾ̽ͷ����Ϊ8��
	 Redrawtime = 60;                             //Ĭ��ˢ��ʱ��Ϊ60��
	 wb = 1;
     xh = 1;
	 bz = 1;
	 Flag_RefrInput = 0;
	 Flag_RefrPara = 0;
	 Flag_RefrRight = 0;
	 Flag_RefrTime = 0;
	 EX0 = 1;
}		     
