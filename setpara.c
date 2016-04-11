/*******************************************************************
* ������
       ��������
* ���ܣ�
*     1.  �����趨�õ�̽ͷ����������ÿ��̽ͷ�Ĳ���,
*         ÿ��̽ͷ�������������ÿ�������������a��b;
*     2.  ����λ��ͨ���������Ƽ����ã�ǧ��λ���ٷ�λ��ʮ��λ����λ��
*     3.  ������ÿһλ����0��9֮��ѭ��ѡ��

********************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"

uchar Incdataright;							//���ư������´�����־
uchar Incdata;								//������С��־??????
uchar idata bz;                               //��ʾ���ò���������־
uchar idata xh;                               //����̽ͷ������־
uchar idata wb;                               //���ò���������־
uchar idata Flag_Yu=0; 
uchar idata Flag_Para=0; 
uchar data Flag_Left=0;                       //����a�еı�־
uchar data Flag_Right=0;                      //����b�еı�־
uchar data Flag_Change=0;
uchar YuThouth[8];								//��ֵǧ��λ
uchar YuCent[8];								//��ֵ�ٷ�λ
uchar YuTenth[8];								//��ֵʮ��λ
uchar YuGe[8];									//��ֵ��λ

extern void dispaly_para();
extern void dispaly_paramove();
extern void Side_RChange();
extern void Side_LChange();
extern void Line_Change();
/******************************
*         ��������
******************************/
void setparameter()
{
	Flag_RefrInput=0;
	Flag_RefrTime=0;
	Flag_RefrRight=1;
	Flag_RefrLeft=1;
    if(wb==1)						//����������־
    { 
        if(Incdataright==1)
		 {
			YuThouth[xh-1]=Incdata;			//YuThoughΪ���а˸�̽ͷ��ֵ��ǧ��λ,xhΪ����̽ͷ������־��IncdataΪ
			dt_in[bz*4-3]=YuThouth[xh-1];
		 }
		else if(Incdataright==2)
		 {
			YuCent[xh-1]=Incdata;
		    dt_in[bz*4-2]=YuCent[xh-1];
		 }
		else if(Incdataright==3)
		 {
			YuTenth[xh-1]=Incdata;
		    dt_in[bz*4-1]=YuTenth[xh-1];
		 }
		else if(Incdataright==4)
		 {
			YuGe[xh-1]=Incdata;
		    dt_in[bz*4]=YuGe[xh-1];
		 }   
    }
    else if(wb!=1)
    {
		/*���ò�����ÿһλ*/
		if(Incdataright==1)
		{
			DataThouth[bz-xh]=Incdata;
			dt_in[bz*4-3]=DataThouth[bz-xh];
		 }
		else if(Incdataright==2)
		 {
			DataCent[bz-xh]=Incdata;
		    dt_in[bz*4-2]=DataCent[bz-xh];
		 }
		else if(Incdataright==3)
		 {
			DataTenth[bz-xh]=Incdata;
		    dt_in[bz*4-1]=DataTenth[bz-xh];
		 }
		else if(Incdataright==4)
		 {
			DataGe[bz-xh]=Incdata;
		    dt_in[bz*4]=DataGe[bz-xh];
		 }
     }
 	  dispaly_para();
}
  
/********************************
*       ������������
*********************************/
void parameterright()
{
	Flag_RefrInput=0;
	Flag_RefrTime=0;
	if(Flag_RefrRight==0)
	{
		Flag_RefrPara=0;
	}
	Incdata=0;
	Incdataright++;                               //�������Ƽ������´���
    Side_RChange();									//�������ұ䶯����
	dispaly_paramove();								//�������ƹ����ʾ
}


