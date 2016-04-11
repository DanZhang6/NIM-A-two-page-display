/*************************************************
*������
*    ������������
*���ܣ�
*    1.ѡ��̽ͷ��������ʼ��8253����������ʱ����
*    2.��ʾʱ���̽ͷ���ã��ȴ���ʱ����
*    3.��ʱ����ˢ����Ļ����ʾ�����ʣ�

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"	 

void ShowSetResult();

void measure()
{   
	 uchar j;
     Signal1=Var_Signal1;                   //ѡ��̽ͷ
     Signal2=Var_Signal2;
     Signal3=Var_Signal3; 
     ShowSetResult(); 					   //��ʾ��ǰ�������
     if((Flag_ParaChange == 1)||(Flag_InputChange == 1))   //̽ͷ��������޸�
     {
	     Flag_ParaChange = 0;
   	     Flag_InputChange = 0;
	  /*������Ĳ����洢��AT24C16*/
	     if(Incinput<9)	 
            dt_in[330]=Incinput;			//IIC�洢����(̽ͷ������־)
	     else								//ע�⵽����ǿ��̽ͷ��������С��9
		 {
	  	    Incinput=8;
			dt_in[330]=Incinput;
		 }
	     DS_SaveData(dt_in);                 //�ѵ���ϵ�������ݱ��浽AT24C16оƬ�� 
     }
	 jishucount=0;
	 for(j=0;j<8;j++)						//��������������ƽ��ƽ��ֵ������
	 {
	 	save0[j]=0;
		save1[j]=0;
		save2[j]=0;
	 }
	 Init_8253();                             //8253��ʼ��     
}

void ShowSetResult()
{
  Flag_RefrInput=0;
  Flag_RefrPara=0;
  Flag_RefrRight=0;
  Flag_RefrTime=0;
  Clear();										//������
  Lcd_Clear();
  if(measure_flag==1)
  {
     Txtext(152,148,"̽ͷ���ƣ�");
     Tnumber(392,148,Incinput);
     Txtext(440,148,"��");
     Txtext(152,216,"����ʱ�䣺");
     Tnumber(392,216,Redrawtime);
     Txtext(480,216,"��");
     Txtext(152,284,"���в�����..."); 
  }
  else
  {
     Txtext(152,148,"�궨��������");
     Tnumber(450,148,biaoding_input);
     Txtext(484,148,"��̽ͷ");
     Txtext(152,216,"����ʱ�䣺");
     Tnumber(392,216,Redrawtime);
     Txtext(480,216,"��");
     Txtext(152,284,"���б궨������..."); 
  } 
 }
