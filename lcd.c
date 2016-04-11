/*************************************************
*������
*     LCD���Һ����ָ������
*���ܣ�
*    1. ͨ����Ƭ��������ʾ���֣����֣�
*    2. Һ����������ü���ʼ����

************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

void Txbyte(uchar idata i);
//void Txbyte_1(uchar idata i);
void Tword(uint idata i);
void Tword_1(uint idata i);
void TxEOF();
void TxEOF_1();

/**************************
*  �򴮿�2����һ���ֽ�
***************************/
void Txbyte(uchar idata i)
{
   uchar idata temp=0;
   IE2=0x00;            //�ش���2�ж�,ES2=0
//   S2CON= S2CON&0xFD;   //B'11111101,���㴮��2��������ж������־
   S2BUF=i;
   do
   {
     temp=S2CON;
     temp=temp&0x02;
    }
	while(temp==0);
    S2CON=S2CON&0xFD;   //B'11111101,���㴮��2��������ж������־
    IE2=0x01;           //������2�ж�,ES2=1
}

/**************************
*  �򴮿�2����һ����
***************************/
void Tword(uint idata i)
 {
     uchar idata x,y;
     x=(i/256);
     Txbyte(x);
     y=i-256*x;
     Txbyte(y);
 }

/****************************
*    �򴮿�2����֡������
*****************************/
void TxEOF()       //����֡������CC 33 C3 3C
{
	 Txbyte(0xCC);
	 Txbyte(0x33);
	 Txbyte(0xC3);
	 Txbyte(0x3C);
 }

/****************************
*      ����2������ʾ
******************************/ 
void Tnumber(int idata x,int idata y,uint idata n)
{
	 uchar idata a,b,c,d,e;
	 Txbyte(0xAA);
	 Txbyte(0x98);
	 Tword(x);              //x����
	 Tword(y);	            //y����
     Txbyte(0x24);
     Txbyte(0xC1);          //��ʾ����ɫ
     Txbyte(0x05);
     Txbyte(0xff);
     Txbyte(0xff);
     Txbyte(0x00);
     Txbyte(0x1f);
	 a=n/10000;	        //����ת����ASCII��
	 b=n%10000/1000;
	 c=n%10000%1000/100;
	 d=n%10000%1000%100/10;
	 e=n%10000%1000%100%10;
	 if(a!=0)
	 {
		 Txbyte(a+0x30);
		 Txbyte(b+0x30);
	     Txbyte(c+0x30);
	     Txbyte(d+0x30);
	     Txbyte(e+0x30);
	 }  
     else
	 {
		  if(b!=0)
		   {
			 Txbyte(b+0x30);
			 Txbyte(c+0x30);
		     Txbyte(d+0x30);
		     Txbyte(e+0x30);
		   } 
 		  else
		   {
		    if(c!=0)
		    {
				 Txbyte(c+0x30);
	             Txbyte(d+0x30);
			     Txbyte(e+0x30);
		     } 
			 else
			  {
			    if(d!=0)
			    {
					 Txbyte(d+0x30);
	                 Txbyte(e+0x30);
			     } 
				else
				 {
                    Txbyte(e+0x30);
				 }
			}
		}
    }
	  
	 TxEOF();	       //���ͽ�����
	 shortdelay(20);
}


/*****************************
*       ����2�ı���ʾ
******************************/
void Txtext(uint idata x,uint idata y,uchar *s)
  {
	   Txbyte(0xAA);      //֡ͷ0xAA
	   Txbyte(0x98);      //48*48����
	   Tword(x);	      //x����
	   Tword(y);	      //y����
       Txbyte(0x24);
	   Txbyte(0x81);      //����ʾ����ɫ
	   Txbyte(0x05);
	   Txbyte(0xff);
	   Txbyte(0xff);
	   Txbyte(0x00);
	   Txbyte(0x1f);
	   while(*s)	      //�����ַ�������
	    {
		  Txbyte(*s);
		  s++;
		  }
	   TxEOF();		      //����֡������
       shortdelay(20);
   }

/*******************************
*        ����2��Ļ��ʼ��
*********************************/
void Lcd_init()
{
	 Txtext(250,182,"�� �� ��");
	 Txtext(120,250,"�� �� �� ʼ �� ��"); 
 }

/*******************************
*        ����2��Ļ����
*********************************/
void Lcd_Start()
{
	 Txtext(190,182,"�� �� �� ѧ");
	 Txtext(50,250,"����������Դ�����о�Ժ"); 
 }

/*******************************
*         ����2  ����
********************************/
void Lcd_Clear()
{
	 Txbyte(0xAA);
	 Txbyte(0x52);
	 TxEOF();
 }

/*************************************
*          ����
*************************************/
void Open (uint idata x,uint idata y)
{
	Txbyte(0xAA);      //֡ͷ0xAA
	Txbyte(0x44);      //�����ʾ
    Txbyte(0x01);      //�����ʾ��
	Tword(x);	       //x����
	Tword(y);	       //y����
	Txbyte(0x1F);
	Txbyte(0x05);
    TxEOF();
}

/*************************************
*          ���ر�
*************************************/
void Close (uint idata x,uint idata y)
{
	Txbyte(0xAA);     //֡ͷ0xAA
	Txbyte(0x44);     //�����ʾ
    Txbyte(0x00);     //�����ʾ�ر�
	Tword(x);	     //x����
	Tword(y);	     //y����
	Txbyte(0x1B);
	Txbyte(0x05);
    TxEOF();
}

/************************************
*         ������
*************************************/
void Clear()
{
  Close(112+Incdataright*48-96,128+wb*68-68);
  Close(432+Incdataright*48-96,128+wb*68-68);
  Close(160+Incdataright*48-144,128+wb*68-68);
  Close(480+Incdataright*48-144,128+wb*68-68);
}


/****************************************
*          ͼƬ��ʾ
****************************************/
/*void picture()
{
    Txbyte(0xAA);       
    Txbyte(0x70);
	Txbyte(0x01);
    TxEOF();
}*/

/*****************************
*       ����2�����ʾ
******************************/
void WarnLed_On(uint idata x,uint idata y)
  {
	   Txbyte(0xAA);      
	   Txbyte(0x9C);      
       Txbyte(0x04); 
       Txbyte(0x00); 
	   Txbyte(0x00); 
       Txbyte(0x00); 
       Txbyte(0x00);
       Txbyte(0x00);  
	   Txbyte(0x28);      
	   Txbyte(0x00);
	   Txbyte(0x28);
       Tword(x);	     //x����
   	   Tword(y);	     //y����
	   TxEOF();		     //����֡������
   }


/*****************************
*       ����2��ɫ��ʾ
******************************/
void WarnLed_Off(uint idata x,uint idata y)
  {
	   Txbyte(0xAA);      
	   Txbyte(0x9C);      
       Txbyte(0x05); 
       Txbyte(0x00); 
	   Txbyte(0x00); 
       Txbyte(0x00); 
       Txbyte(0x00);
       Txbyte(0x00);  
	   Txbyte(0x30);      
	   Txbyte(0x00);
	   Txbyte(0x30);
       Tword(x);	     //x����
   	   Tword(y);	     //y����
	   TxEOF();		     //����֡������
   }

