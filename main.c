/*********************************************************************************************************************
* �������ƣ�
           r������
* ���ܣ�
        1.   ʱ��������ò���ʱ�䣬��ͨ��Һ������ʾ��
       	2.   ̽ͷ��������̽ͷ��������ͨ��Һ������ʾ��
		3.   ������������̽ͷ�����������ͨ��Һ������ʾÿ��̽ͷ�����ʣ�
		4.   �����������ò�����С����ͨ��Һ������ʾ��
		5.   �������Ƽ������ò�����λ�����������ϣ�������ÿλ�����Ĵ�С����ͨ��Һ���������ʾ��
        6.   ������� �����÷��ص���ʼ��״̬��
   
* ������   ̽ͷ���ü�������������λ����ʱ�����ü����������ü������Ƽ������Ƽ���
* �ӿڣ�
           ���룺1.	��������ͨ��74HC244�ӵ�Ƭ�����������ߣ�
		         2. ̽ͷ1-15�ӵ�8053����������Ƭ8253���ݶ˽ӵ�Ƭ�����������ߣ�

		   ����� 8253���ź�--P1.1,����--P1.0������/�ӻ�ѡ��--P1.6����λ--P4.7������������--P1.7,�ӻ���ʾ����--P3.3;
		          ���������ź�Note1--P4.4,����Ӧ���ź�ACKO--P3.5,�ӻ������ź�Note0--P3.4,�ӻ�Ӧ���ź�ACK1--P4.6��
				  ��������--P0�ڣ���ַ����--P2�ڣ�
		            							   
*********************************************************************************************************************/
/********************************
���޶��ģ�����������ֵ�����������õ���ֵ
*********************************/

#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"


bdata bit Flag_KeyInput;                         //̽ͷ������־��Ĭ��0
bdata bit Flag_KeyTime;                          //����ʱ���־��0
bdata bit Flag_KeyPara;                          //���������־��0
bdata bit Flag_KeyRight;                         //���Ʊ�־��Ĭ��0
bdata bit Flag_KeyLeft;                          //���Ʊ�־��0                                                        
bdata bit Flag_KeyReset;                         //������ʾ��־��0
bdata bit Flag_KeyOK;                            //������־��Ĭ��0
bdata bit Flag_Warn;                             //������־
bdata bit Flag_Mast;                             //���ӻ���־
bdata bit Flag_RefrInput;                        //̽ͷ���������ٴΰ��±�־
bdata bit Flag_RefrTime;                         //����ʱ�䰴���ٴΰ��±�־
bdata bit Flag_RefrPara;                         //������������ٴΰ��±�־
bdata bit Flag_RefrRight;                        //���ư����ٴΰ��±�־
bdata bit Flag_RefrLeft;                         //���ư����ٴΰ��±�־
bdata bit measure_flag;							//Ĭ��0
bdata bit bSwOld;                                //ʵ��ɼ������ɵ�״̬
bdata bit bSwNew;                                //ʵ��ɼ������µ�״̬
bdata bit bSbOld;                                //���û���ʾ�����ɵ�״̬
bdata bit bSbNew;                                //���û���ʾ�����µ�״̬
bdata bit bSsOld;                                //�����������ɵ�״̬
bdata bit bSsNew;                                //�����������µ�״̬
bdata bit Flag_Warn_Led;                         //��Ļ�ϱ�������˸��־��Ĭ��0
bdata bit Flag_Warn_Flash;                       //LED����˸��־
bdata bit FlagMasColSlavOK;                      //�����ɼ�����ʱ����˸��ʱ��־��Ĭ��0
bdata bit Flag8253Cnting;                        //8253������ʱ��ʼ��־��Ĭ��0,�ڰ��²�����������1
bdata bit Flag_ParaChange;                       //�������øı䣬Ĭ��0
bdata bit Flag_InputChange;                      //̽ͷ���øı䣬Ĭ��0
bdata bit Flag_Collateral;                       //�ӻ�����ָ�ʱ��־
bdata bit Flag_Meant;							//����ƽ��ֵ��ʶ
//bit Flag_Commond;
uchar dt_in[331];                       	 //IIC�洢���飬��λһ�飬��83��,оƬ�ڲ�Ĭ��0Ϊ255
uchar data Keyvar=0xFF;                    //��������
uchar jishucount;							//�ڼ��μ�����������ƽ��ֵʱ֪�����ǵڼ��μ���
uint  jishu;								//������
uint count;


