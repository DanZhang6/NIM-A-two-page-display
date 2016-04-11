/********************************************************************************************
*������
*      8253��������
*���ܣ�
*    1. ��8253�ж�ȡ������ÿ��8253�к���3�������������8253��15��̽ͷ�ļ�����
*       ǰ�߸�̽ͷ��˫̽ͷ���ͼ���̽ͷ�͸߼���̽ͷ�����ڰ˸�̽ͷ�ǵ�̽ͷ���߼���̽ͷ����
*       ���ݼ�����ѡ���ĸ�̽ͷ������
*    2. T0��ʱ�ж�
*       T0������16λ�Զ�����ģʽ�����ϼ�����ÿѭ��һ�Σ��ܶ�ʱ5Ms, ������ֵDC00������22.1184M��
*    3. ��ʱ������ȡ�����������趨�Ĳ�������ʾ��̽ͷ�ļ����ʣ�
*    4. ���ݼ����ʵĲ�ͬ��ѡ��ͬ�ĵ�λ��uGy/s,mGy/s,Gy/s��;

***********************************************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h"
#include "absacc.h"
#include "stdio.h"
#include "math.h"

#define  C82531C  XBYTE[0x8300]	//8253������˿ڣ���ַ���� CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=11��
#define  C825310D XBYTE[0x8000] //������0				   CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=00��
#define  C825311D XBYTE[0x8100]	//������1                  CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=01;
#define  C825312D XBYTE[0x8200]	//������2				   CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=10;

#define  C82532C  XBYTE[0x8b00]	//XBYTE�������ǽ��ⲿI/O�˿���Ϊ�������ڲ���16λ�����ƣ���Ϊ����
#define  C825320D XBYTE[0x8800]	//ʹ�ܶˣ�XBYTE�����û�������ʹ�ܶ˺��Զ�����wr,rd.
#define  C825321D XBYTE[0x8900]
#define  C825322D XBYTE[0x8a00]

#define  C82533C  XBYTE[0x9300]
#define  C825330D XBYTE[0x9000]
#define  C825331D XBYTE[0x9100]
#define  C825332D XBYTE[0x9200]

#define  C82534C  XBYTE[0x9b00]
#define  C825340D XBYTE[0x9800]
#define  C825341D XBYTE[0x9900]
#define  C825342D XBYTE[0x9a00]

#define  C82535C  XBYTE[0xa300]
#define  C825350D XBYTE[0xa000]
#define  C825351D XBYTE[0xa100]
#define  C825352D XBYTE[0xa200]

uchar  buf[33];                      //��8253�ж����ļ��������ݣ�15�����������Ͱ�λ�߰�λ����3-32��1Ϊ̽ͷ������2Ϊ����ʱ��
uchar  DataGe[75];                   //̽�����궨�����ĸ�λ����ʼʱ��ʮ��ǧ��Ϊ0
uchar  DataTenth[75];                //̽�����궨������ʮ��λ
uchar  DataCent[75];                 //̽�����궨�����İٷ�λ
uchar  DataThouth[75];               //̽�����궨������ǧ��λ
uchar  send_buf[65];                 //NIM_A��NIM_B�������ݵ�����
uchar  Incinput;                     //̽ͷ������־��Ĭ����ʾ̽ͷ����Ϊ8��
float  idata Para[10];               //�趨�õĲ�������
float  GDoseRata[8];                  //ÿ��̽ͷ��õļ�����,GM�����ܵļ�����(2015��10��20�ո�)
float  DDoseRata[8];					//�����ҵļ��������(2015��10��20�ռ�)
float  DoseRata[8];						//��ǰ̽ͷ�����жϵļ���ֵ
float  TrueRate[8];						//��ǰ̽ͷ׼ȷ����ֵ���ڵ�̽ͷ,Ϊһ��ʾGM������
float jishuguan_DoseRata;
float dianlishi_DoseRata;
float  GMean_Value[8];				//ƽ��ֵGM������ƽ��ֵ
float  DMean_Value[8];				//������ƽ��ֵ
float  idata Gsave0[8],Gsave1[8],Gsave2[8];//������ǰ5��DoseRata���ݲ�Ҫ��save0,1,2������¼ǰ��3�ε�DoseRata(ֻƽ�����ε�ֵ)����ƽ������ƽ��ֵ!
float  idata Dsave0[8],Dsave1[8],Dsave2[8];//������


uchar  GFlag_dw[8];                      //GM�����ܵ�λ��־
uchar  DFlag_dw[8];						//�����ҵ�λ��ʶ
uint  idata Tdata[8];                   //̽ͷ������(2015��10��20����TdataΪ�����ܵļ���ֵ)
uint  idata Ddata[8];                   //̽ͷ������(2015��10��20����DdataΪ�����ҵļ���ֵ)
uint idata jishuguan_data;				//�궨ʱ�ļ����ܼ���ֵ�൱��Tdata
uint idata dianlishi_data;				//�궨ʱ�ĵ����Ҽ���ֵ�൱��Tdata
uchar idata Var_Signal1;             //̽ͷ�����ź�1���Ӽ����ܣ�Ĭ��ff���Ӽ�����
uchar idata Var_Signal2;             //̽ͷ�����ź�2���ӵ����ң�Ĭ��00��
uchar idata Var_Signal3;             //̽ͷ�����ź�3���ӵ����ң�Ĭ��00��
uchar idata Var_Led;                 //LED����ָʾ��,Ϊ1ʱ��,��ֵ0xff,
uchar biaoding_input;
bit Flag_Warn_Count;				//Ĭ��0��
uchar Flag_need_Flash[8];			//LED��˸��־
uchar State_Flash[8];
uchar count_change_flag[8];			//�����ı��־��

uchar data Svar1[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};     //�ź�����
uchar data Svar0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
extern uchar jishucount;
extern uint  jishu;
extern uint  count;



void shortdelay(uint i);
void Alarm();
void Led_Disp(uchar Num,uchar Flag,uchar State);
void Updata_Flash(uchar j);
extern bit Speak_Alarm();
/*************************************
*           8253����ֵ
**************************************/
void GetAndDisdata()                               //ˢ����ʾ�ӳ���
{
	uchar th=0;										//8253��8λ
	uchar tl=0;										//8253�Ͱ�λ
	uchar i,j;                                       //buf�����ʼ��
	for(i=0;i<33;i++)
	{
		buf[i]=0;
	}
    for(j=0;j<10;j++)
	{
		Para[j]=0;
	}
	/*===========����8253�ļ���ֵ==========*/
	C82531C=0x84;                                 //�����һƬ8253������2�ļ���ֵ
	tl=C825312D;									 //read 8253 16 bits data(first L then H )
	buf[3]=0xff-tl;
	th=C825312D;
	buf[4]=0xff-th;

	C82531C=0x44;                                 //�����һƬ8253������1�ļ���ֵ
	tl=C825311D;									 //read 8253 16 bits data(first L then H )
	buf[5]=0xff-tl;
	th=C825311D;
	buf[6]=0xff-th;

	C82531C=0x04;                                //�����һƬ8253������0�ļ���ֵ
	tl=C825310D;									 //read 8253 16 bits data(first L then H )
	buf[7]=0xff-tl;
	th=C825310D;
	buf[8]=0xff-th;

	C82532C=0x84;                                //����ڶ�Ƭ8253������2�ļ���ֵ
	tl=C825322D;									 //read 8253 16 bits data(first L then H )
	buf[9]=0xff-tl;
	th=C825322D;
	buf[10]=0xff-th;

	C82532C=0x44;                                //����ڶ�Ƭ8253������1�ļ���ֵ
	tl=C825321D;									 //read 8253 16 bits data(first L then H )
	buf[11]=0xff-tl;
	th=C825321D;
	buf[12]=0xff-th;

	C82532C=0x04;                               //����ڶ�Ƭ8253������0�ļ���ֵ
	tl=C825320D;									 //read 8253 16 bits data(first L then H )
	buf[13]=0xff-tl;
	th=C825320D;
	buf[14]=0xff-th;

	C82533C=0x84;                                //�������Ƭ8253������2�ļ���ֵ
	tl=C825332D;									 //read 8253 16 bits data(first L then H )
	buf[15]=0xff-tl;
	th=C825332D;
	buf[16]=0xff-th;

	C82533C=0x44;                                //�������Ƭ8253������1�ļ���ֵ
	tl=C825331D;									 //read 8253 16 bits data(first L then H )
	buf[17]=0xff-tl;
	th=C825331D;
	buf[18]=0xff-th;

	C82533C=0x04;                               //�������Ƭ8253������0�ļ���ֵ
	tl=C825330D;									 //read 8253 16 bits data(first L then H )
	buf[19]=0xff-tl;
	th=C825330D;
	buf[20]=0xff-th;

	C82534C=0x84;                                //�������Ƭ8253������2�ļ���ֵ
	tl=C825342D;									 //read 8253 16 bits data(first L then H )
	buf[21]=0xff-tl;
	th=C825342D;
	buf[22]=0xff-th;

	C82534C=0x44;                                //�������Ƭ8253������1�ļ���ֵ
	tl=C825341D;									 //read 8253 16 bits data(first L then H )
	buf[23]=0xff-tl;
	th=C825341D;
	buf[24]=0xff-th;

	C82534C=0x04;                               //�������Ƭ8253������0�ļ���ֵ
	tl=C825340D;									 //read 8253 16 bits data(first L then H )
	buf[25]=0xff-tl;
	th=C825340D;
	buf[26]=0xff-th;

	C82535C=0x84;                                //�������Ƭ8253������2�ļ���ֵ
	tl=C825352D;									 //read 8253 16 bits data(first L then H )
	buf[27]=0xff-tl;
	th=C825352D;
	buf[28]=0xff-th;

	C82535C=0x44;                                //�������Ƭ8253������1�ļ���ֵ
	tl=C825351D;									 //read 8253 16 bits data(first L then H )
	buf[29]=0xff-tl;
	th=C825351D;
	buf[30]=0xff-th;

	C82535C=0x04;                               //�������Ƭ8253������0�ļ���ֵ
	tl=C825350D;									 //read 8253 16 bits data(first L then H )
	buf[31]=0xff-tl;
	th=C825350D;
	buf[32]=0xff-th;

	buf[1]=Incinput;                                 //̽ͷ����
	buf[2]=Redrawtime;                               //����ʱ�䣨ˢ��ʱ���־��

}
/*******************************************
*        8253��������ʼ��
********************************************/
 void Init_8253()
 {

//=======װ���ֵ�ٿ�ʼ����=======//
	C82531C=0xb4;                               //10110100
	C825312D=0xff;                   			//10��ͨ��2,11���ȵ��ֽں���ֽڣ�010������ʽ2������0�������Ƽ�����д��ֵ0xff��ʼ����
	C825312D=0xff;

	C82531C=0x74;                               //01110100
	C825311D=0xff;                              //ͬ��
	C825311D=0xff;

	C82531C=0x34;                               //00110100
	C825310D=0xff;                              //д��ֵ �ȵ��ֽں���ֽ� 82531д��ֵ��Ϳ�ʼ����
	C825310D=0xff;								//����������װ���ֵ

	C82532C=0xb4;                               //10110100
	C825322D=0xff;
	C825322D=0xff;

	C82532C=0x74;                               //01110100
	C825321D=0xff;
	C825321D=0xff;

	C82532C=0x34;                               //00110100
	C825320D=0xff;
	C825320D=0xff;

	C82533C=0xb4;                               //10110100
	C825332D=0xff;
	C825332D=0xff;

	C82533C=0x74;                               //01110100
	C825331D=0xff;
	C825331D=0xff;

	C82533C=0x34;                               //00110100
	C825330D=0xff;
	C825330D=0xff;

	C82534C=0xb4;                               //10110100
	C825342D=0xff;
	C825342D=0xff;

	C82534C=0x74;                               //01110100
	C825341D=0xff;
	C825341D=0xff;

	C82534C=0x34;                               //00110100
	C825340D=0xff;
	C825340D=0xff;

	C82535C=0xb4;                               //10110100
	C825352D=0xff;
	C825352D=0xff;

	C82535C=0x74;                               //01110100
	C825351D=0xff;
	C825351D=0xff;

	C82535C=0x34;                               //00110100
	C825350D=0xff;
	C825350D=0xff;

	Var_Led=0xff;                               //ledָʾ
	Led573=Var_Led;
	EX0=0;                                      //���ⲿ�ж�0
 /*��ʱ�������ĳ�ʼ��*/
    GATE=1;                                     //8253��������
    Flag8253Cnting = 1;
	NumT0 = 0;
  }


