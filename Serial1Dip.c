<<<<<<< HEAD
/*******************************************************************
*������
*     ������ʾ����
*���ܣ�
*     ���û���ʾNIM-A��NIM-B�������ݣ������Ҫ����������Ļ����ʾָʾ��

*******************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"
#include "stdio.h"
#include "math.h"


/**������ʼ��*/
//uchar backup_data[20];            //��ű�����ʾ������ 
//uchar data_A[8];
//uchar data_B[12];
//bit Flag_data_change;           //���ô˰�ʱ���ݸ��±�־
//uchar alarm_data = 0x00;
//bit Flag_serial1_led;                        //���û�����˸������ʱ��־

//void Write_Data (uchar Pchar);
//uchar Backup_Covn(uchar temp_data,uchar code_data);
//void BackUp_displayA();
//void SegLED_Clear();
/*************************************
*         ������ʾ����ʾNIM_A    
*************************************/
/*void BackUp_displayA()//4.20���޸�
{
    uchar idata z,x;
	uchar idata uGy,mGy,Gy;
    
	for(z=0;z<2;z++)
	  {   
			if(z == 0)      //��ʾ������̽ͷ
			{
				 x = 2;
			}
			if(z == 1)      //��ʾ�ڰ˸�̽ͷ
			{
				 x = 7;
			}*/
			/******����������******/
/*			if(x == 2)
			{
				if(send_buf[8*x+7] == 1)
				{
					alarm_data = 0x42;
					backup_data[6*z] = alarm_data ;
				}
				else if(send_buf[8*x+7] == 0)
				{
					alarm_data = 0x02;
					backup_data[6*z] = alarm_data ;
				}
			}
			if(x == 7)
			{
				if(send_buf[8*x+7] == 1)
				{
					alarm_data |= 0x20;
					backup_data[6*z] = alarm_data ;
				}
				else if(send_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z] = alarm_data ;
				}
			}*/
			/*****��λ��ѡ��*****/		
/*			if(send_buf[8*x+6] == 1)
			{
				uGy = 1;
				backup_data[6*z+1] = 0x30;
			}
			if(send_buf[8*x+6] == 2)
			{
				mGy = 1;
                backup_data[6*z+1] = 0x28;
			}
			if(send_buf[8*x+6] == 3)
			{
				Gy = 1;
				backup_data[6*z+1] = 0x24;
			}*/
			/*****���ݵ���ʾ******/			
/*			if(send_buf[8*x] != 0)   //���ǧλ��Ϊ��
			{
				data_A[4*z+3] = Backup_Covn(send_buf[8*x],data_A[4*z+3]);
			    data_A[4*z+2] = Backup_Covn(send_buf[8*x+1],data_A[4*z+2]);
				data_A[4*z+1] = Backup_Covn(send_buf[8*x+2],data_A[4*z+1]);
				data_A[4*z] = Backup_Covn(send_buf[8*x+3],data_A[4*z]);
			}
			else if(send_buf[8*x] == 0)   //���ǧλΪ��
			{
				data_A[4*z+3] = 0x03;
				if(send_buf[8*x+1] != 0)   //�����λ��Ϊ��
				{
				    data_A[4*z+2] = Backup_Covn(send_buf[8*x+1],data_A[4*z+2]);
				}
				else if(send_buf[8*x+1] == 0)   //�����λΪ��
				{
				    data_A[4*z+2] = 0x03;	
				}
				data_A[4*z+1] = Backup_Covn(send_buf[8*x+2],data_A[4*z+1]);  //����ʮλ�Ƿ�Ϊ�㣬��Ҫ��ʾ
			    data_A[4*z] = Backup_Covn(send_buf[8*x+3],data_A[4*z]);
			}
			data_A[4*z+3] = data_A[4*z+3] << 4;
			backup_data[4*z+3] = data_A[4*z+3] | data_A[4*z+2];
			data_A[4*z+1] = data_A[4*z+1] << 4;
			backup_data[4*z+2] = data_A[4*z+1] | data_A[4*z];
		}
}*/
/**********************************
*         ������ʾ�رմ���
***********************************/
/*void SegLED_Clear()
{
		Write_Data(0x00);
		Write_Data(0x00);
		Write_Data(0x33);
		Write_Data(0x33);
		Write_Data(0x33);
}*/