extern void Serial_Port_Two_Initial();
extern void Serial_Port_One_Initial();
extern void display_b();
extern void Init_MCU();
extern void Init_RAM();							
extern void Init_Para();
extern bit Speak_Alarm();
extern void Select_Mast();
extern void key_function();
extern void RedLed_Flash();
extern void Warning_Disp(uchar Num,uchar Flag,uchar State);
extern void Updata_Led(uchar j);
//extern void bakeup_conv_data();
extern void Backup_Flash_serial1(uchar *flag,uchar *state);
extern void Init_IO();
extern void Transfer();
extern void parameterleft();
extern void BackUp_displayA();
extern void display_covn_data();
extern void Init_Buf();
extern uint idata compare[24];
//extern float idata save[16];

/*********************************
*            ������
**********************************/
void main()
{ 
    //uchar i;
	Init_IO();                                    //I/O�ڳ�ʼ��,
	Init_MCU();                                   //��������ʼ����T0��ʱ�������ڷ�ʽ1��ʼ������ʾ"���Եȣ���ʼ����",GATE=0,
	Init_RAM();                                   //λ������ʼ��
	Init_Para();                                  //������ʼ��,��ʼ����������IIC��ȡ��ֵ����Ϣ���������
	Init_Buf();									//����ͨѶ�����ʼ��
//	Select_Mast();                                //�ж����ӻ�	
	Init_Time0();	                             //������ʱ��0����ʱ��0��ʼ���������ڷ�ʽһ����ʱ��ֵ��H:0xDC,L:0x00;�жϼ�����ʼ������ʱ5ms,NumT0=0;
    Lcd_Clear();								//LCD����
	Lcd_Start();								//��ʾ"�廪��ѧ/n����������Դ�����о�Ժ"
	EX0=1;                                        //ֻ�����ⲿ0�жϣ��ж�0Ϊ����
	IT0=1;                                        //�ⲿ�ж�0 
	while(1)
	{  	
//	   BackUp_Display();	
	   Select_Mast();                                //�ж����ӻ���д�����ӻ���־Flag_Mast=1(����)0(�ӻ�)
	   key_function(); 								//�������ܣ���whileѭ���в��ϼ�ⰴ����־��������־�ɰ����ⲿ�ж�������
	   if(Flag_Tim0)	                         //8253������ʱ����ʱ����ȡ̽ͷ�ļ������տ�ʼFlag_Tim0=0������Flag8253Counting������ʱ��ʶΪ1���Ҽ������жϴ��������趨���жϴ���ʱ��Flag_timo�Ż����1
	    {										//Flag_Tim0Ϊ������������־,��ʼΪ0,�����򿪶�ʱ���жϺ�,��ʱ��5ms�ж�һ��,���Flag_Tim0һ��,Ϊһ��ʾ��������ʱ����
			Flag_Tim0 = 0;		                 //��ʱ��־��0
			Flag_Warn = 0;		                 //������־��0
			GetAndDisdata(); 	                 //��8253���������õ��������������,������buf����
			ShowData();                          //��ʾ�������� 
			shortdelay(1000);	          		//
	        Transfer();                          //���д�������
//			bakeup_conv_data();           
			Init_8253();   						//��ʼ��8253
			                                      //��ʱ����ʱ�������ֿ�ʼ��ʼ�����м���
	   }
	    if(Flag_Tim0 == 0)                        //����8253���������У��������жϣ��������ж���
       {
			if(Flag_Warn_Flash == 1)                   //����LEDָʾ����˸�����ʱ��Flag_Warn_FlashΪLED����˸��־
			{
				Flag_Warn_Flash = 0;
	            Led_Flash();
			}
			if((Flag_Warn_Led==1) && (PCOLSIG==0))      //��Ļ�Ϻ����˸�����ʱ,������û���ҵ�PCOLSIG=0�ĳ����
			{	
				Flag_Warn_Led = 0;
				RedLed_Flash();							//����˸
			 } 
	    }
	}             
}