/*******************************
*       ����λ���ұ䶯
*********************************/
void Side_RChange()
{
	if(xh%2!=0)                                  //̽ͷ��ʾΪ������̽ͷ1��3��5��7��
	 {
		if(Incdataright == 5)                    
		{    
			Incdataright = 1;
			bz++;                                //���������飬ÿ�����2�ԣ�ÿ���������1 
			if((bz-1)%11==0)                     //��ҳ����ÿ��̽ͷ��10������ϵ����
			{
				xh++;                            //ʮ�����������꣬̽ͷ��־��1
			    if(xh==8)
		        {
			        Flag_RefrPara=0;  
		        }
		        if(xh > Incinput)
		        {
				     Flag_RefrPara=0;             //ˢ������
			         xh=1;                       //̽ͷ��ʾ��־���ܳ���̽ͷ����
					 bz=1;
		        }
			}  
			
			if((xh%2)==0)                         //����̽ͷת˫��̽ͷ����
			{
				if((bz%2)==0)                     //���д���    
				{ 
					wb++;
				}
			}
            if((xh%2)!=0)                         //����̽ͷ���д���
			{
				if((((bz-1)%2)!=0)||(bz==1))                        
				{
					wb++; 
				}
			}
			if(wb==7)                           //�������ù�6�У����е�һ��Ϊ��ֵ���ã���������Ϊ����
			{
				wb=1;
				Flag_Change=1;
			} 				
		}	   
	}
	if(xh%2==0)                                 //̽ͷ��ʾΪ������̽ͷ2��4��6��8��
	{
		if(Incdataright >= 5)
		{    
			Incdataright = 1;
			bz++;                                //ǰ��̽ͷ���������꣬̽ͷ��1 
			if(bz==83)
			{
				xh=1;
				bz=1;
				wb=1;
                Flag_RefrPara=0;
                Flag_Change=1;
			}
			else
			{
				if((bz-1)%11 == 0)                 //��ҳ����ÿ��̽ͷ��10������ϵ����
				{
					xh++;                           //ʮ�����������꣬̽ͷ��־��1
					if(xh==8)
			        {
				        Flag_RefrPara=0;  
			        }
			        if(xh > Incinput)
			        {
					     Flag_RefrPara=0;           //ˢ������
				         xh=1;                     //̽ͷ��ʾ��־���ܳ���̽ͷ����
						 bz=1;
			        }
				}  
				if((xh%2)!=0)                       //˫��̽ͷת����̽ͷ����  
				{
					if((bz%2)!=0)                   
					{
						wb++; 
					}
				}
				if((xh%2)==0)                       //˫��̽ͷ���д���                          
				{
					if(((bz-1)%2)==0)                   
					{ 
						wb++;
					}
				}
				if(xh<8)
				{
					if(wb==7)
					{
						wb=1;
						Flag_Change=1;            //ˢ��̽ͷ��������
					} 
				}
			}
		}
	} 
    Line_Change();
}
/********************************
*       ������������
*********************************/
void parameterleft()                           
{
	Flag_RefrInput=0;
	Flag_RefrTime=0;
	if(Flag_RefrLeft==0)
	  {
        Flag_RefrPara=0;
	  }
  	Incdata=0;
	Incdataright--;                             //�������Ƽ������´���
    Side_LChange();								//����λ����䶯
	dispaly_paramove();							//�������ƹ����ʾ
}

