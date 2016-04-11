<<<<<<< HEAD
/*************************************************
*描述：
*      时间设置
*功能：
*      设定计数器测量时间；

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

uchar Inctime;						//时间按键增加标志
uchar Redrawtime;					//刷新时间标志，默认为60s，单位是多少秒，每定时器中断5ms，一秒中断200次

void settime()
 {	     
      Flag_RefrInput=0;				//探头个数按键再次按下标志
      Flag_RefrPara=0;				//输入参数按键再次按下标志
      Flag_RefrRight=0;				//右移按键再次按下标志
	  Inctime++;                    //按时间输入键，则测量时间改变5s或者60s
	  /*显示设置时间  秒*/  
 	  if(Flag_RefrTime==0)
     {
          Flag_RefrTime=1;
	      Clear();
	      Lcd_Clear();	
		  Txtext(128,216,"时间设置：");
	      Tnumber(368,216,0);		//时间修改到5秒，修改时间2012.6.15
	      Tnumber(416,216,0);
		  Tnumber(464,216,5);
	      Txtext(512,216,"秒");
		  Redrawtime=5;
	  }
     else
	 { 
		 if((Inctime == 1)||(Inctime >= 11))	   //修改时间2012.6.15
			{   
			  Inctime = 1;
			  Redrawtime = 5;
	          Tnumber(368,216,0);
	          Tnumber(416,216,0);
		      Tnumber(464,216,5);          //刷新时间为5秒  
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
*描述：
*      时间设置
*功能：
*      设定计数器测量时间；

**************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

uchar Inctime;						//时间按键增加标志
uchar Redrawtime;					//刷新时间标志，默认为60s，单位是多少秒，每定时器中断5ms，一秒中断200次

void settime()
 {	     
      Flag_RefrInput=0;				//探头个数按键再次按下标志
      Flag_RefrPara=0;				//输入参数按键再次按下标志
      Flag_RefrRight=0;				//右移按键再次按下标志
	  Inctime++;                    //按时间输入键，则测量时间改变5s或者60s
	  /*显示设置时间  秒*/  
 	  if(Flag_RefrTime==0)
     {
          Flag_RefrTime=1;
	      Clear();
	      Lcd_Clear();	
		  Txtext(128,216,"时间设置：");
	      Tnumber(368,216,0);		//时间修改到5秒，修改时间2012.6.15
	      Tnumber(416,216,0);
		  Tnumber(464,216,5);
	      Txtext(512,216,"秒");
		  Redrawtime=5;
	  }
     else
	 { 
		 if((Inctime == 1)||(Inctime >= 11))	   //修改时间2012.6.15
			{   
			  Inctime = 1;
			  Redrawtime = 5;
	          Tnumber(368,216,0);
	          Tnumber(416,216,0);
		      Tnumber(464,216,5);          //刷新时间为5秒  
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