/************************************
*            �����жϳ���
**************************************/
void Int0() interrupt 0
 {
	uint  idata i;
	uchar idata Ptem1,Ptem2;                               
	ES=0;                                           //��ֹ�����ж�
	IE2=0x00;                                       //��ֹ����2�ж�  
	EX0=0;                                          //��ֹ�ⲿ0�ж�
	Keyvar=Keypress;
	Ptem1=Keyvar;                                   //��ȡ�˿ڵĵ�ַ�Ƿ��ȸ���ȫ����1
	Ptem1=Ptem1&0x7F;                          
	for(i=0;i<3000;i++);                            //��ʱȥ��
	Ptem2=Keyvar;
	Ptem2=Ptem2&0x7F;                  
	if(Ptem1!=Ptem2)
	{
	  EX0=1;
	}
	else 
	{
	   if(Keypress==0x7E)                     //����1����������
			 Flag_KeyPara=1;
	   else if(Keypress==0x7D)                //����2������ 
		     Flag_KeyOK=1;
	   else if(Keypress==0x7B)                //����3����������
		     Flag_KeyRight=1;
	   else if(Keypress==0x77)                //����4����������
		     Flag_KeyReset=1;                    
	   else if(Keypress==0x6F)                //����5��ʱ������
		     Flag_KeyTime=1; 
	   else if(Keypress==0x5F)                //����6��̽ͷ����
		     Flag_KeyInput=1; 
       else if(Keypress==0x3F)                //����7����������
             Flag_KeyLeft=1;
	}
	EX0=1;                         //need change
}


