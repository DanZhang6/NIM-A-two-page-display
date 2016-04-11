/********************************************************************************************
*描述：
*      8253计数设置
*功能：
*    1. 从8253中读取计数，每个8253中含有3个计数器，五个8253计15个探头的计数，
*       前七个探头是双探头（低计数探头和高计数探头），第八个探头是单探头（高计数探头），
*       根据计数率选择哪个探头工作；
*    2. T0定时中断
*       T0工作在16位自动重载模式，向上计数，每循环一次，总定时5Ms, 计数初值DC00，晶振22.1184M；
*    3. 定时到，读取计数，根据设定的参数，显示各探头的计数率；
*    4. 根据计数率的不同，选择不同的单位（uGy/s,mGy/s,Gy/s）;

***********************************************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h"
#include "absacc.h"
#include "stdio.h"
#include "math.h"

#define  C82531C  XBYTE[0x8300]	//8253的命令端口（地址）， CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=11；
#define  C825310D XBYTE[0x8000] //计数器0				   CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=00；
#define  C825311D XBYTE[0x8100]	//计数器1                  CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=01;
#define  C825312D XBYTE[0x8200]	//计数器2				   CS4=1,CS3=1,CS2=1,CS1=1,CS0=0,A1A0=10;

#define  C82532C  XBYTE[0x8b00]	//XBYTE的作用是将外部I/O端口置为中括号内部的16位二进制，即为设置
#define  C825320D XBYTE[0x8800]	//使能端，XBYTE的作用还有设置使能端后，自动设置wr,rd.
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

uchar  buf[33];                      //从8253中读到的计数器数据，15个计数器，低八位高八位，从3-32，1为探头个数，2为测量时间
uchar  DataGe[75];                   //探测器标定参数的个位，初始时个十百千均为0
uchar  DataTenth[75];                //探测器标定参数的十分位
uchar  DataCent[75];                 //探测器标定参数的百分位
uchar  DataThouth[75];               //探测器标定参数的千分位
uchar  send_buf[65];                 //NIM_A向NIM_B发送数据的数组
uchar  Incinput;                     //探头个数标志，默认显示探头个数为8个
float  idata Para[10];               //设定好的参数数组
float  GDoseRata[8];                  //每组探头测得的剂量率,GM计数管的剂量率(2015年10月20日改)
float  DDoseRata[8];					//电离室的计算剂量率(2015年10月20日加)
float  DoseRata[8];						//当前探头用来判断的剂量值
float  TrueRate[8];						//当前探头准确剂量值所在的探头,为一表示GM计数管
float jishuguan_DoseRata;
float dianlishi_DoseRata;
float  GMean_Value[8];				//平均值GM计数管平均值
float  DMean_Value[8];				//电离室平均值
float  idata Gsave0[8],Gsave1[8],Gsave2[8];//计数管前5次DoseRata数据不要，save0,1,2用来记录前面3次的DoseRata(只平均三次的值)，来平滑计算平均值!
float  idata Dsave0[8],Dsave1[8],Dsave2[8];//电离室


uchar  GFlag_dw[8];                      //GM计数管单位标志
uchar  DFlag_dw[8];						//电离室单位标识
uint  idata Tdata[8];                   //探头计数率(2015年10月20日让Tdata为计数管的计数值)
uint  idata Ddata[8];                   //探头计数率(2015年10月20日让Ddata为电离室的计数值)
uint idata jishuguan_data;				//标定时的计数管计数值相当于Tdata
uint idata dianlishi_data;				//标定时的电离室计数值相当于Tdata
uchar idata Var_Signal1;             //探头控制信号1，接计数管，默认ff均接计数管
uchar idata Var_Signal2;             //探头控制信号2，接电离室，默认00；
uchar idata Var_Signal3;             //探头控制信号3，接电离室，默认00；
uchar idata Var_Led;                 //LED报警指示灯,为1时灭,初值0xff,
uchar biaoding_input;
bit Flag_Warn_Count;				//默认0；
uchar Flag_need_Flash[8];			//LED闪烁标志
uchar State_Flash[8];
uchar count_change_flag[8];			//计数改变标志？

uchar data Svar1[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};     //信号数组
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
*           8253计数值
**************************************/
void GetAndDisdata()                               //刷新显示子程序
{
	uchar th=0;										//8253高8位
	uchar tl=0;										//8253低八位
	uchar i,j;                                       //buf数组初始化
	for(i=0;i<33;i++)
	{
		buf[i]=0;
	}
    for(j=0;j<10;j++)
	{
		Para[j]=0;
	}
	/*===========锁存8253的计数值==========*/
	C82531C=0x84;                                 //锁存第一片8253计数器2的计数值
	tl=C825312D;									 //read 8253 16 bits data(first L then H )
	buf[3]=0xff-tl;
	th=C825312D;
	buf[4]=0xff-th;

	C82531C=0x44;                                 //锁存第一片8253计数器1的计数值
	tl=C825311D;									 //read 8253 16 bits data(first L then H )
	buf[5]=0xff-tl;
	th=C825311D;
	buf[6]=0xff-th;

	C82531C=0x04;                                //锁存第一片8253计数器0的计数值
	tl=C825310D;									 //read 8253 16 bits data(first L then H )
	buf[7]=0xff-tl;
	th=C825310D;
	buf[8]=0xff-th;

	C82532C=0x84;                                //锁存第二片8253计数器2的计数值
	tl=C825322D;									 //read 8253 16 bits data(first L then H )
	buf[9]=0xff-tl;
	th=C825322D;
	buf[10]=0xff-th;

	C82532C=0x44;                                //锁存第二片8253计数器1的计数值
	tl=C825321D;									 //read 8253 16 bits data(first L then H )
	buf[11]=0xff-tl;
	th=C825321D;
	buf[12]=0xff-th;

	C82532C=0x04;                               //锁存第二片8253计数器0的计数值
	tl=C825320D;									 //read 8253 16 bits data(first L then H )
	buf[13]=0xff-tl;
	th=C825320D;
	buf[14]=0xff-th;

	C82533C=0x84;                                //锁存第三片8253计数器2的计数值
	tl=C825332D;									 //read 8253 16 bits data(first L then H )
	buf[15]=0xff-tl;
	th=C825332D;
	buf[16]=0xff-th;

	C82533C=0x44;                                //锁存第三片8253计数器1的计数值
	tl=C825331D;									 //read 8253 16 bits data(first L then H )
	buf[17]=0xff-tl;
	th=C825331D;
	buf[18]=0xff-th;

	C82533C=0x04;                               //锁存第三片8253计数器0的计数值
	tl=C825330D;									 //read 8253 16 bits data(first L then H )
	buf[19]=0xff-tl;
	th=C825330D;
	buf[20]=0xff-th;

	C82534C=0x84;                                //锁存第四片8253计数器2的计数值
	tl=C825342D;									 //read 8253 16 bits data(first L then H )
	buf[21]=0xff-tl;
	th=C825342D;
	buf[22]=0xff-th;

	C82534C=0x44;                                //锁存第四片8253计数器1的计数值
	tl=C825341D;									 //read 8253 16 bits data(first L then H )
	buf[23]=0xff-tl;
	th=C825341D;
	buf[24]=0xff-th;

	C82534C=0x04;                               //锁存第四片8253计数器0的计数值
	tl=C825340D;									 //read 8253 16 bits data(first L then H )
	buf[25]=0xff-tl;
	th=C825340D;
	buf[26]=0xff-th;

	C82535C=0x84;                                //锁存第五片8253计数器2的计数值
	tl=C825352D;									 //read 8253 16 bits data(first L then H )
	buf[27]=0xff-tl;
	th=C825352D;
	buf[28]=0xff-th;

	C82535C=0x44;                                //锁存第五片8253计数器1的计数值
	tl=C825351D;									 //read 8253 16 bits data(first L then H )
	buf[29]=0xff-tl;
	th=C825351D;
	buf[30]=0xff-th;

	C82535C=0x04;                               //锁存第五片8253计数器0的计数值
	tl=C825350D;									 //read 8253 16 bits data(first L then H )
	buf[31]=0xff-tl;
	th=C825350D;
	buf[32]=0xff-th;

	buf[1]=Incinput;                                 //探头个数
	buf[2]=Redrawtime;                               //测量时间（刷新时间标志）

}
/*******************************************
*        8253计数器初始化
********************************************/
 void Init_8253()
 {

//=======装入初值再开始计数=======//
	C82531C=0xb4;                               //10110100
	C825312D=0xff;                   			//10：通道2,11：先低字节后高字节，010：按方式2工作，0：二进制计数。写初值0xff后开始计数
	C825312D=0xff;

	C82531C=0x74;                               //01110100
	C825311D=0xff;                              //同上
	C825311D=0xff;

	C82531C=0x34;                               //00110100
	C825310D=0xff;                              //写初值 先低字节后高字节 82531写初值后就开始计数
	C825310D=0xff;								//二三行用来装入初值

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

	Var_Led=0xff;                               //led指示
	Led573=Var_Led;
	EX0=0;                                      //关外部中断0
 /*定时计数器的初始化*/
    GATE=1;                                     //8253开启计数
    Flag8253Cnting = 1;
	NumT0 = 0;
  }