/***********************************
*          ������ʾ��������
************************************/
/*uchar Backup_Covn(uchar temp_data,uchar code_data)
{
    uchar cov_data[10] = {0x00,0x08,0x04,0x0c,0x02,0x0a,0x06,0x0e,0x01,0x09};
	uchar i;
	for(i=0;i<10;i++)
	{
		if(temp_data == i)
		{
			code_data = cov_data[i];
			return code_data;
		}
	 }
	 
}*/
/*************************************
*         ������ʾ����ʾNIM_B    
*************************************/
/*void BackUp_displayB()//4.20���޸�
{
    uchar idata z,x;
	uchar idata mBq,kBq,Bq;
	for(z=0;z<3;z++)
	  {    
			if(z == 0)      //��ʾ��һ��̽ͷ
			{
			 x = 0;
			}
			if(z == 1)      //��ʾ�ڶ���̽ͷ
			{
			 x = 1;
			}
            if(z == 2)      //��ʾ�����̽ͷ
            {
			 x = 4;
            }*/
			/******����������******/
/*            if(x == 0)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x10;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}
			if(x == 1)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x08;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}
			if(x == 4)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x04;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}*/
			/*****��λ��ѡ��*****/		
/*			if(receive_buf[8*x+6] == 1)
			{
				Bq = 1;
				backup_data[6*z+13] = 0x20;
				backup_data[6*z+12] |= 0x80;
				
			}
			if(receive_buf[8*x+6] == 2)
			{
				kBq = 1;
                backup_data[6*z+13] = 0x21;
			}
			if(receive_buf[8*x+6] == 3)
			{
				mBq = 1;
				backup_data[6*z+13] = 0x22;
			}*/
			/*****���ݵ���ʾ******/			
/*			if(receive_buf[8*x] != 0)   //���ǧλ��Ϊ��
			{
				data_B[4*z+3] = Backup_Covn(receive_buf[8*x],data_B[4*z+3]);
			    data_B[4*z+2] = Backup_Covn(receive_buf[8*x+1],data_B[4*z+2]);
				data_B[4*z+1] = Backup_Covn(receive_buf[8*x+2],data_B[4*z+1]);
				data_B[4*z] = Backup_Covn(receive_buf[8*x+3],data_B[4*z]);
			}
			else if(receive_buf[8*x] == 0)   //���ǧλΪ��
			{
				data_B[4*z+3] = 0x03;
				if(receive_buf[8*x+1] != 0)   //�����λ��Ϊ��
				{
				    data_B[4*z+2] = Backup_Covn(receive_buf[8*x+1],data_B[4*z+2]);
				}
				else if(receive_buf[8*x+1] == 0)   //�����λΪ��
				{
				    data_B[4*z+2] = 0x03;	
				}
				data_B[4*z+1] = Backup_Covn(receive_buf[8*x+2],data_B[4*z+1]);  //����ʮλ�Ƿ�Ϊ�㣬��Ҫ��ʾ
			    data_B[4*z] = Backup_Covn(receive_buf[8*x+3],data_B[4*z]);
			}
			data_B[4*z+3] = data_B[4*z+3] << 4;
			backup_data[4*z+11] = data_B[4*z+3] | data_B[4*z+2];
			data_B[4*z+1] = data_B[4*z+1] << 4;
			backup_data[4*z+10] = data_B[4*z+1] | data_B[4*z];
		}
}*/

/**************************************
*        ������ʾ������
**************************************/
/*void bakeup_conv_data()
{
	alarm_data = 0x00;
	BackUp_displayA();
	BackUp_displayB();
}*/

/*********************************
*        ����ת������д��
***********************************/
/*void Write_Data (uchar Pchar)	//UART1��������
{
		SBUF  = Pchar;
		while (!TI);		// �ȴ���TI1Ϊ1
		TI	= 0; 			// ��TI1����
}*/


/**********************************
*        ��ʾ���ݵ�������������
***********************************/
/*void display_covn_data()
{
	uchar i,t;	
	for(i=1;i<6;i++)
	{	
//		BackUp_Display();
		if((Flag_Commond == 0) && (Flag_data_change == 1))
		{
		    t = Backup_Covn(i,t);
		    SegLED_Clear();
			Write_Data(backup_data[4*i-4]);
			Write_Data(backup_data[4*i-3]);
			Write_Data(backup_data[4*i-2]);
			Write_Data(backup_data[4*i-1]);
			Write_Data(t);
		}
		if((Flag_Commond == 1) || (Flag_data_change == 0))
		{
			SegLED_Clear();
		 	return;
		}
		if(Flag_Warn_Flash)                  //����LEDָʾ����˸�����ʱ
		{
			Flag_Warn_Flash = 0;
            Led_Flash();
		}
		if((Flag_Warn_Led==1)&&(PCOLSIG==0))  //��Ļ�Ϻ����˸�����ʱ         
		{
			Flag_Warn_Led = 0;
			RedLed_Flash();
		 }
		shortdelay(2000);
	}
}*/
=======
/*******************************************************************
*������
*     ������ʾ����
*���ܣ�
*     ���û���ʾNIM-A��NIM-B�������ݣ������Ҫ����������Ļ����ʾָʾ��

*******************************************************************/
#include "STC12C5A60S2.h"
#include "intrins.h"
#include "config.h" 
#include "absacc.h"
#include "stdio.h"
#include "math.h"