/****************************************
*         CPU��ʼ��
****************************************/
void Init_MCU()
{
	/*��ʱ�������ĳ�ʼ��*/                         
	TMOD=0x01;                                 //T0Ϊ��ʱ���������ڷ�ʽ1                                        
	GATE=0;                                    //8253������
	EA=0;                                      //�����������ж� 
	TR0=0;										//��ʱ�رն�ʱ��0
    IE0=0;                                     //���ⲿ�ж�0��0
	/*ΪLCD��ʾ  ��ֹ�����ж�*/
	EX0=0;                                     //��ֹ�ⲿ1�ж�
	ET0=0;                                     //��ֹ��ʱ0�ж�                        
	ES=0;                                      //��ֹ����1�ж�
	IE2=0x00;                                  //��ֹ����2�ж� 
	/*******���ڳ�ʼ��*********/
    Serial_Port_Two_Initial();                 //����2�жϳ�ʼ��(δ�ҵ�����)
	/*Һ����ʼ����ʾ*/
    Lcd_Clear();                               //����2����
    //picture();                               //����2��ʼͼƬ
    Lcd_init();                                //����2��ʼ��LCD��ʾ�����Եȣ����г�ʼ���С�
	Clear();                                   //������
}
/****************************************
*         ������ʼ��
****************************************/
void Init_Para()
{
	uchar i,j,k,y,m,n,q,x;//,w ,jj 
//	uchar s,z;
	uchar p=0;

	for(i=0;i<75;i++)                           //��ʼ��"����"����
	{
		DataGe[i]=0;      						//��λ
		DataTenth[i]=0;      					//ʮ��λ
		DataCent[i]=0;     						//�ٷ�λ
		DataThouth[i]=0;						//ǧ��λ
	}
	for(j=0;j<8;j++)
	{
	    YuGe[i]=0;      
		YuTenth[i]=0;      
		YuCent[i]=0;     
		YuThouth[i]=0;
        Flag_need_warn[i]=0;
        State_On_Off[i]=0;
		DoseRata[j] = 0;						//ÿ��̽ͷ��õļ�����
		GDoseRata[j] = 0;						//ÿ��̽ͷ��õļ�����
		DDoseRata[j] = 0;						//ÿ��̽ͷ��õļ�����
		TrueRata[j] = 0;						//ÿ��̽ͷ��õļ�����
	    Mean_Value[j] = 0;
		count_change_flag[j]=0;
		save0[j]=0;
		save1[j]=0;
		save2[j]=0;
	}
	for(m=0;m<83;m++)                           //��ʼ���洢����
 	{
		dt_in[4*m]=0;
		dt_in[4*m+1]=0;
		dt_in[4*m+2]=0;
		dt_in[4*m+3]=0;
    } 	
	for(n=0;n<65;n++)                           //��ʼ����������
	{
       send_buf[n]=0;							//NIM_A��NIM_B�������ݵ�����
	}  	
/*	for(s=0;s<20;s++)
	{
		backup_data[s] = 0;
	}*/
	for(y=0;y<8;y++)
	{
        Flag_need_warn[y]=0;
        State_On_Off[y]=0;      
//		data_A[y]=0;  
	}
/*	for(z=0;z<12;z++)
	{
		data_B[z]=0;
	}*/
	/*��AT24C16�ж�ȡ����*/
    DS_ReadData(dt_in);							//IIC EEPROM��ȡ����
	for(x=0;x<83;x++)                          //оƬ�ڲ�Ĭ��0Ϊ255
	{
		if(dt_in[x*4]==255)
		{
			dt_in[4*x]=0;
		}
		if(dt_in[x*4+1]==255)
		{
			dt_in[4*x+1]=0;
		}
		if(dt_in[x*4+2]==255)
		{
			dt_in[4*x+2]=0;
		}
		if(dt_in[x*4+3]==255)
		{
			dt_in[4*x+3]=0;
		}
	}
    Incinput=dt_in[330];                       // ��ȡIIC�е�̽ͷ������־
	/*********��ȡ����*********/
    for(q=0;q<74;q++)
	{
		p++;
		if(p%11==0)
		{
			 p++;
		 } 
	    if((dt_in[p*4+1] <= 9)&&(dt_in[p*4+2] <= 9)&&(dt_in[p*4+3] <= 9)&&(dt_in[p*4+4] <= 9))
        {
			DataThouth[q+1] = dt_in[p*4+1];		//ǧ��λ
			DataCent[q+1] = dt_in[p*4+2];		//�ٷ�λ
			DataTenth[q+1] = dt_in[p*4+3];		//ʮ��λ
			DataGe[q+1] = dt_in[p*4+4]; 		//��λ
			
		  }
	   }
	/********��ȡ��ֵ***********/ 
    for(k=0;k<8;k++)
	{
	 if((dt_in[(k*44)+1] <= 9)&&(dt_in[(k*44)+2] <= 9)&&(dt_in[(k*44)+3] <= 9)&&(dt_in[(k*44)+4] <= 9))
       {
	        YuThouth[k] = dt_in[(k*44)+1];
			YuCent[k] = dt_in[(k*44)+2];
			YuTenth[k] = dt_in[(k*44)+3];
			YuGe[k] = dt_in[(k*44)+4];
	    }
	}	   
   if(dt_in[1]<=9) 
   {
	   Incdata=dt_in[1];
   }
}

/****************************************
*         �����ʼ��
****************************************/
void Init_Buf()
{
	uchar n;
	for(n=0;n<57;n++)                             //��ʼ����������,**���ڴ˴�ԭ�򣺷��ڲ�����ʼ�������У��ڴ˴�����ʱ�������Ѿ������仯** can not understand
	{
	   receive_buf[n] = 0;
	}
}

