#ifndef _CONFIG_H
#define _CONFIG_H

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
#define Led573      XBYTE[0xc000]       //led�ź�ָʾ��
#define Keypress    XBYTE[0xc800]       //������ַ
#define receiveradd XBYTE[0xd000]               //���д�����յ�ַ
#define sendadd     XBYTE[0xd800]               //���д��䷢�͵�ַ
#define Signal1     XBYTE[0xa800]      //̽ͷָ���źŽӼ�����
#define Signal2     XBYTE[0xb000]		//̽ͷָ���źŽӵ�����
#define Signal3     XBYTE[0xb800]		//̽ͷָ���źŽӵ�����

#define NUMSENDBYTES    65              // �������ݸ��������
#define TimeFiveHundred 100              //��ʱ500ms
#define TimeThreeThousand 600        //��ʱ1s
#define TimeOneHundred  1000              //��ʱ100ms
 
extern uchar DataGe[75];             //��λ
extern uchar DataTenth[75];          //ʮ��λ
extern uchar DataCent[75];           //�ٷ�λ
extern uchar DataThouth[75];         //ǧ��λ
extern uchar YuThouth[8];
extern uchar YuCent[8];
extern uchar YuTenth[8];
extern uchar YuGe[8];
extern uchar Flag_need_warn[7];                   //��Ҫ������־
extern uchar State_On_Off[7];                     //ָʾ��״̬��־
extern uchar buf[33];               //Ҫ���͵����ݵĸߵ�λ
extern uchar send_buf[65];            //add 8  Flag_need_Flash  delect 32 yuzhi   4.21
extern uchar dt_in[331]; 
extern uchar Flag_need_Flash[8];
extern uchar State_Flash[8];
extern uchar receive_buf[57];
extern uchar biaoding_input; 
//extern uint compare[32];
//extern uchar backup_data[20];            //��ű�����ʾ������ 
extern uchar data_A[8];
extern uchar data_B[12];
extern float GDoseRata[8];                  //ÿ��̽ͷ��õļ�����
extern float DDoseRata[8];                  //ÿ��̽ͷ��õļ�����
extern float DoseRata[8];                  //ÿ��̽ͷ��õļ�����
extern float TrueRata[8];                  //ÿ��̽ͷ��õļ�����
extern float Mean_Value[8];
extern uchar count_change_flag[8];
extern float idata save0[8],save1[8],save2[8];
extern uchar Redrawtime;
extern uchar Inctime;
extern uchar Incinput;
extern uchar Incdataright;
extern uchar Incdata;
extern uchar jishucount;
extern uchar Flag_dw[8]; 
extern bdata bit Flag_Warn;
extern bdata bit Flag_RefrInput;
extern bdata bit Flag_RefrTime;
extern bdata bit Flag_RefrPara;
extern bdata bit Flag_RefrRight;
extern bdata bit Flag_RefrLeft;
extern bdata bit Flag_KeyLeft;
extern bdata bit Flag_Tim1;
extern bdata bit Flag_Mast;             //���ӻ���־
extern bdata bit FlagMasColSlavOK;
extern bdata bit Flag_Warn_Led;
extern bdata bit Flag8253Cnting;
extern bdata bit Flag_Warn_Count;
extern bdata bit Flag_Warn_Flash;
extern bdata bit Flag_ParaChange;                       //�������øı�
extern bdata bit Flag_InputChange;                      //̽ͷ���øı�
extern bdata bit Flag_Tim0;                             //��ʱ��0������־
//extern bit Flag_serial1_led;   
extern bdata bit Flag_Warn_Serial1;                     //������ʾ�ƶ�ʱ������ʶ
extern bdata bit Flag_Collateral;                       //�ӻ�����ָ�ʱ��־
extern bdata bit FlagCollFall;                         //���ӻ�ͨ��ʧ�ܱ�־ 
extern bdata bit FlagCollErr;
extern bdata bit FlagColling; 
extern bdata bit measure_flag;
//extern bit Flag_data_change;   
//extern bit Flag_Commond; 
extern bdata bit Flag_Meant;               
extern uchar idata bz; 
extern uchar idata xh;
extern uchar idata bz;
extern uchar idata wb;
extern uint  idata Tdata[8];
extern uint  idata Ddata[8];
extern uchar idata Var_Signal1;
extern uchar idata Var_Signal2;
extern uchar idata Var_Signal3;
extern uchar idata Var_Led;
extern uint CntWarn;
extern uint CntColTimer;
extern uint NumT0; 
extern uint CmOverTime;




extern void Lcd_init();
extern void Lcd_Start();
extern void Lcd_Clear();
extern void Init_8253();
extern void Init_Time0();
extern void Txbyte(uchar idata i);
extern void Tword(uint idata i);
extern void TxEOF();
extern void Tnumber(int idata x,int idata y,uint idata n);
extern void Txtext(uint idata x,uint idata y,uchar *s);
extern void Open (uint idata x,uint idata y);
extern void Close (uint idata x,uint idata y);
extern void WarnLed_On(uint idata x,uint idata y);
extern void WarnLed_Off(uint idata x,uint idata y);
extern void DS_SaveData(uchar *dt);
extern void DS_ReadData(uchar *dt);
extern void setparameter();
extern void parameterright();
extern void GetAndDisdata();
extern void ShowData();
extern void init();
extern void SendData();
extern void setinput();
extern void settime();
extern void measure();
extern void ReSet();
extern void Send_Word(uchar *pbuf,uint ibyte);
extern void Collect_Word(uchar *pub,uint nbyte);
extern void shortdelay(uint idata i);
extern void RedLed_Flash();
extern void Warning_Disp(uchar Num,uchar Flag,uchar State);
extern void Clear();
//extern void picture();
extern uchar CKMasterCmd();
extern void Alarm();
extern void Led_Flash(void);
//extern void BackUp_Display(void);

sbit scl = P1^4 ;			//ʱ���߽�p14
sbit sda = P1^5 ;			//�����߽�p15
sbit GATE= P1^1;            //8253�ſ��ź�
sbit Speak=P1^0;            //���������ߵ�ƽ��ʾ����
sbit Judge_Speak=P1^7;      //��������־�źţ����ذ������ߵ�ƽ��ʾ������������
sbit Zhu_Cong=P1^6;         //���ӻ���־�źţ����ذ������ߵ�ƽΪ����
sbit PCOLSIG=P3^3;          //��ʾ�ӻ����ݣ�Ӧ��û��ʵ�尴���������ⰴ��ʹ�ã��ߵ�ƽ��ʾ���ɼ�����
sbit Note0=P3^4;            //���ڴӻ��������ݱ�־
sbit Ack0=P3^5;             //������������������Ӧ���źű�־
sbit Note1=P4^4;            //���ڴӻ��������ݱ�־
sbit Ack1=P4^6;             //������������������Ӧ���źű�־
sbit PALE=P4^5;             //������ʾ���������ߵ�ƽ��ʾ����ʾ���û�

#endif