/**������ʼ��*/
//uchar backup_data[20];            //��ű�����ʾ������ 
//uchar data_A[8];
//uchar data_B[12];
//bit Flag_data_change;           //���ô˰�ʱ���ݸ��±�־
//uchar alarm_data = 0x00;
//bit Flag_serial1_led;                        //���û�����˸������ʱ��־

//void Write_Data (uchar Pchar);
//uchar Backup_Covn(uchar temp_data,uchar code_data);
//void BackUp_displayA();
//void SegLED_Clear();
/*************************************
*         ������ʾ����ʾNIM_A    
*************************************/
/*void BackUp_displayA()//4.20���޸�
{
    uchar idata z,x;
	uchar idata uGy,mGy,Gy;
    
	for(z=0;z<2;z++)
	  {   
			if(z == 0)      //��ʾ������̽ͷ
			{
				 x = 2;
			}
			if(z == 1)      //��ʾ�ڰ˸�̽ͷ
			{
				 x = 7;
			}*/
			/******����������******/
/*			if(x == 2)
			{
				if(send_buf[8*x+7] == 1)
				{
					alarm_data = 0x42;
					backup_data[6*z] = alarm_data ;
				}
				else if(send_buf[8*x+7] == 0)
				{
					alarm_data = 0x02;
					backup_data[6*z] = alarm_data ;
				}
			}
			if(x == 7)
			{
				if(send_buf[8*x+7] == 1)
				{
					alarm_data |= 0x20;
					backup_data[6*z] = alarm_data ;
				}
				else if(send_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z] = alarm_data ;
				}
			}*/
			/*****��λ��ѡ��*****/		
/*			if(send_buf[8*x+6] == 1)
			{
				uGy = 1;
				backup_data[6*z+1] = 0x30;
			}
			if(send_buf[8*x+6] == 2)
			{
				mGy = 1;
                backup_data[6*z+1] = 0x28;
			}
			if(send_buf[8*x+6] == 3)
			{
				Gy = 1;
				backup_data[6*z+1] = 0x24;
			}*/
			/*****���ݵ���ʾ******/			
/*			if(send_buf[8*x] != 0)   //���ǧλ��Ϊ��
			{
				data_A[4*z+3] = Backup_Covn(send_buf[8*x],data_A[4*z+3]);
			    data_A[4*z+2] = Backup_Covn(send_buf[8*x+1],data_A[4*z+2]);
				data_A[4*z+1] = Backup_Covn(send_buf[8*x+2],data_A[4*z+1]);
				data_A[4*z] = Backup_Covn(send_buf[8*x+3],data_A[4*z]);
			}
			else if(send_buf[8*x] == 0)   //���ǧλΪ��
			{
				data_A[4*z+3] = 0x03;
				if(send_buf[8*x+1] != 0)   //�����λ��Ϊ��
				{
				    data_A[4*z+2] = Backup_Covn(send_buf[8*x+1],data_A[4*z+2]);
				}
				else if(send_buf[8*x+1] == 0)   //�����λΪ��
				{
				    data_A[4*z+2] = 0x03;	
				}
				data_A[4*z+1] = Backup_Covn(send_buf[8*x+2],data_A[4*z+1]);  //����ʮλ�Ƿ�Ϊ�㣬��Ҫ��ʾ
			    data_A[4*z] = Backup_Covn(send_buf[8*x+3],data_A[4*z]);
			}
			data_A[4*z+3] = data_A[4*z+3] << 4;
			backup_data[4*z+3] = data_A[4*z+3] | data_A[4*z+2];
			data_A[4*z+1] = data_A[4*z+1] << 4;
			backup_data[4*z+2] = data_A[4*z+1] | data_A[4*z];
		}
}*/
/**********************************
*         ������ʾ�رմ���
***********************************/
/*void SegLED_Clear()
{
		Write_Data(0x00);
		Write_Data(0x00);
		Write_Data(0x33);
		Write_Data(0x33);
		Write_Data(0x33);
}*/

/***********************************
*          ������ʾ��������
************************************/
/*uchar Backup_Covn(uchar temp_data,uchar code_data)
{
    uchar cov_data[10] = {0x00,0x08,0x04,0x0c,0x02,0x0a,0x06,0x0e,0x01,0x09};
	uchar i;
	for(i=0;i<10;i++)
	{
		if(temp_data == i)
		{
			code_data = cov_data[i];
			return code_data;
		}
	 }
	 
}*/
/*************************************
*         ������ʾ����ʾNIM_B    
*************************************/
/*void BackUp_displayB()//4.20���޸�
{
    uchar idata z,x;
	uchar idata mBq,kBq,Bq;
	for(z=0;z<3;z++)
	  {    
			if(z == 0)      //��ʾ��һ��̽ͷ
			{
			 x = 0;
			}
			if(z == 1)      //��ʾ�ڶ���̽ͷ
			{
			 x = 1;
			}
            if(z == 2)      //��ʾ�����̽ͷ
            {
			 x = 4;
            }*/
			/******����������******/
