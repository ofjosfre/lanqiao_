/*******************************************************************************  
* �ļ����ƣ�ͨ��ʱ��оƬDS1302��ʾ������ʱ��ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪBTN��J2����Ϊ1-3��2-4
* ʵ�����󣺰���S7�����л�����ʱ����桢S6��ʱ/��/�롢S5��ʱ/��/��
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include "stc15.h"
#include "ds1302.h"
#include "onewire.h"
unsigned char  inittime[] = {58,59,23,5,1,3,22};//��-��-ʱ-��-��-����-���
unsigned char  alarm_hour,alarm_min,alarm_sec;
unsigned int temperature;
unsigned char gettime[7];
unsigned char  SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
};
unsigned char displaytime_flag  = 0;
unsigned char displayalarm_flag = 0;
unsigned char count = 0;
unsigned char led_count = 0;
unsigned char ledtime_count = 0;
unsigned char led_flag = 0;
unsigned char alarm_ring_flag = 1;
unsigned char time_alarm_flag = 0;
unsigned char shanshuo_flag = 0;
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit LED1 = P0^0;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
void Alarm_LED();
void ds1302_init();
void readtime();
void stoptime(void);
void starttime(void);
void Key_Scan();
void initTimer0();
void HC138_Select(unsigned char i);
void DelayKTime(unsigned char t);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);
void displaytime();
void display_temperature();
void main()
{
	HC138_Select(4);
	P0=0X00;
	HC138_Select(1);
	P0=0XFF;
	ds1302_init();
	initTimer0();
	while(1)
	{
		temperature = (unsigned int)(read_temperature()); 
		readtime();
		Key_Scan();
		Alarm_LED();
		displaytime();
	}
}

void ds1302_init()
{
	unsigned char i,add = 0x80;
	write(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		write(add,inittime[i]);
		add = add+2;
	}
	write(0x8e,0x80);
}
void readtime()
{
	unsigned char i,add = 0x81;
	if(displaytime_flag==0)
	{
		write(0x8e,0x00);
		for(i=0;i<7;i++)
		{
			gettime[i] = read(add);
			add = add+2;
		}
		write(0x8e,0x80);
	}
}
void Display_SEG_Bit(unsigned char pos,unsigned char value)
{
	HC138_Select(3);//λѡ
	P0 = 0x00;
	P0 = 0x01<<(pos-1);
	HC138_Select(2);//��ѡ
	P0 = 0xff;
	P0 = SMG_duanma[value];
	DelaySMG(1);
	P0 = 0xff;
}
void HC138_Select(unsigned char i)
{
	switch(i)
	{
		case 1://LED
				HC138_A = 0;
				HC138_B = 0;
				HC138_C = 1;
				break;
		case 2://SEG
				HC138_A = 1;
				HC138_B = 1;
				HC138_C = 1;
				break;
		case 3://Bit
				HC138_A = 0;
				HC138_B = 1;
				HC138_C = 1;
				break;
		case 4://���ֶٹ����� ȡ����
				HC138_A = 1;
				HC138_B = 0;
				HC138_C = 1;
				break;
		default:break;
	}
}
void DelaySMG(unsigned int times)		//@12.000MHz
{
	unsigned char i,j;
	unsigned int n;
	for(n = 0;n < times;n++)
	{
		i = 12;
		j = 50;
		do
		{
			while (--j);
		}
		while (--i);
	}

}
void displaytime()
{
	if(time_alarm_flag==0)
	{
	if(displaytime_flag==0)
	{
		Display_SEG_Bit(1,gettime[2]/10);//ʱ��ʮλ
		Display_SEG_Bit(2,gettime[2]%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,gettime[1]%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//���ʮλ
		Display_SEG_Bit(8,gettime[0]%10);//��ĸ�λ
	}
	else if(displaytime_flag==1)
	{
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,gettime[1]%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//���ʮλ
		Display_SEG_Bit(8,gettime[0]%10);//��ĸ�λ
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(1,gettime[2]/10);//ʱ��ʮλ
		Display_SEG_Bit(2,gettime[2]%10);//ʱ�ĸ�λ
		
		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;
		}
	}
	else if(displaytime_flag==2)
	{
	
		Display_SEG_Bit(1,gettime[2]/10);//ʱ��ʮλ
		Display_SEG_Bit(2,gettime[2]%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//���ʮλ
		Display_SEG_Bit(8,gettime[0]%10);//��ĸ�λ
		if(shanshuo_flag==0)
		{

		Display_SEG_Bit(4,gettime[1]/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,gettime[1]%10);//�ֵĸ�λ

		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;

		}
	}
	else
	{
		Display_SEG_Bit(1,gettime[2]/10);//ʱ��ʮλ
		Display_SEG_Bit(2,gettime[2]%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,gettime[1]%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		if(shanshuo_flag==0)
		{

		Display_SEG_Bit(7,gettime[0]/10);//���ʮλ
		Display_SEG_Bit(8,gettime[0]%10);//��ĸ�λ
		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;
		}
	}
}
	else
	{
		if(displayalarm_flag==0)
	{
		//����-ʱ
	
		Display_SEG_Bit(1,alarm_hour/10);//ʱ��ʮλ
		Display_SEG_Bit(2,alarm_hour%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,alarm_min%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//���ʮλ
		Display_SEG_Bit(8,alarm_sec%10);//��ĸ�λ
	}
	else if(displayalarm_flag==1)
	{
		//����-ʱ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,alarm_min%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//���ʮλ
		Display_SEG_Bit(8,alarm_sec%10);//��ĸ�λ
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(1,alarm_hour/10);//ʱ��ʮλ
		Display_SEG_Bit(2,alarm_hour%10);//ʱ�ĸ�λ
		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;
		}
	}
	else if(displayalarm_flag==2)
	{
		//����-��
		Display_SEG_Bit(1,alarm_hour/10);//ʱ��ʮλ
		Display_SEG_Bit(2,alarm_hour%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//���ʮλ
		Display_SEG_Bit(8,alarm_sec%10);//��ĸ�λ
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(4,alarm_min/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,alarm_min%10);//�ֵĸ�λ
		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;
		}
	}
	else 
	{
		//����-��
		Display_SEG_Bit(1,alarm_hour/10);//ʱ��ʮλ
		Display_SEG_Bit(2,alarm_hour%10);//ʱ�ĸ�λ
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//�ֵ�ʮλ
		Display_SEG_Bit(5,alarm_min%10);//�ֵĸ�λ
		Display_SEG_Bit(6,16);//-
		if(shanshuo_flag==0)
		{

			Display_SEG_Bit(7,alarm_sec/10);//���ʮλ
			Display_SEG_Bit(8,alarm_sec%10);//��ĸ�λ
		}
		else
		{
				HC138_Select(3);//λѡ
				P0 = 0x00;
		}
	}
}
}
void DelayKTime(unsigned char t)
{
	while(t--)
	{
		alarm_ring_flag=1;
		displaytime();
	}
}
void Key_Scan()
{
	if(S7==0)
	{
		DelayKTime(25);
		if(S7==0)
		{
			while(S7==0)
			{
				alarm_ring_flag=1;
					displaytime();
			}
			if(alarm_ring_flag==0)//��������ʱ���趨ʱ���־��0�����������죬1��������δ��ʱ����߽������ѹر�
			{
					ET1 = 0;
					TR1 = 0;
					ledtime_count = 0;
					led_flag = 0;
					alarm_ring_flag=1;
			}
			else
			{
					if(time_alarm_flag==1)//ʱ��/������ʾ��־��0��ʱ����ʾ   1��������ʾ
					{
						//������S7ʱ,����ʱ��������ʾ�����л�Ϊʱ����ʾ����
							time_alarm_flag = 0;
							starttime();
							displaytime_flag = 0;
							displayalarm_flag = 0;
					}
					else
					{
						//������˰���Ϊʱ���������
							time_alarm_flag = 0;
							displaytime_flag++;
					}
					if((displaytime_flag==1)||(displaytime_flag==2)||(displaytime_flag==3))
					{
							stoptime();
					}
					else if(displaytime_flag==4)
					{
							starttime();
							displaytime_flag = 0;
					}
			}
		}
			
		
	}
	
	
		if(S6==0)
	{
		DelayKTime(25);
		
		if(S6==0)
		{
			while(S6==0)
			{
				alarm_ring_flag=1;
					displaytime();
			}
			if(alarm_ring_flag==0)//��������ʱ���趨ʱ���־��0�����������죬1��������δ��ʱ����߽������ѹر�
			{
					ET1 = 0;
					TR1 = 0;
					ledtime_count = 0;
					led_flag = 0;
					alarm_ring_flag=1;
			}
//			time_alarm_flag = 0;
			else
			{
					if(time_alarm_flag==0)//ʱ��/������ʾ��־��0��ʱ����ʾ   1��������ʾ
					{
						//������S6ʱ,����ʱ��ʱ����ʾ�����л�Ϊ������ʾ����
							time_alarm_flag = 1;
							starttime();
							displayalarm_flag = 0;
							displaytime_flag = 0;
					}
					else
					{
						//������˰���Ϊʱ���������
							time_alarm_flag = 1;
							displayalarm_flag++;
					}
					if(displayalarm_flag==4)
					{
							starttime();
							displayalarm_flag = 0;
						  ledtime_count = 0;//ÿ�����������Ӻ�Ҫ���¼���5s��ʱ��
							ET1 = 1;
					}
		}
		}
	}
	
	
		if(S5==0)
	{
		DelayKTime(25);
		if(S5==0)
		{
			while(S5==0)
			{
				alarm_ring_flag=1;
					displaytime();
			}
			if(alarm_ring_flag==0)//��������ʱ���趨ʱ���־��0�����������죬1��������δ��ʱ����߽������ѹر�
			{
					ET1 = 0;
					TR1 = 0;
					ledtime_count = 0;
					led_flag = 0;
					alarm_ring_flag=1;
			}
			else
			{
					if(displaytime_flag==1)
					{
						gettime[2]++;
						if(gettime[2]==24)
						{
							gettime[2] = 0;
						}
			}
			
			if(displaytime_flag==2)
			{
					gettime[1]++;
					if(gettime[1]==60)
					{
						gettime[1] = 0;
					}
			} 
			if(displaytime_flag==3)
			{
					gettime[0]++;
					if(gettime[0]==60)
					{
						gettime[0] = 0;
					}
			}
			
			if(displayalarm_flag==1)
			{
					alarm_hour++;
					if(alarm_hour==24)
					{
							alarm_hour = 0;
					}
			}
			
			if(displayalarm_flag==2)
			{
					alarm_min++;
					if(alarm_min==60)
					{
							alarm_min = 0;
					}
			}
			if(displayalarm_flag==3)
			{
					alarm_sec++;
					if(alarm_sec==60)
					{
							alarm_sec = 0;
					}
			}
			
		}
	}
	}
			if(S4==0)
			{
				DelayKTime(25);
				if(S4==0)
				{
					while(S4==0)
					{
//				displaytime();
				//��ʾ�¶�
						//alarm_ring_flag=1;
						if((displaytime_flag==0)&&(displayalarm_flag==0))
						{
							temperature = (unsigned int)(read_temperature()); 
							display_temperature();
							readtime();
							Alarm_LED();
						}
					}
			if(alarm_ring_flag==0)//��������ʱ���趨ʱ���־��0�����������죬1��������δ��ʱ����߽������ѹر�
			{
				ET1 = 0;
				TR1 = 0;
				ledtime_count = 0;
				led_flag = 0;
				alarm_ring_flag=1;
			}
			else
			{
				if(displaytime_flag==1)
				{
					if(gettime[2]==0)
					{
						gettime[2] = 24;
					}
					gettime[2]--;
				
				}
			 if(displaytime_flag==2)
			{
					if(gettime[1]==0)
					{
						gettime[1] = 60;
					}
					gettime[1]--;
				
			} 
			if(displaytime_flag==3)
			{
				if(gettime[0]==0)
				{
					gettime[0] = 60;
				}
				gettime[0]--;
				
			}
			
			if(displayalarm_flag==1)
			{
				if(alarm_hour==0)
				{
					alarm_hour = 24;
				}
				alarm_hour--;
			}
			
			
			if(displayalarm_flag==2)
			{
				if(alarm_min==0)
				{
					alarm_min = 60;
				}
				alarm_min--;
			}
			
			if(displayalarm_flag==3)
			{
				if(alarm_sec==0)
				{
					alarm_sec = 60;
				}
				alarm_sec--;
			}
			
		}
		}
	}
}

void stoptime(void)
{
	write(0x8E,0X00);
	write(0x80,gettime[0]|0x80);//��ͣ
	write(0x8E,0x80);

}
void starttime(void)
{
	write(0x8E,0X00);
	write(0x84,gettime[2]);//��ʼ
	write(0x82,gettime[1]);//��ʼ
	write(0x80,gettime[0]&0x7f);//��ʼ
	write(0x8E,0x80);

}
void initTimer0()
{
	TMOD = 0X00;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	
	TH1 = (65536-50000)/256;
	TL1 = (65536-50000)%256;
	EA = 1;
	ET0 =1;
	ET1 =1;
	TR0 = 1;
}

void Timer0()interrupt 1
{
	count++;
	if(count>=20)
	{
		count = 0;
		shanshuo_flag = !shanshuo_flag;
	}
}

void Timer1()interrupt 3
{
	led_count++;
	ledtime_count++;
	if(led_count>=4)//������ʾ����0.2sΪ�����˸
	{
		led_count = 0;
		led_flag = !led_flag;
	}
		if(ledtime_count>=110)//���ӳ���ʱ��Լ5S����׼100Ϊ5s��ʵ��110����ʱ����
	{
		ledtime_count = 0;
		led_flag = 0;
		alarm_ring_flag = 1;
		TR1 = 0;
		TH1 = (65536-50000)/256;
		TL1 = (65536-50000)%256;
	}
}
void display_temperature()
{
	if(temperature!=8500)
	{
			Display_SEG_Bit(6,temperature/10%10);
			Display_SEG_Bit(7,temperature%10);
			Display_SEG_Bit(8,12);
	}
}
void Alarm_LED()
{
	if((gettime[2]==alarm_hour)&&(gettime[1]==alarm_min)&&(gettime[0]==alarm_sec))
	{
		alarm_ring_flag = 0;
		//ET1 = 1;
		TR1 = 1;
	}
	if(alarm_ring_flag==0)//��������ʱ���趨ʱ���־��0�����������죬1��������δ������ʱ����߽������ѹرջ��߳������쳬��5s
	{
		if(led_flag==1)
		{
			P0 = 0XFF;
			HC138_Select(1);
			LED1 = 0;
		}
		else
		{
			P0 = 0XFF;
			HC138_Select(1);
			LED1 = 1;
		}
	}
	else
	{
		P0 = 0XFF;
		HC138_Select(1);
		LED1 = 1;
	}
}
