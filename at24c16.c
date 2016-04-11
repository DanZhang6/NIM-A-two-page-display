/*************************************************
*������
*     AT24C16���ݴ�ȡ����
*���ܣ�
*     �洢ʱ�䣬̽ͷ���������ò�����������

************************************************/

#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h"

void delay_n5ms_24(uchar idata n);


/********��ʼ����****************/
void start_24(void)     //ʱ���߱��ָߵ�ƽ�ڼ䣬�����ߵ�ƽ�Ӹߵ��͵�������Ϊ���ߵ���ʼ�ź�
{
     _nop_();
	 scl = 0 ;
	 sda = 1 ;
	 scl = 1 ;
     _nop_();      
	 sda = 0 ;
     _nop_();
     _nop_();
	 scl = 0 ;
     _nop_();
     _nop_();
     sda = 1 ;
}

/*******��������****************/
void stop_24(void)     //ʱ���߱��ָߵ�ƽ�ڼ䣬�����ߵ�ƽ�ӵ͵��ߵ�������Ϊ���ߵ�ֹͣ�ź�
{
	 scl = 0 ;
	 sda = 0 ;
	 scl = 1 ; _nop_() ;
	 sda = 1 ; _nop_() ;
	 scl = 0 ;
}
/*******************************
        ��8 Bit ����
 ******************************/
uchar read_8_bit_24(void)
{
	 uchar idata r_8_bit;
	 uchar idata n;
	 uchar idata m;
	 uchar idata temp[8] = 0 ;
	 
	 r_8_bit = 0 ;
	 scl = 0 ;scl = 1 ;
	 for(n=0; n<8; n++)					//�õ����ݣ��ɸߵ���λ
		{
			scl = 1 ;
			_nop_() ;
			temp[n] = sda ;
			scl = 0 ;
		}
	 scl = 0 ;
	 sda = 1 ;scl = 1 ; 
	 _nop_() ; _nop_() ;
	 scl = 0 ;							//����ȷ��λ
	 
	 for(m=0; m<8; m++)					//���ݴ���
	 	{
			r_8_bit = r_8_bit << 1 ;
			if(temp[m]==1) r_8_bit = r_8_bit|0x01 ;
			else r_8_bit = r_8_bit|0x00 ;
		}
	 return r_8_bit ;
}

/***************************************
            д8 Bit ���� x
****************************************/
void write_8_bit_24(uchar idata x)
{
	 uchar idata w_8_bit;
	 uchar idata n, m, a;
	 uchar idata temp[8] = 0 ;
	 w_8_bit = x ;
	 
	 for(n=0; n<8; n++)
	 	{
			a = w_8_bit&0x80 ;				//��λ��ǰ
			if(a==0x80) temp[n] = 1 ;
			else temp[n] = 0 ;
			w_8_bit = w_8_bit << 1 ;
		}
	 
	 sda = 1 ;scl = 0 ;
	 for(m=0; m<8; m++)						//��д��λ
		{
			sda = temp[m] ;
			scl = 1 ;
			_nop_() ;
			scl = 0 ;
		}
	 
	 scl = 1 ; _nop_() ;
	 scl = 0 ;								//����ȷ��λ

 }

/************************************************************************
 *                         ��1 Byte ����
 * ���������r_add      Ҫ�������ݵĵ�ַ,��ʽΪ:1010+ҳ��(3λ)+0+��ַ(8λ)
 * ���ز�����r_1_byte   ���õ�������
**************************************************************************/
uchar read_1_byte_24(uint idata r_add)
{
	 uchar idata r_1_byte ;
	 uchar idata H , L, h;
	 
	 H = r_add / 256 ;						//�õ���8λ����
	 L = r_add % 256 ;						//�õ���8λ����
	 h = H+1 ;								//�õ���ָ�������
	 
	 delay_n5ms_24(1);
	 
	 start_24();
	 write_8_bit_24(H);						//��дָ��
	 	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	 write_8_bit_24(L);						//д���ַ
	 	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	 
	 start_24();
	 write_8_bit_24(h);						//��������
	 	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	
	 r_1_byte = read_8_bit_24() ;			//��������
	 	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	 stop_24();
	
	return r_1_byte ;
}

/******************************************************************
 *                          д1 Byte ����
 * ���������w_add     д�ĵ�ַ����ʽΪ:1010+ҳ��(3λ)+0+��ַ(8λ)
 *		     w_1_byte  Ҫд�������
*******************************************************************/
void write_1_byte_24(uint idata w_add , uchar idata w_1_byte)
{
	 uchar idata H,L;
	 H = w_add / 256 ;
	 L = w_add % 256 ;
	 
	 delay_n5ms_24(1);							//����ʱ�����������дҪ��ʱ������
	 
	 start_24();
	 write_8_bit_24(H);							//��дָ��,д��8λ
	   	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	 
	 write_8_bit_24(L);							//д���ַ,д��8λ
	   	_nop_() ;_nop_() ;
        _nop_(); _nop_();
	 
	 write_8_bit_24(w_1_byte);					//д������
	   	_nop_() ;_nop_() ; 
        _nop_(); _nop_();
	 stop_24();

}

/************************************
 *        ��ʱ���� 5n ms
 ************************************/
void delay_n5ms_24(uchar idata n)
{
	 uchar idata q ;
	 uint idata p ;
	 for(q=0;q<n;q++)
	 for(p=0;p<5000;p++){};
}

/*************************************
 *         �洢����
*************************************/
void DS_SaveData(uchar *dt)
{
	uint data i;
	uint data w=0xa000;
	for(i=0;i<331;i++)
	 {
		write_1_byte_24(w,dt[i]);
        delay_n5ms_24(2);
        w++;
		if(w==0xa100)
	    {
		  w=0xa200;
		  }
	 }
}  

/*************************************
 *          ��ȡ����
*************************************/
void DS_ReadData(uchar *dt)
{
	uint data j;
	uint data w=0xa000;
	for(j=0;j<331;j++)
	{
		dt[j]=read_1_byte_24(w);					//��ȡ1Byte����
        delay_n5ms_24(2);							//��ʱ2*5=10ms
        w++;
		if(w==0xa100)
	    { 
	    w=0xa200;
		}
	}
}  