/************************************
*          λ��ʶ��ʼ��
*************************************/
void Init_RAM()
{
    bz = 1;                                      //�������ó�ʼ����������־
	xh = 1;                                      //�������ó�ʼ̽ͷ������־
	wb = 1;                                      //�������ó�ʼ����������־
    Note0 = 1;                                   //���ڴӻ��������ݱ�־��ʼ��(P3^4)
	Note1 = 1;									//���ڴӻ��������ݱ�־��ʼ��(P4^4)
	Ack0 = 1;									//������������������Ӧ���źű�־(P3^5)
	Ack1 = 1;  									//������������������Ӧ���źű�־(P4^6)
	Speak = 1;                                    //�ߵ�ƽ�������������(P1^0)	
	Incdata = 0;                                  //������С��ʼ��
	Inctime = 0;                                  //ʱ�䰴�����ӱ�־��ʼ��
	Incinput = 8;                                 //Ĭ����ʾ̽ͷ����Ϊ8��
	Redrawtime =60;                              //Ĭ��ˢ��ʱ��Ϊ60��
	Incdataright = 1;							//���ư�����ʼ��
	jishu=0;
	jishucount=0;
	count=0;                             
    Var_Led = 0xff;                               //led����ָʾ��
	Led573 = Var_Led; 								//��XBYTE�������ݸ�LED
    Var_Signal1 = 0xff;                           //̽ͷ�����źű�־���Ӽ�����
    Var_Signal2 = 0x00; 							//̽ͷ�����źű�־���ӵ�����
    Var_Signal3 = 0x00;								//̽ͷ�����źű�־���ӵ�����
	Signal1 = Var_Signal1;							//�������������źţ�Ĭ��ȫ�Ӽ�����
    Signal2 = Var_Signal2;
    Signal3 = Var_Signal3;
    sendadd = 0x00;                                //�����дӻ����ͳ�ʼָ��
    PCOLSIG = 1;                                   //�ߵ�ƽ�����ɼ����ݣ�P3^3,
    bSwNew = PCOLSIG;	                           //�ɼ����ݣ�bswNewΪλ����������
    bSwOld = bSwNew;    							//�ϰ���
    PALE = 1;                                      //�ߵ�ƽ������ʾ���û�,P4^5,������ʾ������
    bSbOld = 1;   									//���û���ʾ����������־
	Judge_Speak = 1;                               //�ߵ�ƽ�����������������P1^7���������ذ���
	bSsNew = Judge_Speak;	                       //����������
    bSsOld = bSsNew;    
    Zhu_Cong = 1;                                  //���ӻ���־�źţ��ߵ�ƽ����������             	
	Flag_KeyInput = 0;                             //̽ͷ������־
	Flag_KeyTime = 0;                              //����ʱ���־
	Flag_KeyPara = 0;                              //���������־
	Flag_KeyRight = 0;                             //���Ʊ�־
	Flag_KeyOK = 0;                                //��ʼ������־
	Flag_KeyReset = 0;                             //������ʾ��־;
	Flag_KeyLeft = 0;
	Flag_Tim0 = 0;
	Flag_Warn_Led = 0;
	FlagMasColSlavOK = 0; 
	Flag8253Cnting = 0;
	Flag_Warn_Count = 0;
	measure_flag=1;
	FlagCollErr = 0;
    CntColTimer = 0;
    FlagColling = 0;
	Flag_ParaChange = 0;
	Flag_InputChange = 0;
//	Flag_serial1_led = 0;
	CmOverTime = 0;
    Flag_Collateral = 0;  
    FlagCollFall = 0;   		
	Flag_Meant = 0; 
//	Flag_data_change = 0;
//	Flag_Commond = 1;
}

/************************************
*         I/O�˿ڳ�ʼ��
*************************************/
void Init_IO()                       //����I/O�˿�����
{
    P4SW = 0x70;                    //P4.4 P4.5 P4.6��Ϊ��ͨI/O��

	P3M1 |= 0x30;                   //����P3.5��P3.4������
	P3M0 &= 0xCF;

	P3M1 &= 0xFC;
	P3M0 |= 0x03;                   //����P3.1, p3.0��Ϊǿ�������	

	P4M1 &= 0xAF;                   //����P4��Ϊǿ�������
	P4M0 |= 0x50;
	
}