/*            if(x == 0)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x10;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}
			if(x == 1)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x08;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}
			if(x == 4)
			{
				if(receive_buf[8*x+7] == 1)
				{
					alarm_data |= 0x04;
					backup_data[6*z+12] = alarm_data ;
				}
				else if(receive_buf[8*x+7] == 0)
				{
					alarm_data |= 0x00;
					backup_data[6*z+12] = alarm_data ;
				}
			}*/
			/*****��λ��ѡ��*****/		
/*			if(receive_buf[8*x+6] == 1)
			{
				Bq = 1;
				backup_data[6*z+13] = 0x20;
				backup_data[6*z+12] |= 0x80;
				
			}
			if(receive_buf[8*x+6] == 2)
			{
				kBq = 1;
                backup_data[6*z+13] = 0x21;
			}
			if(receive_buf[8*x+6] == 3)
			{
				mBq = 1;
				backup_data[6*z+13] = 0x22;
			}*/
			/*****���ݵ���ʾ******/			
/*			if(receive_buf[8*x] != 0)   //���ǧλ��Ϊ��
			{
				data_B[4*z+3] = Backup_Covn(receive_buf[8*x],data_B[4*z+3]);
			    data_B[4*z+2] = Backup_Covn(receive_buf[8*x+1],data_B[4*z+2]);
				data_B[4*z+1] = Backup_Covn(receive_buf[8*x+2],data_B[4*z+1]);
				data_B[4*z] = Backup_Covn(receive_buf[8*x+3],data_B[4*z]);
			}
			else if(receive_buf[8*x] == 0)   //���ǧλΪ��
			{
				data_B[4*z+3] = 0x03;
				if(receive_buf[8*x+1] != 0)   //�����λ��Ϊ��
				{
				    data_B[4*z+2] = Backup_Covn(receive_buf[8*x+1],data_B[4*z+2]);
				}
				else if(receive_buf[8*x+1] == 0)   //�����λΪ��
				{
				    data_B[4*z+2] = 0x03;	
				}
				data_B[4*z+1] = Backup_Covn(receive_buf[8*x+2],data_B[4*z+1]);  //����ʮλ�Ƿ�Ϊ�㣬��Ҫ��ʾ
			    data_B[4*z] = Backup_Covn(receive_buf[8*x+3],data_B[4*z]);
			}
			data_B[4*z+3] = data_B[4*z+3] << 4;
			backup_data[4*z+11] = data_B[4*z+3] | data_B[4*z+2];
			data_B[4*z+1] = data_B[4*z+1] << 4;
			backup_data[4*z+10] = data_B[4*z+1] | data_B[4*z];
		}
}*/

/**************************************
*        ������ʾ������
**************************************/
/*void bakeup_conv_data()
{
	alarm_data = 0x00;
	BackUp_displayA();
	BackUp_displayB();
}*/

/*********************************
*        ����ת������д��
***********************************/
/*void Write_Data (uchar Pchar)	//UART1��������
{
		SBUF  = Pchar;
		while (!TI);		// �ȴ���TI1Ϊ1
		TI	= 0; 			// ��TI1����
}*/


/**********************************
*        ��ʾ���ݵ�������������
***********************************/
/*void display_covn_data()
{
	uchar i,t;	
	for(i=1;i<6;i++)
	{	
//		BackUp_Display();
		if((Flag_Commond == 0) && (Flag_data_change == 1))
		{
		    t = Backup_Covn(i,t);
		    SegLED_Clear();
			Write_Data(backup_data[4*i-4]);
			Write_Data(backup_data[4*i-3]);
			Write_Data(backup_data[4*i-2]);
			Write_Data(backup_data[4*i-1]);
			Write_Data(t);
		}
		if((Flag_Commond == 1) || (Flag_data_change == 0))
		{
			SegLED_Clear();
		 	return;
		}
		if(Flag_Warn_Flash)                  //����LEDָʾ����˸�����ʱ
		{
			Flag_Warn_Flash = 0;
            Led_Flash();
		}
		if((Flag_Warn_Led==1)&&(PCOLSIG==0))  //��Ļ�Ϻ����˸�����ʱ         
		{
			Flag_Warn_Led = 0;
			RedLed_Flash();
		 }
		shortdelay(2000);
	}
}*/
>>>>>>> origin/master