/*******************************
*       ����λ����䶯
*********************************/
void Side_LChange()
{
	if(xh%2!=0)                                 //����̽ͷ��̽ͷ1��3�� 5�� 7������λ������
	 {
		if(Incdataright ==0)
		{    
			Incdataright = 4;
			bz--;                                //���������飬ÿ�����2�ԣ�ÿ���������1 
            if(bz%11==0)                         //ÿ��̽ͷ��10������ϵ������һ����ֵ���ã�������11�飬������һ̽ͷ����
			{   
			    xh--;                            //ʮ���������ú���ֵ�����꣬̽ͷ��־��1
		        if(xh<1)                         //������Ƶ���ʼλ�ã������ƾ͵���Incinput��̽ͷ���趨���̽ͷ������
		        {
				     Flag_RefrPara=0;            //ˢ�µ�ǰ��Ļ
			         xh=Incinput;                //̽ͷ��ʾ��־������趨̽ͷ����
		        }
			}                              			
			if((xh%2)!=0)                        //����̽ͷ�Ļ��д���
			{
				if(((bz%2)!=0)||(bz==0))          //bzΪ����ʱ����
				{ 
					wb--;                        //�����������ϱ�С
					if(wb==0)                    //�Ƶ����
					{
					   wb=6;
					   bz=xh*11;
                       Flag_Change=1;             //ˢ��̽ͷ����
					}
				}
			}
			if((xh%2)==0)                        //����̽ͷת����˫��̽ͷʱ�����д���
			{
				if((bz%2)==0)                    //��ֵ������ͻ���
				{ 
					wb--;
					if(wb==0)                   
					{
					   if(xh==8)                  //�ڰ˸�̽ͷ��һ����ֵ������������ã�������
					   {
					   	   wb=3;                  //�ӵ����п�ʼ����
						   bz=82;                 //���һ�������bz=82
					   }
					   else                       //����̽ͷ��һ����ֵ������������ã�������
					   {
						   wb=6;                  //�ӵ����п�ʼ����
						   bz=xh*11;
					   }
                       Flag_Change=1;             //ˢ��̽ͷ����
					}
				}
			}			
		}   
	}
	if(xh%2==0)                                   //˫��̽ͷ��̽ͷ2��4�� 6��8������λ������
	{
		if(Incdataright ==0)
		{    
			Incdataright = 4;	
           	bz--;                                //���������飬ÿ�����2�ԣ�ÿ���������1  
            if(bz%11==0)                         //ÿ��̽ͷ��10������ϵ������һ����ֵ���ã�������11�飬������һ̽ͷ����
			{
				xh--;                             
				if(xh==7)                        
		        {
			        Flag_RefrPara=0;              //��8��̽ͷ�͵�7��̽ͷ�����ݸ�����ͬ����ת��ʱҪˢ�½���
		        }
		        if(xh<1)
		        {
				     Flag_RefrPara=0; 
			         xh=Incinput;                 //̽ͷ��ʾ��1��Incinput֮��ѭ��
		        }
			}  		
			if((xh%2)!=0)                         //˫��̽ͷת����̽ͷ���д���
			{
				if((bz%2)!=0)                   
				{
					wb--; 
					if(wb==0)                  
					{
					   wb=6;
					   bz=xh*11;
                       Flag_Change=1;
					}
				}
			}
			if((xh%2)==0)                        //˫��̽ͷ���д���
			{
				if((bz%2)==0)                   
				{ 
					wb--;
					if(wb==0)                   
					{
					   wb=6;
					   bz=xh*11;
                       Flag_Change=1;
					}
				}	
			}
		}
	} 
	Line_Change();
}

/****************************************
*         ����������ת��
****************************************/
void Line_Change()
{
	if(wb!=1)
	{
	    Flag_Para=1;
		if((xh%2)!=0)
		{
			if(bz%2==0)
			{
				Flag_Left=1;
			}
			if(bz%2!=0)
			{
				Flag_Right=1;            
			}
		}
		else if((xh%2)==0)
		{
			if(bz%2!=0)
			{
				Flag_Left=1;
			}
			if(bz%2==0)
			{
				Flag_Right=1;            
			}
		}  
	}
	else if(wb==1)
	{
		Flag_Yu=1; 	   	
	}
}
/**************************************
*         �������ƹ����ʾ 
*************************************/
void dispaly_paramove()
{
 /*���ƹ����ʾ*/
    if(Flag_RefrRight==1)
     {
        if(Flag_Para==1)
		{
		    Flag_Para=0;
		    if(Flag_Left==1)
		    {
				Flag_Left=0;
				if(Incdataright==1)
				   {
					   Open(112+Incdataright*48-48,128+wb*68-68);		
				   }
				else if(Incdataright==2)
				   {
					   Close(112+Incdataright*48-96,128+wb*68-68);
					   Open(160+Incdataright*48-96,128+wb*68-68);
				   }	
				else if(Incdataright==3)
				   {
					   Close(160+Incdataright*48-144,128+wb*68-68);
					   Open(160+Incdataright*48-96,128+wb*68-68);
				   }	
				else if(Incdataright==4)
				   {
					   Close(160+Incdataright*48-144,128+wb*68-68);
					   Open(160+Incdataright*48-96,128+wb*68-68); 
				   }
			 }
			 if(Flag_Right==1)
			 {
				Flag_Right=0;
				if(Incdataright==1)
				   {   
					   Open(432+Incdataright*48-48,128+wb*68-68);		
				   }
				else if(Incdataright==2)
				   {
					   Close(432+Incdataright*48-96,128+wb*68-68);
					   Open(480+Incdataright*48-96,128+wb*68-68);
				   }	
				else if(Incdataright==3)
				   {
					   Close(480+Incdataright*48-144,128+wb*68-68);
					   Open(480+Incdataright*48-98,128+wb*68-68);
				   }	
				else if(Incdataright==4)
				   {
				       Close(432,128);
					   Close(480+Incdataright*48-144,128+wb*68-68);
					   Open(480+Incdataright*48-96,128+wb*68-68); 
				   }
			    }
			 }
		 if(Flag_Change==1)
	     {
	        Flag_Change=0;
	        dispaly_para();
		 }
		 if(Flag_Yu==1)
		 {
			Flag_Yu=0;
			if(Incdataright==1)
			   {   
				   Open(432,128);		
			   }
			else if(Incdataright==2)
			   {
				   Close(432,128);
				   Open(480,128);
			   }	
			else if(Incdataright==3)
			   {
				   Close(480,128);
				   Open(528,128);
			   }	
			else if(Incdataright==4)
			   {
				   Close(528,128);
				   Open(576,128); 
			   }
		  }
    }       
}


