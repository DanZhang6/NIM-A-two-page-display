<<<<<<< HEAD
/*************************************************
*������
*      ̽ͷ����
*���ܣ�
*      �����ڼ䣬���˼���ʾ��ǰ̽ͷ����
*      �������������˼�����̽ͷ����
*      ѡ��Χ 1-8

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

/*̽ͷ��������*/
void setinput()
{   
    Flag_RefrPara=0;					//������������ٴΰ��±�־
    Flag_RefrRight=0;					//�����ٴΰ��±�־
    Flag_RefrTime=0;					//����ʱ���ٴΰ��±�־
	Incinput++; 						//̽ͷ������־
    if(Incinput>16)						//������15��̽ͷ
	{
	 	Incinput=1;
	}
    /*��ʾ���õ�̽ͷ������������Ϊ10��*/
   	if(Flag_RefrInput==0)
    {
		Flag_RefrInput=1; 
		Clear();							//������
		Lcd_Clear();						//LCD����
		if(Incinput<9)
		{
        	Txtext(104,216,"���ò���̽ͷ���ƣ�"); 
        	Tnumber(522,216,Incinput);
        	Txtext(542,216,"��"); 		     //"��"
		}
		else
	    {
		    measure_flag=0;
		 	Clear();
		 	Lcd_Clear();
		  	biaoding_input=Incinput-8;
		  	Txtext(14,216,"��ӦҪ�궨��̽ͷ����"); 
          	Tnumber(514,216,biaoding_input);
          	Txtext(544,216,"��"); 
		  	Redrawtime=5;
		
		}
	}
	  else
	  {
	  	if(Incinput<9)
	 	{
		   measure_flag=1;
		   Clear();
		   Lcd_Clear();
		   Txtext(104,216,"���ò���̽ͷ���ƣ�"); 
           Tnumber(522,216,Incinput);
           Txtext(542,216,"��"); 
	  	} 
		else if(Incinput>8) 
	 	 {
		  	measure_flag=0;
		  	Clear();
		 	Lcd_Clear();
		  	biaoding_input=Incinput-8;
		  	Txtext(14,216,"��ӦҪ�궨��̽ͷ����"); 
          	Tnumber(514,216,biaoding_input);
          	Txtext(544,216,"��");
		  	Redrawtime=5;
		 }
		}
=======
/*************************************************
*������
*      ̽ͷ����
*���ܣ�
*      �����ڼ䣬���˼���ʾ��ǰ̽ͷ����
*      �������������˼�����̽ͷ����
*      ѡ��Χ 1-8

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

/*̽ͷ��������*/
void setinput()
{   
    Flag_RefrPara=0;					//������������ٴΰ��±�־
    Flag_RefrRight=0;					//�����ٴΰ��±�־
    Flag_RefrTime=0;					//����ʱ���ٴΰ��±�־
	Incinput++; 						//̽ͷ������־
    if(Incinput>16)						//������15��̽ͷ
	{
	 	Incinput=1;
	}
    /*��ʾ���õ�̽ͷ������������Ϊ10��*/
   	if(Flag_RefrInput==0)
    {
		Flag_RefrInput=1; 
		Clear();							//������
		Lcd_Clear();						//LCD����
		if(Incinput<9)
		{
        	Txtext(104,216,"���ò���̽ͷ���ƣ�"); 
        	Tnumber(522,216,Incinput);
        	Txtext(542,216,"��"); 		     //"��"
		}
		else
	    {
		    measure_flag=0;
		 	Clear();
		 	Lcd_Clear();
		  	biaoding_input=Incinput-8;
		  	Txtext(14,216,"��ӦҪ�궨��̽ͷ����"); 
          	Tnumber(514,216,biaoding_input);
          	Txtext(544,216,"��"); 
		  	Redrawtime=5;
		
		}
	}
	  else
	  {
	  	if(Incinput<9)
	 	{
		   measure_flag=1;
		   Clear();
		   Lcd_Clear();
		   Txtext(104,216,"���ò���̽ͷ���ƣ�"); 
           Tnumber(522,216,Incinput);
           Txtext(542,216,"��"); 
	  	} 
		else if(Incinput>8) 
	 	 {
		  	measure_flag=0;
		  	Clear();
		 	Lcd_Clear();
		  	biaoding_input=Incinput-8;
		  	Txtext(14,216,"��ӦҪ�궨��̽ͷ����"); 
          	Tnumber(514,216,biaoding_input);
          	Txtext(544,216,"��");
		  	Redrawtime=5;
		 }
		}
>>>>>>> origin/master
}