/************************************
*           显示计数
************************************/
void ShowData()
{
	uchar m,j,Var1;//,i
	//uchar ii=2;
	uchar u;											//15年10月20日加
    ulong count_temp,jishuguan_count_temp,dianlishi_count_temp;
	float idata ptemp,rata,Drata,yudecide,zdata,jishuguan_rata,jishuguan_jtemp,dianlishi_jtemp;	//15年10月20日修改
	float Gjtemp[8];									//15年10月20日修改
	float Djtemp[8];									//15年10月20日修改
	double yu,yudata,mtemp;
	uchar Tbcd[12];	   									//测得的数据的bcd码数组
    Lcd_Clear();
if(measure_flag)										//(1)measure_flag在Init_RAM中初始化变为1
{
	for(u=0;u<8;u++)                   //变量初始化
	{
		Gjtemp[j]=0;
		Djtemp[j]=0;
		GFlag_dw[j]=0;                      //GM计数管单位标志
		DFlag_dw[j]=0;
		Ddata[j]=0;										//(2015年10月20日加)
		Tdata[j]=0;                                        //探头计数率uint,紫色为全局变量，蓝色为局部变量
	}
	for(j=0;j<Incinput;j++)                             //(2)存计数区为buf[3]-buf[32];探头个数标志
	{
		zdata=0;										//float计数管
		rata=0;											//float计数管计数率
		Drata=0;										//float电离室计数率(2015年10月20日加)
    ptemp=0;										//float
    yudata=0;										//double
		mtemp=0;										//double
		yu=0;											//double
		m=j*10;                           //保留
		//i=jishu%(Incinput*3);
		//isave=count%(Incinput*2);
    /*********判断计数率***********/
	  /*********前七个探头探测器设置，取得8253计数值记录在Tdata*/
    if(j<7)											//(3)前七个探头既有电离室也有计数器，第八个只有电离室故分开处理
		{
      Var1=(Var_Signal1 & Svar1[j]);              //(4)探头控制信号1,var-signal探头控制标志(全接计数管)；Svar1[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}
			if(Var1 == Svar1[j])						           //意思是当J号探头为计数管时，同时取出计数管和电离室的计数
			{
        Tdata[j] = buf[4*j+4]*256+buf[4*j+3];		//buf为锁存的8253计数器高八位低八位，共16位(当前标号探测器计数管的计数)
				Ddata[j] = buf[4*j+6]*256+buf[4*j+5];		//电离室计数(2015年10月20日加)
			}
			else if(Var1 == 0)							//(4)
			{
        Ddata[j] = buf[4*j+6]*256+buf[4*j+5];		//电离室计数(2015年10月20日改)
        Tdata[j] = buf[4*j+4]*256+buf[4*j+3];		//计数管计数(2015年10月20日加)
			  		Var_Signal1 = Var_Signal1 & Svar0[j];	//Svar0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}
			}
			/*通过刷新时间标志Redrawtime和从8253采集来的计数值Tdata计算探测器计数率,并且GM计数管与电离室计算计数率方法不同*/
			if(count_change_flag[j]==0)					//(4)计数改变标志，初始化count_change_flag均为0,若当前剂量值用GM计数管作为输出时为0
			{
				if(Redrawtime >= 60)					    //(5)修改2012.6.15 ，因为小于60秒与大于60s的计算方法不同，Redrawtime刷新时间标志
				{
					zdata = Redrawtime/60;
          rata = Tdata[j]/zdata; 					//计数管的计数率
          Drata = Ddata[j]/zdata;					//电离室的计数率(2015年10月20日加)
				}
				else if (Redrawtime < 60 )				   //修改2012.6.15 ，因为小于60秒与大于60s的计算方法不同
				{
					zdata =  600/Redrawtime;	          //修改2012.7.1 ，因为60秒除以40,50秒小数，会出现问题
                	rata =Tdata[j]*zdata/10;				  //修改2012.7.1
					Drata =Ddata[j]*zdata/10;					//电离室(2015年10月20日加)
				}
			}
			else if(count_change_flag[j]==1)				//在Dose_Rata大于1000时Count_Change_flag置1,即当当前剂量值用电离室为输出时为1
			{
				 rata=Tdata[j]*1.67;							//因为此时选用电离室作为输出,电离室的刷新时间标志Redrawtime变为了3;
				 Drata=Ddata[j]*1.67;
			}
			/*判断DoseRata值大小进行量程参数的切换，通过公式，探测器标定的参数(Para[])和
			rata(就是Nc实际的计数率)计算剂量率DoseRata（规定其单位为uGy/h）*/
			if(DoseRata[j] < 0.15)	              // 修改2012.6.15 && (DoseRata[j] >= 0)
			{
        Para[0] = (float)(DataThouth[m+1]*1000+DataCent[m+1]*100+DataTenth[m+1]*10+DataGe[m+1]); //调节参数
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
			  Var_Signal1 = Var_Signal1 | Svar1[j];                       //控制信号1接计数管，为高电平
			  Var_Signal2 = Var_Signal2 & Svar0[j];						//控制信号2接电离室10(8),低电平
			  Var_Signal3 = Var_Signal3 & Svar0[j];                       //控制信号2接电离室10(6),低电平
				count_change_flag[j]=0;
			}
			else if((DoseRata[j] >= 0.15) && (DoseRata[j] < 1000))             //当剂量率超过0.15uGy/h时，使用计数管的第二组参数
			{
			  Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //调节参数
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[4] = (float)(DataThouth[m+5]*1000+DataCent[m+5]*100+DataTenth[m+5]*10+DataGe[m+5]); //调节参数
        Para[5] = (float)((float)(DataThouth[m+6]*1000+DataCent[m+6]*100+DataTenth[m+6]*10+DataGe[m+6])/1000);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[5]*(Drata-Para[4]));
        DoseRata[j]=GDoseRata[j];
        TrueRate[j]=1;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar1[j];                   //控制信号1接计数管，为高电平
        Var_Signal2 = Var_Signal2 & Svar0[j];                   //控制信号2接电离室10(8),低电平
        Var_Signal3 = Var_Signal3 & Svar0[j];                   //控制信号2接电离室10(6),低电平
        count_change_flag[j]=0;
			}
      else if((DoseRata[j] >= 1000) && (DoseRata[j] < 500000))       //当剂量率超过1mGy/h时，使用电离室的第一个欧姆档(10(10))
		  {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //调节参数
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[4] = (float)(DataThouth[m+5]*1000+DataCent[m+5]*100+DataTenth[m+5]*10+DataGe[m+5]); //调节参数
        Para[5] = (float)((float)(DataThouth[m+6]*1000+DataCent[m+6]*100+DataTenth[m+6]*10+DataGe[m+6])/1000);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[5]*(Drata-Para[4]));
        DoseRata[j]=DDoseRata[j];
        TrueRate[j]=0;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar0[j];                    //控制信号1接计数管，高电平,GM计数管高压始终通电
        Var_Signal2 = Var_Signal2 & Svar0[j];                    //控制信号2接电离室10(8),低电平
        Var_Signal3 = Var_Signal3 & Svar0[j];	                  //控制信号2接电离室10(6),低电平
        count_change_flag[j]=1;
        Redrawtime = 3;
      }
      else if((DoseRata[j] >= 500000) && (DoseRata[j] < 1000000))   //当剂量率超过500mGy/h时，使用电离室的第一个欧姆档(10(8)),有待具体确定
      {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //调节参数
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[6] = (float)(DataThouth[m+7]*1000+DataCent[m+7]*100+DataTenth[m+7]*10+DataGe[m+7]); //调节参数
        Para[7] = (float)((float)(DataThouth[m+8]*1000+DataCent[m+8]*100+DataTenth[m+8]*10+DataGe[m+8])/10);
        GDoseRata[j] = (float)(Para[3]*(rata-Para[2]));
        DDoseRata[j] = (float)(Para[7]*(Drata-Para[6]));
        DoseRata[j]=DDoseRata[j];
        TrueRate[j]=0;
        if(GDoseRata[j]<0)
        GDoseRata[j]=0.0;
        if(DDoseRata[j]<0)
        DDoseRata[j]=0.0;
        Var_Signal1 = Var_Signal1 | Svar0[j];                   //控制信号1接计数管，低电平
        Var_Signal2 = Var_Signal2 | Svar1[j];                   //控制信号2接电离室10(8),高电平
        Var_Signal3 = Var_Signal3 & Svar0[j];                   //控制信号2接电离室10(6),低电平
        count_change_flag[j]=1;
        Redrawtime = 3;
      }
      else if(DoseRata[j] >= 1000000)                              //当剂量率超过1Gy/h时，使用电离室的第一个欧姆档(10(6)),有待具体确定
      {
        Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //调节参数
        Para[3] = (float)((float)(DataThouth[m+4]*1000+DataCent[m+4]*100+DataTenth[m+4]*10+DataGe[m+4])/1000000);
        Para[8] = (float)(DataThouth[m+9]*1000+DataCent[m+9]*100+DataTenth[m+9]*10+DataGe[m+9]); //调节参数
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
			/******从DoseRata取得即时值三次平滑取平均值存入Mean_Value******/
      if(Flag_Meant == 1) 										//前五次计数值不要,第六次以后的计数值才要,jishucount>4后Flag_Meant=1
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
				  GMean_Value[j]= (float)((Gsave0[j]+Gsave1[j]+GDoseRata[j])/3);	    //平滑求均值   add 2012.6.15
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
    }							//前七个有电离室也有计数器，且有多个欧姆档
    else if(j == 7)                        //NIM_A的第八个探头只有高压电离室，且只有一个欧姆档
    {
			Ddata[j] = buf[32]*256 + buf[31];
		    //compare[i]=Tdata;
			//if((compare[j]==compare[j+Incinput])&&(compare[j+Incinput]==compare[j+Incinput*2])&&(compare[j+Incinput*2]==compare[j]))
			// Tdata=0;
			if(Redrawtime >= 60)		       //修改2012.6.15 ，因为小于60秒与大于60s的计算方法不同
			{
				zdata = Redrawtime/60;
            	Drata = Ddata[j]/zdata;
			}
			else if (Redrawtime < 60)		   //修改2012.6.15 ，因为小于60秒与大于60s的计算方法不同
			{
				zdata =  Ddata[j]*60;
				Drata = zdata/Redrawtime;
			}
	   	    if(DoseRata[j] < 1000000)
			{
		        Para[0] = (float)(DataThouth[m+1]*1000+DataCent[m+1]*100+DataTenth[m+1]*10+DataGe[m+1]); //调节参数
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
			    Para[2] = (float)(DataThouth[m+3]*1000+DataCent[m+3]*100+DataTenth[m+3]*10+DataGe[m+3]); //调节参数
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
				    DMean_Value[j]= (float)((Dsave0[j]+Dsave1[j]+Dsave2[j])/3);	    //平滑求均值   add 2012.6.15
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
		}							//第八个探头只有电离室
		/*将根据剂量率值重新设置的探头信号改变标志执行*/
		Signal1 = Var_Signal1;                                        //探头信号转变
		Signal2 = Var_Signal2;
		Signal3 = Var_Signal3;
		/**********单位换算后为jtemp************/
		if(GMean_Value[j] < 0)	                                      //修改2012.6.15 通过均值计算
		{
		    Gjtemp[j] = 0.0;
		}
		else if(GMean_Value[j] >= 0)										//Mean_Value平均值单位为uGy/h
		{
			if(GMean_Value[j] < 1000)									//Mean_Value<1000表示单位为uGy/h
			{
			    Gjtemp[j] = (float)GMean_Value[j];
				GFlag_dw[j] = 1;
			}
			else if((GMean_Value[j] >= 1000)&&(GMean_Value[j] < 1000000))	//1000<Mean_Value<1000000表示mGy/h
			{
			    Gjtemp[j] = (float)(GMean_Value[j]/1000);
				GFlag_dw[j] = 2;
			}
		    else if(GMean_Value[j] >= 1000000)							//Mean_Value>=1000000表示Gy/h
			{
			    Gjtemp[j] = (float)(GMean_Value[j]/1000000);
				GFlag_dw[j] = 3;
			}
		}
		if(DMean_Value[j] < 0)	                                      //修改2012.6.15 通过均值计算
		{
		    Djtemp[j] = 0.0;
		}
		else if(DMean_Value[j] >= 0)										//Mean_Value平均值单位为uGy/h
		{
			if(DMean_Value[j] < 1000)									//Mean_Value<1000表示单位为uGy/h
			{
			    Djtemp[j] = (float)DMean_Value[j];
				DFlag_dw[j] = 1;
			}
			else if((DMean_Value[j] >= 1000)&&(DMean_Value[j] < 1000000))	//1000<Mean_Value<1000000表示mGy/h
			{
			    Djtemp[j] = (float)(DMean_Value[j]/1000);
				DFlag_dw[j] = 2;
			}
		    else if(DMean_Value[j] >= 1000000)							//Mean_Value>=1000000表示Gy/h
			{
			    Djtemp[j] = (float)(DMean_Value[j]/1000000);
				DFlag_dw[j] = 3;
			}
		}
        /***********判断阈值************注意这里的阈值选择哪一个探头的计量值作为比较?暂时先标注掉*/
        mtemp=(YuThouth[j]+YuCent[j]*0.1+YuTenth[j]*0.01);	    //暂时没根据位数修改;阈值计算还原
	    yu=(double)(pow(10,YuGe[j]));
  	    yudata=(double)(mtemp*yu);
		yudecide=Gjtemp[j]*TrueRate[j]+Djtemp[j]*((1-TrueRate[j])/(1+TrueRate[j]));
		if(yudecide >= yudata)                  //如果计数超过某一个范围，则声音报警
		 {
			if((Judge_Speak==0) ||(Speak_Alarm()==1))           //蜂鸣器报警，Speak_Alarm返回1蜂鸣器工作
			{
				 Flag_Warn=1;									 //报警标志置一
			}
			Flag_need_Flash[j] = 1;								 //LED闪烁标志置1
	        State_Flash[j]=1;
			Var_Led=Var_Led & Svar0[j];				//LED报警指示灯（char）Svar0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}
		 }
		else if(yudecide < yudata)
		 {
		    Flag_need_Flash[j] = 0;
	        State_Flash[j]=0;
            Var_Led=Var_Led | Svar1[j];
		 }
	}			//for循环中8个探头均显示执行完
	for(j=0;j<15;j++)									//显示循环
	{
		Tbcd[5]=0;										//uchar数组共12位，测得的数据的bcd码数组
		Tbcd[4]=0;
		Tbcd[3]=0;
		Tbcd[2]=0;
		Tbcd[1]=0;
		Tbcd[0]=0;
		if(j<7)											//前七个计数管的显示
		{
			 /***********剂量率转换单位后转换成可以显示的BCD码*************2015年10月20日暂时修改,把显示数据转换程序放在for1->8循环之外*/
			 if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  似乎多余，可不可以修改掉
			 {
			 Tbcd[5] =(uchar)(((int) Gjtemp[j])/100);           //百位
			 Tbcd[4] =(uchar)(((int) Gjtemp[j]) % 100 / 10);    //十位
			 Tbcd[3] =(uchar)(((int) Gjtemp[j]) % 100 % 10);    //个位
			 count_temp =(Gjtemp[j] - (int)Gjtemp[j])*1000;			//计算小于1的部分，把小数部分变大，保留三位小数
			 Tbcd[2] =(uchar)(count_temp/100);				//小小数十分位
			 Tbcd[1] =(uchar)(count_temp%100/10);			//小数百分位
			 Tbcd[0] =(uchar)(count_temp%100%10);			//小数千分位
			 //if(Flag_dw == 1)								//根据Tbcd[]计算yudecide(float)用来判断剂量率是否超过阈值
			//	 yudecide=(float)(Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001);
			 //if(Flag_dw == 2)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000);
			 //if(Flag_dw == 3)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000000);
			 }
			/********剂量率显示**********/
			Txtext(10,13+(j*58),"计数");				//显示探头
			Tnumber(110,13+(j*58),j+1);					//显示探头号
			Txtext(140,13+(j*58),":");					//显示冒号
			if(Tbcd[5] != 0)							//若百位非零，根据数的大小来显示
			{
				Tnumber(180,13+(j*58),Tbcd[5]);
				Tnumber(210,13+(j*58),Tbcd[4]);
				Tnumber(240,13+(j*58),Tbcd[3]);
				Txtext(270,13+(j*58),".");
				Tnumber(290,13+(j*58),Tbcd[2]);
				Tnumber(320,13+(j*58),Tbcd[1]);
				Tnumber(350,13+(j*58),Tbcd[0]);
				if(Tdata[j]<10)							//用于显示Tdata锁存的计数值显示
					Tnumber(576,13+(j*58),Tdata[j]);	     //测试完成后去掉	2012.6.24
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
				if(Tbcd[4] != 0)						//百位为零十位非零
				{

					Tnumber(210,13+(j*58),Tbcd[4]);
					Tnumber(240,13+(j*58),Tbcd[3]);
					Txtext(270,13+(j*58),".");
					Tnumber(290,13+(j*58),Tbcd[2]);
					Tnumber(320,13+(j*58),Tbcd[1]);
					Tnumber(350,13+(j*58),Tbcd[0]);
					if(Tdata[j]<10)
						Tnumber(576,13+(j*58),Tdata[j]);	     //测试完成后去掉	2012.6.24
					else if((Tdata[j]>=10)&&(Tdata[j]<100))
						Tnumber(556,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=100)&&(Tdata[j]<1000))
						Tnumber(536,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=1000)&&(Tdata[j]<10000))
						Tnumber(516,13+(j*58),Tdata[j]);
					else if(Tdata[j]>=10000)
						Tnumber(496,13+(j*58),Tdata[j]);	 //测试完成后去掉	 2012.6.24
				 }
				 else									//百位十位位均为零，个位非零
				 {
					Tnumber(240,13+(j*58),Tbcd[3]);
					Txtext(270,13+(j*58),".");				//.占20个像素
					Tnumber(290,13+(j*58),Tbcd[2]);			//一个数字占30个像素
					Tnumber(320,13+(j*58),Tbcd[1]);
					Tnumber(350,13+(j*58),Tbcd[0]);
					if(Tdata[j]<10)
						Tnumber(576,13+(j*58),Tdata[j]);	     //测试完成后去掉	2012.6.24
					else if((Tdata[j]>=10)&&(Tdata[j]<100))		//可以看见一个数字占20像素
						Tnumber(556,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=100)&&(Tdata[j]<1000))
						Tnumber(536,13+(j*58),Tdata[j]);
					else if((Tdata[j]>=1000)&&(Tdata[j]<10000))
						Tnumber(516,13+(j*58),Tdata[j]);
					else if(Tdata[j]>=10000)
						Tnumber(496,13+(j*58),Tdata[j]);

				 }
			}
		   /**********计量单位显示***********/
			if(GFlag_dw[j]==1)					      //测试完成后加上  2012.6.24
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
			Alarm();                           //报警
			send_buf[8*j]=Tbcd[5];				//向NIM-B发送显示数据
			send_buf[8*j+1]=Tbcd[4];
			send_buf[8*j+2]=Tbcd[3];
			send_buf[8*j+3]=Tbcd[2];
			send_buf[8*j+4]=Tbcd[1];
			send_buf[8*j+5]=Tbcd[0];
			send_buf[8*j+6]=Flag_dw;			//发送单位标志
			send_buf[8*j+7]=Flag_need_Flash[j];	//LED闪烁标志
		}
		else
		{
			shortdelay(5000);
			Lcd_Clear();
			Tbcd[5]=0;										//uchar数组共12位，测得的数据的bcd码数组
			Tbcd[4]=0;
			Tbcd[3]=0;
			Tbcd[2]=0;
			Tbcd[1]=0;
			Tbcd[0]=0;
			/***********剂量率转换单位后转换成可以显示的BCD码*************2015年10月20日暂时修改,把显示数据转换程序放在for1->8循环之外*/
			 if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  似乎多余，可不可以修改掉
			 {
			 Tbcd[5] =(uchar)(((int) Djtemp[j-7])/100);           //百位
			 Tbcd[4] =(uchar)(((int) Djtemp[j-7]) % 100 / 10);    //十位
			 Tbcd[3] =(uchar)(((int) Djtemp[j-7]) % 100 % 10);    //个位
			 count_temp =(Djtemp[j-7] - (int)Djtemp[j-7])*1000;			//计算小于1的部分，把小数部分变大，保留三位小数
			 Tbcd[2] =(uchar)(count_temp/100);				//小小数十分位
			 Tbcd[1] =(uchar)(count_temp%100/10);			//小数百分位
			 Tbcd[0] =(uchar)(count_temp%100%10);			//小数千分位
			 //if(Flag_dw == 1)								//根据Tbcd[]计算yudecide(float)用来判断剂量率是否超过阈值
			//	 yudecide=(float)(Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001);
			 //if(Flag_dw == 2)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000);
			 //if(Flag_dw == 3)
			//	 yudecide=(float)((Tbcd[5]*100+Tbcd[4]*10+Tbcd[3]+Tbcd[2]*0.1+Tbcd[1]*0.01+Tbcd[0]*0.001)*1000000);
			 }
			/********剂量率显示**********/
			Txtext(10,13+((j-7)*58),"计数");				//显示探头
			Tnumber(110,13+((j-7)*58),(j-7)+1);					//显示探头号
			Txtext(140,13+((j-7)*58),":");					//显示冒号
			if(Tbcd[5] != 0)							//若百位非零，根据数的大小来显示
			{
				Tnumber(180,13+((j-7)*58),Tbcd[5]);
				Tnumber(210,13+((j-7)*58),Tbcd[4]);
				Tnumber(240,13+((j-7)*58),Tbcd[3]);
				Txtext(270,13+((j-7)*58),".");
				Tnumber(290,13+((j-7)*58),Tbcd[2]);
				Tnumber(320,13+((j-7)*58),Tbcd[1]);
				Tnumber(350,13+((j-7)*58),Tbcd[0]);
				if(Ddata[j-7]<10)							//用于显示Tdata锁存的计数值显示
					Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //测试完成后去掉	2012.6.24
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
				if(Tbcd[4] != 0)						//百位为零十位非零
				{

					Tnumber(210,13+((j-7)*58),Tbcd[4]);
					Tnumber(240,13+((j-7)*58),Tbcd[3]);
					Txtext(270,13+((j-7)*58),".");
					Tnumber(290,13+((j-7)*58),Tbcd[2]);
					Tnumber(320,13+((j-7)*58),Tbcd[1]);
					Tnumber(350,13+((j-7)*58),Tbcd[0]);
					if(Ddata[j-7]<10)
						Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //测试完成后去掉	2012.6.24
					else if((Ddata[j-7]>=10)&&(Ddata[j-7]<100))
						Tnumber(556,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=100)&&(Ddata[j-7]<1000))
						Tnumber(536,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=1000)&&(Ddata[j-7]<10000))
						Tnumber(516,13+((j-7)*58),Ddata[j-7]);
					else if(Ddata[j-7]>=10000)
						Tnumber(496,13+((j-7)*58),Ddata[j-7]);	 //测试完成后去掉	 2012.6.24
				 }
				 else									//百位十位位均为零，个位非零
				 {
					Tnumber(240,13+((j-7)*58),Tbcd[3]);
					Txtext(270,13+((j-7)*58),".");				//.占20个像素
					Tnumber(290,13+((j-7)*58),Tbcd[2]);			//一个数字占30个像素
					Tnumber(320,13+((j-7)*58),Tbcd[1]);
					Tnumber(350,13+((j-7)*58),Tbcd[0]);
					if(Ddata[j-7]<10)
						Tnumber(576,13+((j-7)*58),Ddata[j-7]);	     //测试完成后去掉	2012.6.24
					else if((Ddata[j-7]>=10)&&(Ddata[j-7]<100))		//可以看见一个数字占20像素
						Tnumber(556,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=100)&&(Ddata[j-7]<1000))
						Tnumber(536,13+((j-7)*58),Ddata[j-7]);
					else if((Ddata[j-7]>=1000)&&(Ddata[j-7]<10000))
						Tnumber(516,13+((j-7)*58),Ddata[j-7]);
					else if(Ddata[j-7]>=10000)
						Tnumber(496,13+((j-7)*58),Ddata[j-7]);

				 }
			}
		   /**********计量单位显示***********/
			if(DFlag_dw[j-7]==1)					      //测试完成后加上  2012.6.24
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
			Alarm();                           //报警
			send_buf[8*j]=Tbcd[5];				//向NIM-B发送显示数据
			send_buf[8*j+1]=Tbcd[4];
			send_buf[8*j+2]=Tbcd[3];
			send_buf[8*j+3]=Tbcd[2];
			send_buf[8*j+4]=Tbcd[1];
			send_buf[8*j+5]=Tbcd[0];
			send_buf[8*j+6]=Flag_dw;			//发送单位标志
			send_buf[8*j+7]=Flag_need_Flash[j];	//LED闪烁标志
		}
	  Flag_Warn_Count=1;
	  jishucount++;							//计数次数增加（算平均值时用）
	  if(jishucount==255)					//计数次数因为是char型变量，最多支持255个，所以要置零
	      jishucount=8;
	  if(jishucount>4)
	      Flag_Meant = 1;//前五次的取值不要，第五次才开始要，置Flag_Meant=1；否则均置0
	  else
	      Flag_Meant = 0;
	  shortdelay(5000);						//延时
	  //for(i=0;i<33;i++)	//显示完清0
	       //buf[i]=0;
}						//测量标志
else
{
        jishuguan_data=0;                   //need think  whether need?  4.21,int计数管数据，
		dianlishi_data=0;					//电离室数据
		jishuguan_rata=0;					//计数管计数率
		jishuguan_jtemp=0;					//计数管剂量值(单位转换后)
		dianlishi_jtemp=0;					//电离室剂量值（单位转换后）
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
		j=10*biaoding_input-7;						//biaodin_input为要标定的探头序号
		jishuguan_data = buf[4*biaoding_input]*256+buf[4*biaoding_input-1];		//相当于Tdata
		jishuguan_rata=jishuguan_data*12;										//计数率，直接乘以12?
		dianlishi_data = buf[4*biaoding_input+1]*256+buf[4*biaoding_input+2];	//电离室Tdata
		Para[0] = (float)(DataThouth[j]*1000+DataCent[j]*100+DataTenth[j]*10+DataGe[j]); //调节参数
	    Para[1] = (float)((float)(DataThouth[j+1]*1000+DataCent[j+1]*100+DataTenth[j+1]*10+DataGe[j+1])/1000000);
	    jishuguan_DoseRata = (float)(Para[1]*(jishuguan_rata-Para[0]));
		if(jishuguan_DoseRata<0)
			jishuguan_DoseRata=0.0;
		jishuguan_jtemp = (float)jishuguan_DoseRata;
		if((Tbcd[11]<=9)&&(Tbcd[10]<=9)&&(Tbcd[9]<=9)&&(Tbcd[8]<=9)&&(Tbcd[7]<=9)&&(Tbcd[6]<=9))   //4.21  似乎多余，可不可以修改掉
		{

			Tbcd[11] =(uchar)(((int) jishuguan_jtemp)/100);           //千位
			Tbcd[10] =(uchar)(((int) jishuguan_jtemp) % 100 / 10);    //百位
			Tbcd[9] =(uchar)(((int) jishuguan_jtemp) % 100 % 10);    //个位
			jishuguan_count_temp =(jishuguan_jtemp - (int)jishuguan_jtemp)*1000;
			Tbcd[8] =(uchar)(jishuguan_count_temp/100);
			Tbcd[7] =(uchar)(jishuguan_count_temp%100/10);
			Tbcd[6] =(uchar)(jishuguan_count_temp%100%10);
		}
		Para[2] = (float)(DataThouth[j+2]*1000+DataCent[j+2]*100+DataTenth[j+2]*10+DataGe[j+2]); //调节参数
	    Para[3] = (float)((float)(DataThouth[j+3]*1000+DataCent[j+3]*100+DataTenth[j+3]*10+DataGe[j+3])/1000);
		dianlishi_DoseRata = (float)(Para[3]*(dianlishi_data-Para[2]));
		if(dianlishi_DoseRata<0)
			dianlishi_DoseRata=0.0;
		dianlishi_jtemp = (float)(dianlishi_DoseRata/1000);
		if((Tbcd[5]<=9)&&(Tbcd[4]<=9)&&(Tbcd[3]<=9)&&(Tbcd[2]<=9)&&(Tbcd[1]<=9)&&(Tbcd[0]<=9))   //4.21  似乎多余，可不可以修改掉
		{

			Tbcd[5] =(uchar)(((int) dianlishi_jtemp)/100);           //千位
			Tbcd[4] =(uchar)(((int) dianlishi_jtemp) % 100 / 10);    //百位
			Tbcd[3] =(uchar)(((int) dianlishi_jtemp) % 100 % 10);    //个位
			dianlishi_count_temp =(dianlishi_jtemp - (int)dianlishi_jtemp)*1000;
			Tbcd[2] =(uchar)(dianlishi_count_temp/100);
			Tbcd[1] =(uchar)(dianlishi_count_temp%100/10);
			Tbcd[0] =(uchar)(dianlishi_count_temp%100%10);
		}
		Txtext(200,58,"标定探头");
		Tnumber(400,58,biaoding_input);
		Txtext(10,116,"盖革管");;
		Txtext(160,116,":");
		Txtext(220,174,"uGy/h");
		Txtext(520,174,"CPM");
		Txtext(10,290,"电离室");;
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
			  	Tnumber(490,174,jishuguan_data);	     //测试完成后去掉	2012.6.24
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
			  		Tnumber(490,174,jishuguan_data);	     //测试完成后去掉	2012.6.24
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
			  		Tnumber(490,174,jishuguan_data);	     //测试完成后去掉	2012.6.24
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
			  	Tnumber(490,348,dianlishi_data);	     //测试完成后去掉	2012.6.24
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
			  		Tnumber(490,348,dianlishi_data);	     //测试完成后去掉	2012.6.24
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
			  		Tnumber(490,348,dianlishi_data);	     //测试完成后去掉	2012.6.24
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
*           短延时
************************************/
void shortdelay(uint i)
{
   uint k;
   uint n;
   for(k=0;k<i;k++)
	   for(n=200;n>0;n--);
}

/********************************
*        指示灯和报警
********************************/
void Alarm()
{
	uchar n;
    for(n=0;n<10;n++)
    {
	     Led573=Var_Led;
		 if(Flag_Warn==1)                   //声音报警
			{
		  	  Speak=1;
			 }
   }
}

/*******************************
*       更新指示灯状态
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
*          指示灯闪烁
************************************/
void Led_Flash(void)
{
    uchar i;
	for(i=0;i<8;i++)
	{
		Updata_Flash(i);                               //状态取反
        Led_Disp(i,Flag_need_Flash[i],State_Flash[i]); //灯状态显示
	}
}

/************************************
*         指示灯状态显示
*************************************/
void Led_Disp(uchar Num,uchar Flag,uchar State)
{
	if(Flag)
	{
		if(State)
		{
			Var_Led=Var_Led & Svar0[Num];            //指示灯亮
			Led573=Var_Led;
			if(Flag_Warn==1)                        //需要蜂鸣器工作
			{
				Speak=0;                            //蜂鸣器响
				shortdelay(100);
			}
			return;
		 }
	}
	Var_Led=Var_Led | Svar1[Num];                   //指示灯灭
	Led573=Var_Led;
	if(Flag_Warn==1)                                //需要蜂鸣器工作
    {
		Speak=1;                                    //蜂鸣器不响
		shortdelay(100);
		}
}