/**********************************************
*             ������ʾ 
**********************************************/
void dispaly_para()
{
      if(Flag_RefrPara==0)
      {
	   Flag_RefrPara=1;
	   Clear();
	   Lcd_Clear();
	   Txtext(200,12,"��");
	   Tnumber(260,12,xh);
	   Txtext(296,12,"��̽ͷ");
       Txtext(112,80,"������ֵ:");
       Tnumber(432,80,YuThouth[xh-1]);
       Txtext(456,80,".");
	   Tnumber(480,80,YuCent[xh-1]);
	   Tnumber(528,80,YuTenth[xh-1]);
	   Tnumber(576,80,YuGe[xh-1]);
	   Txtext(16,148,"N0:");
	   Tnumber(112,148,DataThouth[xh*10-9]);
	   Tnumber(160,148,DataCent[xh*10-9]);
	   Tnumber(208,148,DataTenth[xh*10-9]);
	   Tnumber(256,148,DataGe[xh*10-9]);

	   Txtext(336,148,"f0:");
	   Tnumber(432,148,DataThouth[xh*10-8]);
	   Tnumber(480,148,DataCent[xh*10-8]);
	   Tnumber(528,148,DataTenth[xh*10-8]);
	   Tnumber(576,148,DataGe[xh*10-8]);

	   Txtext(16,216,"N1:");
	   Tnumber(112,216,DataThouth[xh*10-7]);
	   Tnumber(160,216,DataCent[xh*10-7]);
	   Tnumber(208,216,DataTenth[xh*10-7]);
	   Tnumber(256,216,DataGe[xh*10-7]);
	  
       
	   Txtext(336,216,"f1:");
	   Tnumber(432,216,DataThouth[xh*10-6]);
	   Tnumber(480,216,DataCent[xh*10-6]);
	   Tnumber(528,216,DataTenth[xh*10-6]);
	   Tnumber(576,216,DataGe[xh*10-6]);
	   if(xh<=7)                            //�ڰ˸�̽ͷֻ�����β���
	   { 
		   Txtext(16,284,"N2:");
		   Tnumber(112,284,DataThouth[xh*10-5]);
		   Tnumber(160,284,DataCent[xh*10-5]);
		   Tnumber(208,284,DataTenth[xh*10-5]);
		   Tnumber(256,284,DataGe[xh*10-5]);
	       
		   Txtext(336,284,"f2:");
		   Tnumber(432,284,DataThouth[xh*10-4]);
		   Tnumber(480,284,DataCent[xh*10-4]);
		   Tnumber(528,284,DataTenth[xh*10-4]);
		   Tnumber(576,284,DataGe[xh*10-4]);
	     
		   Txtext(16,352,"N3:");
		   Tnumber(112,352,DataThouth[xh*10-3]);
		   Tnumber(160,352,DataCent[xh*10-3]);
	       Tnumber(208,352,DataTenth[xh*10-3]);
	       Tnumber(256,352,DataGe[xh*10-3]);

		   Txtext(336,352,"f3:");
		   Tnumber(432,352,DataThouth[xh*10-2]);
		   Tnumber(480,352,DataCent[xh*10-2]);
	       Tnumber(528,352,DataTenth[xh*10-2]);
	       Tnumber(576,352,DataGe[xh*10-2]);

		   Txtext(16,420,"N4:");
		   Tnumber(112,420,DataThouth[xh*10-1]);
		   Tnumber(160,420,DataCent[xh*10-1]);
	       Tnumber(208,420,DataTenth[xh*10-1]);
	       Tnumber(256,420,DataGe[xh*10-1]);
		
		   Txtext(336,420,"f4:");
		   Tnumber(432,420,DataThouth[xh*10]);
		   Tnumber(480,420,DataCent[xh*10]);
	       Tnumber(528,420,DataTenth[xh*10]);
	       Tnumber(576,420,DataGe[xh*10]); 
	    } 
    }
    else if(Flag_RefrPara==1)
    {   
	   Tnumber(260,12,xh);
       Tnumber(432,80,YuThouth[xh-1]);
	   Tnumber(480,80,YuCent[xh-1]);
	   Tnumber(528,80,YuTenth[xh-1]);
	   Tnumber(576,80,YuGe[xh-1]);
	   Tnumber(112,148,DataThouth[xh*10-9]);
	   Tnumber(160,148,DataCent[xh*10-9]);
	   Tnumber(208,148,DataTenth[xh*10-9]);
	   Tnumber(256,148,DataGe[xh*10-9]);
	
	   Tnumber(432,148,DataThouth[xh*10-8]);
	   Tnumber(480,148,DataCent[xh*10-8]);
	   Tnumber(528,148,DataTenth[xh*10-8]);
	   Tnumber(576,148,DataGe[xh*10-8]);

	   Tnumber(112,216,DataThouth[xh*10-7]);
	   Tnumber(160,216,DataCent[xh*10-7]);
	   Tnumber(208,216,DataTenth[xh*10-7]);
	   Tnumber(256,216,DataGe[xh*10-7]);

	   Tnumber(432,216,DataThouth[xh*10-6]);
	   Tnumber(480,216,DataCent[xh*10-6]);
	   Tnumber(528,216,DataTenth[xh*10-6]);
	   Tnumber(576,216,DataGe[xh*10-6]);
	    if(xh<=7)                            //�ڰ˸�̽ͷֻ�����β���
	     {
		   Tnumber(112,284,DataThouth[xh*10-5]);
		   Tnumber(160,284,DataCent[xh*10-5]);
		   Tnumber(208,284,DataTenth[xh*10-5]);
		   Tnumber(256,284,DataGe[xh*10-5]);
	       
		   Tnumber(432,284,DataThouth[xh*10-4]);
		   Tnumber(480,284,DataCent[xh*10-4]);
		   Tnumber(528,284,DataTenth[xh*10-4]);
		   Tnumber(576,284,DataGe[xh*10-4]);
	     
		   Tnumber(112,352,DataThouth[xh*10-3]);
		   Tnumber(160,352,DataCent[xh*10-3]);
	       Tnumber(208,352,DataTenth[xh*10-3]);
	       Tnumber(256,352,DataGe[xh*10-3]);

		   Tnumber(432,352,DataThouth[xh*10-2]);
		   Tnumber(480,352,DataCent[xh*10-2]);
	       Tnumber(528,352,DataTenth[xh*10-2]);
	       Tnumber(576,352,DataGe[xh*10-2]);

		   Tnumber(112,420,DataThouth[xh*10-1]);
		   Tnumber(160,420,DataCent[xh*10-1]);
	       Tnumber(208,420,DataTenth[xh*10-1]);
	       Tnumber(256,420,DataGe[xh*10-1]);
		
		   Tnumber(432,420,DataThouth[xh*10]);
		   Tnumber(480,420,DataCent[xh*10]);
	       Tnumber(528,420,DataTenth[xh*10]);
	       Tnumber(576,420,DataGe[xh*10]); 
	    } 
    	Incdata++;
        if(Incdata>9)
		{
		 Incdata=0;
		}
	 }
}