/************************************
*           ��ʾ����
************************************/
void ShowData()
{
	uchar m,j,Var1;//,i
	//uchar ii=2;
	uchar u;											//15��10��20�ռ�
    ulong count_temp,jishuguan_count_temp,dianlishi_count_temp;
	float idata ptemp,rata,Drata,yudecide,zdata,jishuguan_rata,jishuguan_jtemp,dianlishi_jtemp;	//15��10��20���޸�
	float Gjtemp[8];									//15��10��20���޸�
	float Djtemp[8];									//15��10��20���޸�
	double yu,yudata,mtemp;
	uchar Tbcd[12];	   									//��õ����ݵ�bcd������
    Lcd_Clear();
if(measure_flag)										//(1)measure_flag��Init_RAM�г�ʼ����Ϊ1
{
	for(u=0;u<8;u++)                   //������ʼ��
	{
		Gjtemp[j]=0;
		Djtemp[j]=0;
		GFlag_dw[j]=0;                      //GM�����ܵ�λ��־
		DFlag_dw[j]=0;
		Ddata[j]=0;										//(2015��10��20�ռ�)
		Tdata[j]=0;                                        //̽ͷ������uint,��ɫΪȫ�ֱ�������ɫΪ�ֲ�����
	}
	for(j=0;j<Incinput;j++)                             //(2)�������Ϊbuf[3]-buf[32];̽ͷ������־
	{
		zdata=0;										//float������
		rata=0;											//float�����ܼ�����
		Drata=0;										//float�����Ҽ�����(2015��10��20�ռ�)
    ptemp=0;										//float
    yudata=0;										//double
		mtemp=0;										//double
		yu=0;											//double
		m=j*10;                           //����
		//i=jishu%(Incinput*3);
		//isave=count%(Incinput*2);
    /*********�жϼ�����***********/
	  /*********ǰ�߸�̽ͷ̽�������ã�ȡ��8253����ֵ��¼��Tdata*/
    if(j<7)											//(3)ǰ�߸�̽ͷ���е�����Ҳ�м��������ڰ˸�ֻ�е����ҹʷֿ�����
		{
      Var1=(Var_Signal1 & Svar1[j]);              //(4)̽ͷ�����ź�1,var-signal̽ͷ���Ʊ�־(ȫ�Ӽ�����)��Svar1[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}
			if(Var1 == Svar1[j])						           //��˼�ǵ�J��̽ͷΪ������ʱ��ͬʱȡ�������ܺ͵����ҵļ���
			{
        Tdata[j] = buf[4*j+4]*256+buf[4*j+3];		//bufΪ�����8253�������߰�λ�Ͱ�λ����16λ(��ǰ���̽���������ܵļ���)
				Ddata[j] = buf[4*j+6]*256+buf[4*j+5];		//�����Ҽ���(2015��10��20�ռ�)
			}
			else if(Var1 == 0)							//(4)
			{
        Ddata[j] = buf[4*j+6]*256+buf[4*j+5];		//�����Ҽ���(2015��10��20�ո�)
        Tdata[j] = buf[4*j+4]*256+buf[4*j+3];		//�����ܼ���(2015��10��20�ռ�)
			  		Var_Signal1 = Var_Signal1 & Svar0[j];	//Svar0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}
			}
			/*ͨ��ˢ��ʱ���־Redrawtime�ʹ�8253�ɼ����ļ���ֵTdata����̽����������,����GM������������Ҽ�������ʷ�����ͬ*/
			if(count_change_flag[j]==0)					//(4)�����ı��־����ʼ��count_change_flag��Ϊ0,����ǰ����ֵ��GM��������Ϊ���ʱΪ0
			{
				if(Redrawtime >= 60)					    //(5)�޸�2012.6.15 ����ΪС��60�������60s�ļ��㷽����ͬ��Redrawtimeˢ��ʱ���־
				{
					zdata = Redrawtime/60;
          rata = Tdata[j]/zdata; 					//�����ܵļ�����
          Drata = Ddata[j]/zdata;					//�����ҵļ�����(2015��10��20�ռ�)
				}
				else if (Redrawtime < 60 )				   //�޸�2012.6.15 ����ΪС��60�������60s�ļ��㷽����ͬ
				{
					zdata =  600/Redrawtime;	          //�޸�2012.7.1 ����Ϊ60�����40,50��С�������������
                	rata =Tdata[j]*zdata/10;				  //�޸�2012.7.1
					Drata =Ddata[j]*zdata/10;					//������(2015��10��20�ռ�)
				}
			}
			else if(count_change_flag[j]==1)				//��Dose_Rata����1000ʱCount_Change_flag��1,������ǰ����ֵ�õ�����Ϊ���ʱΪ1
			{
				 rata=Tdata[j]*1.67;							//��Ϊ��ʱѡ�õ�������Ϊ���,�����ҵ�ˢ��ʱ���־Redrawtime��Ϊ��3;
				 Drata=Ddata[j]*1.67;
			}
			/*�ж�DoseRataֵ��С�������̲������л���ͨ����ʽ��̽�����궨�Ĳ���(Para[])��
			rata(����Ncʵ�ʵļ�����)���������DoseRata���涨�䵥λΪuGy/h��*/
			if(DoseRata[j] < 0.15)	              // �޸�2012.6.15 && (DoseRata[j] >= 0)
			{
        Para[0] = (float)(DataThouth[m+1]*1000+DataCent[m+1]*100+DataTenth[m+1]*10+DataGe[m+1]); //���ڲ���
        Para[1] = (float)((float)(DataThouth[m+2]*1000+DataCent[m+2]*100+DataTenth[m+2]*10+DataGe[m+2])/1000000);
        Para[5] = (float)((float)(DataThouth[m+6]*1000+DataCent[m+6]*100+DataTenth[m+6]*10+DataGe[m+6])/1000);
        GDoseRata[j] = (float)(Para[1]*(rata-Para[0]));
        DDoseRata[j] = (float)(Para[5]*(Drata-Para[4]));
        DoseRata[j]=GDoseRata[j];
        TrueRate[j]=1;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
				DDoseRata[j]=0.0;
			  Var_Signal1 = Var_Signal1 | Svar1[j];                       //�����ź�1�Ӽ����ܣ�Ϊ�ߵ�ƽ
			  Var_Signal2 = Var_Signal2 & Svar0[j];						//�����ź�2�ӵ�����10(8),�͵�ƽ
			  Var_Signal3 = Var_Signal3 & Svar0[j];                       //�����ź�2�ӵ�����10(6),�͵�ƽ
				count_change_flag[j]=0;
			}
			else if((DoseRata[j] >= 0.15) && (DoseRata[j] < 1000))             //�������ʳ���0.15uGy/hʱ��ʹ�ü����ܵĵڶ������
			{
			  Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //���ڲ���
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[4] = (float)(DataThouth[m+5]*1000+DataCent[m+5]*100+DataTenth[m+5]*10+DataGe[m+5]); //���ڲ���
        Para[5] = (float)((float)(DataThouth[m+6]*1000+DataCent[m+6]*100+DataTenth[m+6]*10+DataGe[m+6])/1000);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[5]*(Drata-Para[4]));
        DoseRata[j]=GDoseRata[j];
        TrueRate[j]=1;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar1[j];                   //�����ź�1�Ӽ����ܣ�Ϊ�ߵ�ƽ
        Var_Signal2 = Var_Signal2 & Svar0[j];                   //�����ź�2�ӵ�����10(8),�͵�ƽ
        Var_Signal3 = Var_Signal3 & Svar0[j];                   //�����ź�2�ӵ�����10(6),�͵�ƽ
        count_change_flag[j]=0;
			}
      else if((DoseRata[j] >= 1000) && (DoseRata[j] < 500000))       //�������ʳ���1mGy/hʱ��ʹ�õ����ҵĵ�һ��ŷķ��(10(10))
		  {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //���ڲ���
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[4] = (float)(DataThouth[m+5]*1000+DataCent[m+5]*100+DataTenth[m+5]*10+DataGe[m+5]); //���ڲ���
        Para[5] = (float)((float)(DataThouth[m+6]*1000+DataCent[m+6]*100+DataTenth[m+6]*10+DataGe[m+6])/1000);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[5]*(Drata-Para[4]));
        DoseRata[j]=DDoseRata[j];
        TrueRate[j]=0;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar0[j];                    //�����ź�1�Ӽ����ܣ��ߵ�ƽ,GM�����ܸ�ѹʼ��ͨ��
        Var_Signal2 = Var_Signal2 & Svar0[j];                    //�����ź�2�ӵ�����10(8),�͵�ƽ
        Var_Signal3 = Var_Signal3 & Svar0[j];	                  //�����ź�2�ӵ�����10(6),�͵�ƽ
        count_change_flag[j]=1;
        Redrawtime = 3;
      }
      else if((DoseRata[j] >= 500000) && (DoseRata[j] < 1000000))   //�������ʳ���500mGy/hʱ��ʹ�õ����ҵĵ�һ��ŷķ��(10(8)),�д�����ȷ��
      {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //���ڲ���
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[6] = (float)(DataThouth[m+7]*1000+DataCent[m+7]*100+DataTenth[m+7]*10+DataGe[m+7]); //���ڲ���
        Para[7] = (float)((float)(DataThouth[m+8]*1000+DataCent[m+8]*100+DataTenth[m+8]*10+DataGe[m+8])/10);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[7]*(Drata-Para[6]));
        DoseRata[j]=DDoseRata[j];
        TrueRate[j]=0;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar0[j];                   //�����ź�1�Ӽ����ܣ��͵�ƽ
        Var_Signal2 = Var_Signal2 | Svar1[j];                   //�����ź�2�ӵ�����10(8),�ߵ�ƽ
        Var_Signal3 = Var_Signal3 & Svar0[j];                   //�����ź�2�ӵ�����10(6),�͵�ƽ
        count_change_flag[j]=1;
        Redrawtime = 3;
      }
      else if(DoseRata[j] >= 1000000)                              //�������ʳ���1Gy/hʱ��ʹ�õ����ҵĵ�һ��ŷķ��(10(6)),�д�����ȷ��
      {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //���ڲ���
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[8] = (float)(DataThouth[m+9]*1000+DataCent[m+9]*100+DataTenth[m+9]*10+DataGe[m+9]); //���ڲ���
        Para[9] = (float)((float)((DataThouth[m+10]*1000+DataCent[m+10]*100+DataTenth[m+10]*10+DataGe[m+10])*10));
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[9]*(Drata-Para[8]));
        DoseRata[j]=DDoseRata[j];
        TrueRate[j]=0;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar0[j];
        Var_Signal2 = Var_Signal2 & Svar0[j];
        Var_Signal3 = Var_Signal3 | Svar1[j];
        count_change_flag[j]=1;
        Redrawtime = 3;
      }
			/******��DoseRataȡ�ü�ʱֵ����ƽ��ȡƽ��ֵ����Mean_Value******/
      if(Flag_Meant == 1) 										//ǰ��μ���ֵ��Ҫ,�������Ժ�ļ���ֵ��Ҫ,jishucount>4��Flag_Meant=1
		  {
        if(jishucount==5)
				{
			    GMean_Value[j]=GDoseRata[j];
					DMean_Value[j]=DDoseRata[j];
					Gsave0[j]=GDoseRata[j];
					Dsave0[j]=DDoseRata[j];
				}
				else if(jishucount==6)
				{
					GMean_Value[j]=(float)((Gsave0[j]+GDoseRata[j])/2);
					DMean_Value[j]=(float)((Dsave0[j]+DDoseRata[j])/2);
					Gsave1[j]=GDoseRata[j];
					Dsave1[j]=DDoseRata[j];
				}
				else if(jishucount==7)
				{
				  GMean_Value[j]= (float)((Gsave0[j]+Gsave1[j]+GDoseRata[j])/3);	    //ƽ�����ֵ   add 2012.6.15
					DMean_Value[j]= (float)((Dsave0[j]+Dsave1[j]+DDoseRata[j])/3);
					Gsave2[j]=GDoseRata[j];
					Dsave2[j]=DDoseRata[j];
				}
				else if(jishucount>7)
				{
					Gsave0[j]=Gsave1[j];
					Gsave1[j]=Gsave2[j];
					Gsave2[j]=GDoseRata[j];
					GMean_Value[j]=(float)((Gsave0[j]+Gsave1[j]+GDoseRata[j])/3);
					Dsave0[j]=Dsave1[j];
					Dsave1[j]=Dsave2[j];
					Dsave2[j]=DDoseRata[j];
					DMean_Value[j]=(float)((Dsave0[j]+Dsave1[j]+DDoseRata[j])/3);
				}
      }
      else if(Flag_Meant == 0)
		  {
        GMean_Value[j] = GDoseRata[j];
			  DMean_Value[j] = DDoseRata[j];
		  }
    }							//ǰ�߸��е�����Ҳ�м����������ж��ŷķ��
    else if(j == 7)                        //NIM_A�ĵڰ˸�̽ͷֻ�и�ѹ�����ң���ֻ��һ��ŷķ��
    {
			Ddata[j] = buf[32]*256 + buf[31];
		    //compare[i]=Tdata;
			//if((compare[j]==compare[j+Incinput])&&(compare[j+Incinput]==compare[j+Incinput*2])&&(compare[j+Incinput*2]==compare[j]))
			// Tdata=0;
			if(Redrawtime >= 60)		       //�޸�2012.6.15 ����ΪС��60�������60s�ļ��㷽����ͬ
			{
				zdata = Redrawtime/60;
            	Drata = Ddata[j]/zdata;
			}
			else if (Redrawtime < 60)		   //�޸�2012.6.15 ����ΪС��60�������60s�ļ��㷽����ͬ
			{
				zdata =  Ddata[j]*60;
				Drata = zdata/Redrawtime;
			}
	   	    if(DoseRata[j] < 1000000)
			{
		        Para[0] = (float)(DataThouth[m+1]*1000+DataCent[m+1]*100+DataTenth[m+1]*10+DataGe[m+1]); //���ڲ���
			    Para[1] = (float)((float)(DataThouth[m+2]*1000+DataCent[m+2]*100+DataTenth[m+2]*10+DataGe[m+2])/1000000);
			    DDoseRata[j] = (float)(Para[1]*(Drata-Para[0]));
				DoseRata[j]=DDoseRata[j];
				TrueRate[j]=0;
				if(DDoseRata[j]<0)
					DDoseRata[j]=0.0;
		        Var_Signal3 = Var_Signal3 & Svar0[j];
            }
		    else if(DoseRata[j] >= 1000000)
			{
			    Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //���ڲ���
			    Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
			    DDoseRata[j] = (float)(Para[3]*(Drata-Para[2]));
				DoseRata[j]=DDoseRata[j];
				TrueRate[j]=0;
				if(DDoseRata[j]<0)
					DDoseRata[j]=0.0;
		        Var_Signal3 = Var_Signal3 | Svar1[j];
				Redrawtime = 3;
			}

			if(Flag_Meant == 1)
		  	{
			    //save[isave]=DoseRata[j];
			  	if(jishucount==5)
				{
			        DMean_Value[j]=DDoseRata[j];
					Dsave0[j]=DDoseRata[j];
				}
				else if(jishucount==6)
				{
					DMean_Value[j]=(float)((DMean_Value[j]+DDoseRata[j])/2);
					Dsave1[j]=DDoseRata[j];
				}
				else if(jishucount==7)
				{
					Dsave2[j]=DDoseRata[j];
				    DMean_Value[j]= (float)((Dsave0[j]+Dsave1[j]+Dsave2[j])/3);	    //ƽ�����ֵ   add 2012.6.15
				}
				else if(jishucount>7)
				{
					Dsave0[j]=Dsave1[j];
					Dsave1[j]=Dsave2[j];
					Dsave2[j]=DDoseRata[j];
					DMean_Value[j]=(float)((Dsave0[j]+Dsave1[j]+DDoseRata[j])/3);
				}

	      	}
		 	else if(Flag_Meant == 0)
		  	{
				DMean_Value[j] = DDoseRata[j];
		  	}
		}							//�ڰ˸�̽ͷֻ�е�����
		/*�����ݼ�����ֵ�������õ�̽ͷ�źŸı��־ִ��*/
		Signal1 = Var_Signal1;                                        //̽ͷ�ź�ת��
		Signal2 = Var_Signal2;
		Signal3 = Var_Signal3;
		/**********��λ�����Ϊjtemp************/
		if(GMean_Value[j] < 0)	                                      //�޸�2012.6.15 ͨ����ֵ����
		{
		    Gjtemp[j] = 0.0;
		}
		else if(GMean_Value[j] >= 0)										//Mean_Valueƽ��ֵ��λΪuGy/h
		{
			if(GMean_Value[j] < 1000)									//Mean_Value<1000��ʾ��λΪuGy/h
			{
			    Gjtemp[j] = (float)GMean_Value[j];
				GFlag_dw[j] = 1;
			}
			else if((GMean_Value[j] >= 1000)&&(GMean_Value[j] < 1000000))	//1000<Mean_Value<1000000��ʾmGy/h
			{
			    Gjtemp[j] = (float)(GMean_Value[j]/1000);
				GFlag_dw[j] = 2;
			}
		    else if(GMean_Value[j] >= 1000000)							//Mean_Value>=1000000��ʾGy/h
			{
			    Gjtemp[j] = (float)(GMean_Value[j]/1000000);
				GFlag_dw[j] = 3;
			}
		}
		if(DMean_Value[j] < 0)	                                      //�޸�2012.6.15 ͨ����ֵ����
		{
		    Djtemp[j] = 0.0;
		}
		else if(DMean_Value[j] >= 0)										//Mean_Valueƽ��ֵ��λΪuGy/h
		{
			if(DMean_Value[j] < 1000)									//Mean_Value<1000��ʾ��λΪuGy/h
			{
			    Djtemp[j] = (float)DMean_Value[j];
				DFlag_dw[j] = 1;
			}
			else if((DMean_Value[j] >= 1000)&&(DMean_Value[j] < 1000000))	//1000<Mean_Value<1000000��ʾmGy/h
			{
			    Djtemp[j] = (float)(DMean_Value[j]/1000);
				DFlag_dw[j] = 2;
			}
		    else if(DMean_Value[j] >= 1000000)							//Mean_Value>=1000000��ʾGy/h
			{
			    Djtemp[j] = (float)(DMean_Value[j]/1000000);
				DFlag_dw[j] = 3;
			}
		}
        /***********�ж���ֵ************ע���������ֵѡ����һ��̽ͷ�ļ���ֵ��Ϊ�Ƚ�?��ʱ�ȱ�ע��*/
        mtemp=(YuThouth[j]+YuCent[j]*0.1+YuTenth[j]*0.01);	    //��ʱû����λ���޸�;��ֵ���㻹ԭ
	    yu=(double)(pow(10,YuGe[j]));
  	    yudata=(double)(mtemp*yu);
		yudecide=Gjtemp[j]*TrueRate[j]+Djtemp[j]*((1-TrueRate[j])/(1+TrueRate[j]));
		if(yudecide >= yudata)                  //�����������ĳһ����Χ������������
		 {
			if((Judge_Speak==0) ||(Speak_Alarm()==1))           //������������Speak_Alarm����1����������
			{
				 Flag_Warn=1;									 //������־��һ
			}
			Flag_need_Flash[j] = 1;								 //LED��˸��־��1
	        State_Flash[j]=1;
			Var_Led=Var_Led & Svar0[j];				//LED����ָʾ�ƣ�char��Svar0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}
		 }
		else if(yudecide < yudata)
		 {
		    Flag_need_Flash[j] = 0;
	        State_Flash[j]=0;
            Var_Led=Var_Led | Svar1[j];
		 }
	}			//forѭ����8��̽ͷ����ʾִ����
	for(j=0;j<15;j++)									//��ʾѭ��
	{
		Tbcd[5]=0;										//uchar���鹲12λ����õ����ݵ�bcd������
		Tbcd[4]=0;
		Tbcd[3]=0;
		Tbcd[2]=0;
		Tbcd[1]=0;
		Tbcd[0]=0;
		if(j<7)											//ǰ�߸������ܵ���ʾ
		{
			 /***********������ת����λ��ת���ɿ�����ʾ��BCD��*************2015��10��20����ʱ�޸�,����ʾ����ת���������for1->8ѭ��֮��*/
			 if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  �ƺ����࣬�ɲ������޸ĵ�
			 {
			 Tbcd[5] =(uchar)(((int) Gjtemp[j])/100);           //��λ
			 Tbcd[4] =(uchar)(((int) Gjtemp[j]) % 100 / 10);    //ʮλ
			 Tbcd[3] =(uchar)(((int) Gjtemp[j]) % 100 % 10);    //��λ
			 count_temp =(Gjtemp[j] - (int)Gjtemp[j])*1000;			//����С��1�Ĳ��֣���С�����ֱ�󣬱�����λС��
			 Tbcd[2] =(uchar)(count_temp/100);				//СС��ʮ��λ
			 Tbcd[1] =(uchar)(count_temp%100/10);			//С���ٷ�λ
			 Tbcd[0] =(uchar)(count_temp%100%10);			//С��ǧ��λ
			 //if(Flag_dw == 1)								//����Tbcd[]����yudecide(float)�����жϼ������Ƿ񳬹���ֵ
			//	 yudecide=(float)(Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001);
			 //if(Flag_dw == 2)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000);
			 //if(Flag_dw == 3)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000000);
			 }
			/********��������ʾ**********/
			Txtext(10,13+(j*58),"����");				//��ʾ̽ͷ
			Tnumber(110,13+(j*58),j+1);					//��ʾ̽ͷ��
			Txtext(140,13+(j*58),":");					//��ʾð��
			if(Tbcd[5] != 0)							//����λ���㣬�������Ĵ�С����ʾ
			{
				Tnumber(180,13+(j*58),Tbcd[5]);
				Tnumber(210,13+(j*58),Tbcd[4]);
				Tnumber(240,13+(j*58),Tbcd[3]);
				Txtext(270,13+(j*58),".");
				Tnumber(290,13+(j*58),Tbcd[2]);
				Tnumber(320,13+(j*58),Tbcd[1]);
				Tnumber(350,13+(j*58),Tbcd[0]);
				if(Tdata[j]<10)							//������ʾTdata����ļ���ֵ��ʾ
					Tnumber(576,13+(j*58),Tdata[j]);	     //������ɺ�ȥ��	2012.6.24
				else if((Tdata[j]>=10)&&(Tdata[j]<100))
					Tnumber(556,13+(j*58),Tdata[j]);
				else if((Tdata[j]>=100)&&(Tdata[j]<1000))
					Tnumber(536,13+(j*58),Tdata[j]);
				else if((Tdata[j]>=1000)&&(Tdata[j]<10000))
					Tnumber(516,13+(j*58),Tdata[j]);
				else if(Tdata[j]>=10000)
					Tnumber(496,13+(j*58),Tdata[j]);
			}
			else
			{
				if(Tbcd[4] != 0)						//��λΪ��ʮλ����
				{

					Tnumber(210,13+(j*58),Tbcd[4]);
					Tnumber(240,13+(j*58),Tbcd[3]);
					Txtext(270,13+(j*58),".");
					Tnumber(290,13+(j*58),Tbcd[2]);
					Tnumber(320,13+(j*58),Tbcd[1]);
					Tnumber(350,13+(j*58),Tbcd[0]);
					if(Tdata[j]<10)
						Tnumber(576,13+(j*58),Tdata[j]);	     //������ɺ�ȥ��	2012.6.24
					else if((Tdata[j]>=10)&&(Tdata[j]<100))
						Tnumber(556,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=100)&&(Tdata[j]<1000))
						Tnumber(536,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=1000)&&(Tdata[j]<10000))
						Tnumber(516,13+(j*58),Tdata[j]);
					else if(Tdata[j]>=10000)
						Tnumber(496,13+(j*58),Tdata[j]);	 //������ɺ�ȥ��	 2012.6.24
				 }
				 else									//��λʮλλ��Ϊ�㣬��λ����
				 {
					Tnumber(240,13+(j*58),Tbcd[3]);
					Txtext(270,13+(j*58),".");				//.ռ20������
					Tnumber(290,13+(j*58),Tbcd[2]);			//һ������ռ30������
					Tnumber(320,13+(j*58),Tbcd[1]);
					Tnumber(350,13+(j*58),Tbcd[0]);
					if(Tdata[j]<10)
						Tnumber(576,13+(j*58),Tdata[j]);	     //������ɺ�ȥ��	2012.6.24
					else if((Tdata[j]>=10)&&(Tdata[j]<100))		//���Կ���һ������ռ20����
						Tnumber(556,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=100)&&(Tdata[j]<1000))
						Tnumber(536,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=1000)&&(Tdata[j]<10000))
						Tnumber(516,13+(j*58),Tdata[j]);
					else if(Tdata[j]>=10000)
						Tnumber(496,13+(j*58),Tdata[j]);

				 }
			}
		   /**********������λ��ʾ***********/
			if(GFlag_dw[j]==1)					      //������ɺ����  2012.6.24
			{
				Txtext(390,13+(j*58),"uGy");
			}
			else  if(GFlag_dw[j]==2)
			{
				Txtext(390,13+(j*58),"mGy");
			}
			else if(GFlag_dw[j]==3)
			{
				Txtext(390,13+(j*58),"Gy");
			}
			Alarm();                           //����
			send_buf[8*j]=Tbcd[5];				//��NIM-B������ʾ����
			send_buf[8*j+1]=Tbcd[4];
			send_buf[8*j+2]=Tbcd[3];
			send_buf[8*j+3]=Tbcd[2];
			send_buf[8*j+4]=Tbcd[1];
			send_buf[8*j+5]=Tbcd[0];
			send_buf[8*j+6]=Flag_dw;			//���͵�λ��־
			send_buf[8*j+7]=Flag_need_Flash[j];	//LED��˸��־
		}
		else
		{
			shortdelay(5000);
			Lcd_Clear();
			Tbcd[5]=0;										//uchar���鹲12λ����õ����ݵ�bcd������
			Tbcd[4]=0;
			Tbcd[3]=0;
			Tbcd[2]=0;
			Tbcd[1]=0;
			Tbcd[0]=0;
			/***********������ת����λ��ת���ɿ�����ʾ��BCD��*************2015��10��20����ʱ�޸�,����ʾ����ת���������for1->8ѭ��֮��*/
			 if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  �ƺ����࣬�ɲ������޸ĵ�
			 {
			 Tbcd[5] =(uchar)(((int) Djtemp[j-7])/100);           //��λ
			 Tbcd[4] =(uchar)(((int) Djtemp[j-7]) % 100 / 10);    //ʮλ
			 Tbcd[3] =(uchar)(((int) Djtemp[j-7]) % 100 % 10);    //��λ
			 count_temp =(Djtemp[j-7] - (int)Djtemp[j-7])*1000;			//����С��1�Ĳ��֣���С�����ֱ�󣬱�����λС��
			 Tbcd[2] =(uchar)(count_temp/100);				//СС��ʮ��λ
			 Tbcd[1] =(uchar)(count_temp%100/10);			//С���ٷ�λ
			 Tbcd[0] =(uchar)(count_temp%100%10);			//С��ǧ��λ
			 //if(Flag_dw == 1)								//����Tbcd[]����yudecide(float)�����жϼ������Ƿ񳬹���ֵ
			//	 yudecide=(float)(Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001);
			 //if(Flag_dw == 2)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000);
			 //if(Flag_dw == 3)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000000);
			 }
			/********��������ʾ**********/
			Txtext(10,13+((j-7)*58),"����");				//��ʾ̽ͷ
			Tnumber(110,13+((j-7)*58),(j-7)+1);					//��ʾ̽ͷ��
			Txtext(140,13+((j-7)*58),":");					//��ʾð��
			if(Tbcd[5] != 0)							//����λ���㣬�������Ĵ�С����ʾ
			{
				Tnumber(180,13+((j-7)*58),Tbcd[5]);
				Tnumber(210,13+((j-7)*58),Tbcd[4]);
				Tnumber(240,13+((j-7)*58),Tbcd[3]);
				Txtext(270,13+((j-7)*58),".");
				Tnumber(290,13+((j-7)*58),Tbcd[2]);
				Tnumber(320,13+((j-7)*58),Tbcd[1]);
				Tnumber(350,13+((j-7)*58),Tbcd[0]);
				if(Ddata[j-7]<10)							//������ʾTdata����ļ���ֵ��ʾ
					Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //������ɺ�ȥ��	2012.6.24
				else if((Ddata[j-7]>=10)&&(Ddata[j-7]<100))
					Tnumber(556,13+((j-7)*58),Ddata[j-7]);
				else if((Ddata[j-7]>=100)&&(Ddata[j-7]<1000))
					Tnumber(536,13+((j-7)*58),Ddata[j-7]);
				else if((Ddata[j-7]>=1000)&&(Ddata[j-7]<10000))
					Tnumber(516,13+((j-7)*58),Ddata[j-7]);
				else if(Ddata[j-7]>=10000)
					Tnumber(496,13+((j-7)*58),Ddata[j-7]);
			}
			else
			{
				if(Tbcd[4] != 0)						//��λΪ��ʮλ����
				{

					Tnumber(210,13+((j-7)*58),Tbcd[4]);
					Tnumber(240,13+((j-7)*58),Tbcd[3]);
					Txtext(270,13+((j-7)*58),".");
					Tnumber(290,13+((j-7)*58),Tbcd[2]);
					Tnumber(320,13+((j-7)*58),Tbcd[1]);
					Tnumber(350,13+((j-7)*58),Tbcd[0]);
					if(Ddata[j-7]<10)
						Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //������ɺ�ȥ��	2012.6.24
					else if((Ddata[j-7]>=10)&&(Ddata[j-7]<100))
						Tnumber(556,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=100)&&(Ddata[j-7]<1000))
						Tnumber(536,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=1000)&&(Ddata[j-7]<10000))
						Tnumber(516,13+((j-7)*58),Ddata[j-7]);
					else if(Ddata[j-7]>=10000)
						Tnumber(496,13+((j-7)*58),Ddata[j-7]);	 //������ɺ�ȥ��	 2012.6.24
				 }
				 else									//��λʮλλ��Ϊ�㣬��λ����
				 {
					Tnumber(240,13+((j-7)*58),Tbcd[3]);
					Txtext(270,13+((j-7)*58),".");				//.ռ20������
					Tnumber(290,13+((j-7)*58),Tbcd[2]);			//һ������ռ30������
					Tnumber(320,13+((j-7)*58),Tbcd[1]);
					Tnumber(350,13+((j-7)*58),Tbcd[0]);
					if(Ddata[j-7]<10)
						Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //������ɺ�ȥ��	2012.6.24
					else if((Ddata[j-7]>=10)&&(Ddata[j-7]<100))		//���Կ���һ������ռ20����
						Tnumber(556,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=100)&&(Ddata[j-7]<1000))
						Tnumber(536,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=1000)&&(Ddata[j-7]<10000))
						Tnumber(516,13+((j-7)*58),Ddata[j-7]);
					else if(Ddata[j-7]>=10000)
						Tnumber(496,13+((j-7)*58),Ddata[j-7]);

				 }
			}
		   /**********������λ��ʾ***********/
			if(DFlag_dw[j-7]==1)					      //������ɺ����  2012.6.24
			{
				Txtext(390,13+((j-7)*58),"uGy");
			}
			else  if(DFlag_dw[j-7]==2)
			{
				Txtext(390,13+((j-7)*58),"mGy");
			}
			else if(DFlag_dw[j-7]==3)
			{
				Txtext(390,13+((j-7)*58),"Gy");
			}
			Alarm();                           //����
			send_buf[8*j]=Tbcd[5];				//��NIM-B������ʾ����
			send_buf[8*j+1]=Tbcd[4];
			send_buf[8*j+2]=Tbcd[3];
			send_buf[8*j+3]=Tbcd[2];
			send_buf[8*j+4]=Tbcd[1];
			send_buf[8*j+5]=Tbcd[0];
			send_buf[8*j+6]=Flag_dw;			//���͵�λ��־
			send_buf[8*j+7]=Flag_need_Flash[j];	//LED��˸��־
		}
	  Flag_Warn_Count=1;
	  jishucount++;							//�����������ӣ���ƽ��ֵʱ�ã�
	  if(jishucount==255)					//����������Ϊ��char�ͱ��������֧��255��������Ҫ����
	      jishucount=8;
	  if(jishucount>4)
	      Flag_Meant = 1;//ǰ��ε�ȡֵ��Ҫ������βſ�ʼҪ����Flag_Meant=1���������0
	  else
	      Flag_Meant = 0;
	  shortdelay(5000);						//��ʱ
	  //for(i=0;i<33;i++)	//��ʾ����0
	       //buf[i]=0;
}						//������־
else
{
        jishuguan_data=0;                   //need think  whether need?  4.21,int���������ݣ�
		dianlishi_data=0;					//����������
		jishuguan_rata=0;					//�����ܼ�����
		jishuguan_jtemp=0;					//�����ܼ���ֵ(��λת����)
		dianlishi_jtemp=0;					//�����Ҽ���ֵ����λת����
		Tbcd[11]=0;
		Tbcd[10]=0;
		Tbcd[9]=0;
		Tbcd[8]=0;
		Tbcd[7]=0;
		Tbcd[6]=0;
		Tbcd[5]=0;
		Tbcd[4]=0;
		Tbcd[3]=0;
		Tbcd[2]=0;
		Tbcd[1]=0;
		Tbcd[0]=0;
		j=10*biaoding_input-7;						//biaodin_inputΪҪ�궨��̽ͷ���
		jishuguan_data = buf[4*biaoding_input]*256+buf[4*biaoding_input-1];		//�൱��Tdata
		jishuguan_rata=jishuguan_data*12;										//�����ʣ�ֱ�ӳ���12?
		dianlishi_data = buf[4*biaoding_input+1]*256+buf[4*biaoding_input+2];	//������Tdata
		Para[0] = (float)(DataThouth[j]*1000+DataCent[j]*100+DataTenth[j]*10+DataGe[j]); //���ڲ���
	    Para[1] = (float)((float)(DataThouth[j+1]*1000+DataCent[j+1]*100+DataTenth[j+1]*10+DataGe[j+1])/1000000);
	    jishuguan_DoseRata = (float)(Para[1]*(jishuguan_rata-Para[0]));
		if(jishuguan_DoseRata<0)
			jishuguan_DoseRata=0.0;
		jishuguan_jtemp = (float)jishuguan_DoseRata;
		if((Tbcd[11]<=9)&&(Tbcd[10]<=9)&&(Tbcd[9]<=9)&&(Tbcd[8]<=9)&&(Tbcd[7]<=9)&&(Tbcd[6]<=9))   //4.21  �ƺ����࣬�ɲ������޸ĵ�
		{

			Tbcd[11] =(uchar)(((int) jishuguan_jtemp)/100);           //ǧλ
			Tbcd[10] =(uchar)(((int) jishuguan_jtemp) % 100 / 10);    //��λ
			Tbcd[9] =(uchar)(((int) jishuguan_jtemp) % 100 % 10);    //��λ
			jishuguan_count_temp =(jishuguan_jtemp - (int)jishuguan_jtemp)*1000;
			Tbcd[8] =(uchar)(jishuguan_count_temp/100);
			Tbcd[7] =(uchar)(jishuguan_count_temp%100/10);
			Tbcd[6] =(uchar)(jishuguan_count_temp%100%10);
		}
		Para[2] = (float)(DataThouth[j+2]*1000+DataCent[j+2]*100+DataTenth[j+2]*10+DataGe[j+2]); //���ڲ���
	    Para[3] = (float)((float)(DataThouth[j+3]*1000+DataCent[j+3]*100+DataTenth[j+3]*10+DataGe[j+3])/1000);
		dianlishi_DoseRata = (float)(Para[3]*(dianlishi_data-Para[2]));
		if(dianlishi_DoseRata<0)
			dianlishi_DoseRata=0.0;
		dianlishi_jtemp = (float)(dianlishi_DoseRata/1000);
		if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  �ƺ����࣬�ɲ������޸ĵ�
		{

			Tbcd[5] =(uchar)(((int) dianlishi_jtemp)/100);           //ǧλ
			Tbcd[4] =(uchar)(((int) dianlishi_jtemp) % 100 / 10);    //��λ
			Tbcd[3] =(uchar)(((int) dianlishi_jtemp) % 100 % 10);    //��λ
			dianlishi_count_temp =(dianlishi_jtemp - (int)dianlishi_jtemp)*1000;
			Tbcd[2] =(uchar)(dianlishi_count_temp/100);
			Tbcd[1] =(uchar)(dianlishi_count_temp%100/10);
			Tbcd[0] =(uchar)(dianlishi_count_temp%100%10);
		}
		Txtext(200,58,"�궨̽ͷ");
		Tnumber(400,58,biaoding_input);
		Txtext(10,116,"�Ǹ��");;
		Txtext(160,116,":");
		Txtext(220,174,"uGy/h");
		Txtext(520,174,"CPM");
		Txtext(10,290,"������");;
		Txtext(160,290,":");
		Txtext(220,348,"mGy/h");
		Txtext(520,348,"CP5S");
		if(Tbcd[11] != 0)
		{
			Tnumber(10,174,Tbcd[11]);
			Tnumber(40,174,Tbcd[10]);
            Tnumber(70,174,Tbcd[9]);
	    	Txtext(100,174,".");
	    	Tnumber(130,174,Tbcd[8]);
		    Tnumber(160,174,Tbcd[7]);
			Tnumber(190,174,Tbcd[6]);
			if(jishuguan_data<10)
			  	Tnumber(490,174,jishuguan_data);	     //������ɺ�ȥ��	2012.6.24
			else if((jishuguan_data>=10)&&(jishuguan_data<100))
				Tnumber(460,174,jishuguan_data);
			else if((jishuguan_data>=100)&&(jishuguan_data<1000))
				Tnumber(430,174,jishuguan_data);
			else if((jishuguan_data>=1000)&&(jishuguan_data<10000))
				Tnumber(400,174,jishuguan_data);
			else if(jishuguan_data>=10000)
				Tnumber(370,174,jishuguan_data);
		}
		else
		{
			if(Tbcd[10] != 0)
			{

				Tnumber(40,174,Tbcd[10]);
            	Tnumber(70,174,Tbcd[9]);
	    		Txtext(100,174,".");
	    		Tnumber(130,174,Tbcd[8]);
		    	Tnumber(160,174,Tbcd[7]);
				Tnumber(190,174,Tbcd[6]);
				if(jishuguan_data<10)
			  		Tnumber(490,174,jishuguan_data);	     //������ɺ�ȥ��	2012.6.24
				else if((jishuguan_data>=10)&&(jishuguan_data<100))
					Tnumber(460,174,jishuguan_data);
				else if((jishuguan_data>=100)&&(jishuguan_data<1000))
					Tnumber(430,174,jishuguan_data);
				else if((jishuguan_data>=1000)&&(jishuguan_data<10000))
					Tnumber(400,174,jishuguan_data);
				else if(jishuguan_data>=10000)
					Tnumber(370,174,jishuguan_data);
			 }
			 else
			 {
            	Tnumber(70,174,Tbcd[9]);
	    		Txtext(100,174,".");
	    		Tnumber(130,174,Tbcd[8]);
		    	Tnumber(160,174,Tbcd[7]);
				Tnumber(190,174,Tbcd[6]);
				if(jishuguan_data<10)
			  		Tnumber(490,174,jishuguan_data);	     //������ɺ�ȥ��	2012.6.24
				else if((jishuguan_data>=10)&&(jishuguan_data<100))
					Tnumber(460,174,jishuguan_data);
				else if((jishuguan_data>=100)&&(jishuguan_data<1000))
					Tnumber(430,174,jishuguan_data);
				else if((jishuguan_data>=1000)&&(jishuguan_data<10000))
					Tnumber(400,174,jishuguan_data);
				else if(jishuguan_data>=10000)
					Tnumber(370,174,jishuguan_data);

			 }
		}
		if(Tbcd[5] != 0)
		{
			Tnumber(10,348,Tbcd[5]);
			Tnumber(40,348,Tbcd[4]);
            Tnumber(70,348,Tbcd[3]);
	    	Txtext(100,348,".");
	    	Tnumber(130,348,Tbcd[2]);
		    Tnumber(160,348,Tbcd[1]);
			Tnumber(190,348,Tbcd[0]);
			if(dianlishi_data<10)
			  	Tnumber(490,348,dianlishi_data);	     //������ɺ�ȥ��	2012.6.24
			else if((dianlishi_data>=10)&&(dianlishi_data<100))
				Tnumber(460,348,dianlishi_data);
			else if((dianlishi_data>=100)&&(dianlishi_data<1000))
				Tnumber(430,348,dianlishi_data);
			else if((dianlishi_data>=1000)&&(dianlishi_data<10000))
				Tnumber(400,348,dianlishi_data);
			else if(dianlishi_data>=10000)
				Tnumber(370,348,dianlishi_data);
		}
		else
		{
			if(Tbcd[4] != 0)
			{
				Tnumber(40,348,Tbcd[4]);
            	Tnumber(70,348,Tbcd[3]);
	    		Txtext(100,348,".");
	    		Tnumber(130,348,Tbcd[2]);
		    	Tnumber(160,348,Tbcd[1]);
				Tnumber(190,348,Tbcd[0]);
				if(dianlishi_data<10)
			  		Tnumber(490,348,dianlishi_data);	     //������ɺ�ȥ��	2012.6.24
				else if((dianlishi_data>=10)&&(dianlishi_data<100))
					Tnumber(460,348,dianlishi_data);
				else if((dianlishi_data>=100)&&(dianlishi_data<1000))
					Tnumber(430,348,dianlishi_data);
				else if((dianlishi_data>=1000)&&(dianlishi_data<10000))
					Tnumber(400,348,dianlishi_data);
				else if(dianlishi_data>=10000)
					Tnumber(370,348,dianlishi_data);
			 }
			 else
			 {
            	Tnumber(70,348,Tbcd[3]);
	    		Txtext(100,348,".");
	    		Tnumber(130,348,Tbcd[2]);
		    	Tnumber(160,348,Tbcd[1]);
				Tnumber(190,348,Tbcd[0]);
				if(dianlishi_data<10)
			  		Tnumber(490,348,dianlishi_data);	     //������ɺ�ȥ��	2012.6.24
				else if((dianlishi_data>=10)&&(dianlishi_data<100))
					Tnumber(460,348,dianlishi_data);
				else if((dianlishi_data>=100)&&(dianlishi_data<1000))
					Tnumber(430,348,dianlishi_data);
				else if((dianlishi_data>=1000)&&(dianlishi_data<10000))
					Tnumber(400,348,dianlishi_data);
				else if(dianlishi_data>=10000)
					Tnumber(370,348,dianlishi_data);
			 }
		}

}

 }

