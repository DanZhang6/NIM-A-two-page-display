<<<<<<< HEAD
/*************************************************
*������
*      ʱ������
*���ܣ�
*      �趨����������ʱ�䣻

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

uchar Inctime;						//ʱ�䰴�����ӱ�־
uchar Redrawtime;					//ˢ��ʱ���־��Ĭ��Ϊ60s����λ�Ƕ����룬ÿ��ʱ���ж�5ms��һ���ж�200��

void settime()
 {	     
      Flag_RefrInput=0;				//̽ͷ���������ٴΰ��±�־
      Flag_RefrPara=0;				//������������ٴΰ��±�־
      Flag_RefrRight=0;				//���ư����ٴΰ��±�־
	  Inctime++;                    //��ʱ��������������ʱ��ı�5s����60s
	  /*��ʾ����ʱ��  ��*/  
 	  if(Flag_RefrTime==0)
     {
          Flag_RefrTime=1;
	      Clear();
	      Lcd_Clear();	
		  Txtext(128,216,"ʱ�����ã�");
	      Tnumber(368,216,0);		//ʱ���޸ĵ�5�룬�޸�ʱ��2012.6.15
	      Tnumber(416,216,0);
		  Tnumber(464,216,5);
	      Txtext(512,216,"��");
		  Redrawtime=5;
	  }
     else
	 { 
		 if((Inctime == 1)||(Inctime >= 11))	   //�޸�ʱ��2012.6.15
			{   
			  Inctime = 1;
			  Redrawtime = 5;
	          Tnumber(368,216,0);
	          Tnumber(416,216,0);
		      Tnumber(464,216,5);          //ˢ��ʱ��Ϊ5��  
			  }
	     else if(Inctime == 2)
			 {  
			  Redrawtime = 10;
			  Tnumber(368,216,0);
	          Tnumber(416,216,1);
		      Tnumber(464,216,0);
			 }
	     else if(Inctime == 3)
			 {  
			  Redrawtime = 20;
			  Tnumber(368,216,0);
	          Tnumber(416,216,2);
		      Tnumber(464,216,0);
			 }
	     else if(Inctime == 4)
			 {  
			  Redrawtime = 30;
			  Tnumber(368,216,0);
	          Tnumber(416,216,3);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 5)
			 {  
			  Redrawtime = 40;
			  Tnumber(368,216,0);
	          Tnumber(416,216,4);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 6)
			 {  
			  Redrawtime = 50;
			  Tnumber(368,216,0);
	          Tnumber(416,216,5);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 7)
			 {  
			  Redrawtime = 60;
			  Tnumber(368,216,0);
	          Tnumber(416,216,6);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 8)
			 {  
			  Redrawtime = 120;
			  Tnumber(368,216,1);
	          Tnumber(416,216,2);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 9)
			 {  
			  Redrawtime = 240;
			  Tnumber(368,216,2);
	          Tnumber(416,216,4);
		      Tnumber(464,216,0);
			 }
        else if(Inctime == 10)
			 {  
			  Redrawtime = 480;
			  Tnumber(368,216,4);
	          Tnumber(416,216,8);
		      Tnumber(464,216,0);
			 }
	   

      } 
=======
/*************************************************
*������
*      ʱ������
*���ܣ�
*      �趨����������ʱ�䣻

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

uchar Inctime;						//ʱ�䰴�����ӱ�־
uchar Redrawtime;					//ˢ��ʱ���־��Ĭ��Ϊ60s����λ�Ƕ����룬ÿ��ʱ���ж�5ms��һ���ж�200��

void settime()
 {	     
      Flag_RefrInput=0;				//̽ͷ���������ٴΰ��±�־
      Flag_RefrPara=0;				//������������ٴΰ��±�־
      Flag_RefrRight=0;				//���ư����ٴΰ��±�־
	  Inctime++;                    //��ʱ��������������ʱ��ı�5s����60s
	  /*��ʾ����ʱ��  ��*/  
 	  if(Flag_RefrTime==0)
     {
          Flag_RefrTime=1;
	      Clear();
	      Lcd_Clear();	
		  Txtext(128,216,"ʱ�����ã�");
	      Tnumber(368,216,0);		//ʱ���޸ĵ�5�룬�޸�ʱ��2012.6.15
	      Tnumber(416,216,0);
		  Tnumber(464,216,5);
	      Txtext(512,216,"��");
		  Redrawtime=5;
	  }
     else
	 { 
		 if((Inctime == 1)||(Inctime >= 11))	   //�޸�ʱ��2012.6.15
			{   
			  Inctime = 1;
			  Redrawtime = 5;
	          Tnumber(368,216,0);
	          Tnumber(416,216,0);
		      Tnumber(464,216,5);          //ˢ��ʱ��Ϊ5��  
			  }
	     else if(Inctime == 2)
			 {  
			  Redrawtime = 10;
			  Tnumber(368,216,0);
	          Tnumber(416,216,1);
		      Tnumber(464,216,0);
			 }
	     else if(Inctime == 3)
			 {  
			  Redrawtime = 20;
			  Tnumber(368,216,0);
	          Tnumber(416,216,2);
		      Tnumber(464,216,0);
			 }
	     else if(Inctime == 4)
			 {  
			  Redrawtime = 30;
			  Tnumber(368,216,0);
	          Tnumber(416,216,3);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 5)
			 {  
			  Redrawtime = 40;
			  Tnumber(368,216,0);
	          Tnumber(416,216,4);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 6)
			 {  
			  Redrawtime = 50;
			  Tnumber(368,216,0);
	          Tnumber(416,216,5);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 7)
			 {  
			  Redrawtime = 60;
			  Tnumber(368,216,0);
	          Tnumber(416,216,6);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 8)
			 {  
			  Redrawtime = 120;
			  Tnumber(368,216,1);
	          Tnumber(416,216,2);
		      Tnumber(464,216,0);
			 }
		else if(Inctime == 9)
			 {  
			  Redrawtime = 240;
			  Tnumber(368,216,2);
	          Tnumber(416,216,4);
		      Tnumber(464,216,0);
			 }
        else if(Inctime == 10)
			 {  
			  Redrawtime = 480;
			  Tnumber(368,216,4);
	          Tnumber(416,216,8);
		      Tnumber(464,216,0);
			 }
	   

      } 
>>>>>>> origin/master
}	  