/************************************
*          �ɼ����ݰ�������
*************************************/
/*bit Select_collect(void)
{
	bit bRet;
	bRet = 0;
	bSwNew=PCOLSIG;
    if(bSwNew != bSwOld)
    {
    	if(bSwOld==0)
         {
          	bRet = 0;          //���ɼ��ӻ�����
	     }
        else
         {        	      
			bRet = 1;          //�ɼ��ӻ�����
         }
		bSwOld=bSwNew;
    }
   	return bRet;	
}*/

/************************************
*          ���ӻ���������
*************************************/
void Select_Mast()
{
	if(Zhu_Cong==1)				//P1^6���ӻ����ذ������ߵ�ƽΪ����
     {
      	Flag_Mast = 1;          //����
     }
    else if(Zhu_Cong==0)
     {
      	Flag_Mast = 0;         //�ӻ�
     }		
}

/************************************
*       ���û���ʾ��������****************4.21��������������⣺������ʾ��������ͨ���������ǿ��ذ�����ʽ��
*************************************/
/*void BackUp_Display(void)
{	
	bSbNew = PALE;
    if(bSbNew != bSbOld)
    {
		if(bSbNew == 0)
         {  
			Flag_Commond = ~Flag_Commond;
         }
		bSbOld = bSbNew;
    }
}*/


/***********************************
*         ��������������
***********************************/
bit Speak_Alarm()
{
	bit bspeak;					//����һλBool�ͱ���
	bspeak = 0;
	bSsNew=Judge_Speak;			//���������ذ���(����Ϊ��״̬)
    if(bSsNew != bSsOld)		//����������״̬�б䶯
    {
    	if(bSsOld==0)			//������״̬Ϊ0
         {
          	bspeak = 0;        //������������
	     }
        else
         {       
			bspeak = 1;      //����������
         }
		bSsOld=bSsNew;
    }
   	return bspeak;	
}
/**********************************
*          ��������
**********************************/
void key_function()
{
	  if(Flag_KeyInput)                  //̽ͷ��������(���º����̽ͷ�ı궨����,ͬʱ��ʾGM�����ܺ͵����ҵļ���ֵ�ͼ�������ļ���ֵ)
	   {
	       Flag_InputChange = 1;         //̽ͷ���ñ䶯��־��1,��ʾ̽ͷ�䶯��,�ڸ���̽ͷ���������,������Ϊһ,����ȴ洢�Ĺ�������
		   Flag_KeyInput = 0;			//������־λ����
		   setinput();
		   EX0=1;						//�����ⲿ�ж�0
	   }
	   if(Flag_KeyTime)                   //ʱ�䰴������
	   {
		   Flag_KeyTime = 0;
		   settime();
		   EX0=1;
	   }
	   if(Flag_KeyPara)                   //������������
	   {
	       Flag_ParaChange = 1;           //�������ñ䶯��־��1,��ʾ�����䶯��,�ڸ���������������,������Ϊһ,����ȴ洢�Ĺ��Ĳ���
		   Flag_KeyPara = 0;
		   setparameter();
		   EX0=1;
	   }
	   if(Flag_KeyRight)                  //���ư�������
	   {
		   Flag_KeyRight = 0;
		   parameterright();
		   EX0=1;
	   }
	   if(Flag_KeyLeft)
	   {
		   Flag_KeyLeft = 0;
		   parameterleft();
		   EX0=1;
	   }
	   if(Flag_KeyOK)                     //������������
	   {
		   Flag_KeyOK = 0;
		   measure();
           EX0=0;
	   }
	   if(Flag_KeyReset)                  //���谴������
	   {
		   Flag_KeyReset = 0;
		   ReSet();
		   EX0=1;
	   }
}


/****************************************
*          ���ӻ����д���
****************************************/
void Transfer()
{
     if(Flag_Mast==0)                                  //�ӻ�
       {
	        Flag_Collateral = 1;
			CmOverTime = 0;
            send_buf[64]=Incinput;
			Send_Word(send_buf,NUMSENDBYTES);         //��������
       }
       if(Flag_Mast==1)                               //����
       {		
            Collect_Word(receive_buf,57);    
	  
       }
         	   
}