/************************************
*           ����ʱ
************************************/
void shortdelay(uint i)
{
   uint k;
   uint n;
   for(k=0;k<i;k++)
	   for(n=200;n>0;n--);
}

/********************************
*        ָʾ�ƺͱ���
********************************/
void Alarm()
{
	uchar n;
    for(n=0;n<10;n++)
    {
	     Led573=Var_Led;
		 if(Flag_Warn==1)                   //��������
			{
		  	  Speak=1;
			 }
   }
}

/*******************************
*       ����ָʾ��״̬
*******************************/
void Updata_Flash(uchar j)
{
	if(Flag_need_Flash[j])
	{
	    if(State_Flash[j]==1)
		{
			State_Flash[j]=0;
		}
		else
		{
			State_Flash[j]=1;
		}
	}
	else
			State_Flash[j]=0;

}

/***********************************
*          ָʾ����˸
************************************/
void Led_Flash(void)
{
    uchar i;
	for(i=0;i<8;i++)
	{
		Updata_Flash(i);                               //״̬ȡ��
        Led_Disp(i,Flag_need_Flash[i],State_Flash[i]); //��״̬��ʾ
	}
}

/************************************
*         ָʾ��״̬��ʾ
*************************************/
void Led_Disp(uchar Num,uchar Flag,uchar State)
{
	if(Flag)
	{
		if(State)
		{
			Var_Led=Var_Led & Svar0[Num];            //ָʾ����
			Led573=Var_Led;
			if(Flag_Warn==1)                        //��Ҫ����������
			{
				Speak=0;                            //��������
				shortdelay(100);
			}
			return;
		 }
	}
	Var_Led=Var_Led | Svar1[Num];                   //ָʾ����
	Led573=Var_Led;
	if(Flag_Warn==1)                                //��Ҫ����������
    {
		Speak=1;                                    //����������
		shortdelay(100);
		}
}
