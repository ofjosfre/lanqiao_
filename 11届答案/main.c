/*******************************************************************************  
* �ļ����ƣ��������ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪKBD��J2����Ϊ1-3��2-4
* ʵ�����󣺵����²��ɿ�����ʱ��1��2���������ʾ��Ӧ�ļ�ֵ����ֵ��ΧΪ0-15
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include <stc15.h>
#include "iic.h"
unsigned char code SMG_duanma[21]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0x8C,0xc1,0xc8
};//- . P U n
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;

sbit LED5 = P0^4;
sbit LED6 = P0^5;
sbit LED7 = P0^6;
sbit LED8 = P0^7;
sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
sbit C4 = P3^4;

unsigned char LED1_FLAG,LED2_FLAG;
unsigned char display_flag;
unsigned char keyVal=0;
float Vp;
unsigned char count_cmp;
unsigned int LED1_count ;
unsigned int LED3_count ;
float value_read_AIN3;
//float compare_AIN3;
void Display();
void KBD_Scan();
void LED_Control();
void display_can();
void HC138_Select(unsigned char i);
void display_ain3();
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelayK(unsigned char t);
void delay_seg(unsigned int times);
void display_count();
void Key_Scan();
void Function_Key();
void Dianya_Comp();
void Get_Time5s();
unsigned char Read_AIN3();
unsigned char Read_AT24C02(unsigned char addr);
void Write_AT24C02(unsigned char addr,unsigned char dat);
void initTimer0();
void LED_Control();
void main()
{
	HC138_Select(4);
	P0=0X00;//�ط��������̵���
	HC138_Select(1);
	P0=0XFF;//�ص�
	initTimer0();
	while(1)
	{
		Vp = Read_AT24C02(0x00);//��ȡAT24C02�д洢�ĵ�ѹ����
		if(Vp>50)
		{
			Vp = 50;
		}
		Vp = Vp/10.0;//��С10�����������Ĳ���
		value_read_AIN3 = Read_AIN3()*5.0/255;//��ȡģ���ѹ
		Function_Key();//����ɨ�衢������ �����ʾ�����л�����ѹ��������������ֵ��0��
		Get_Time5s();//���ݶ�ȡ���ĵ�ѹֵ���5S�ļ�ʱ
		LED_Control();//����L1��L2��L3��
		Dianya_Comp();//�ж϶�ȡ�ĵ�ѹֵ�Ƿ����½��ҵ��ڵ�ѹ����
		Display();//������ʾ
	}
}
void LED_Control()
{
	if(LED1_FLAG==1)
	{
		HC138_Select(1);//λѡ
		LED1 = 0;
	}
	else
	{
		HC138_Select(1);//λѡ
		LED1 = 1;
	}
	if(LED2_FLAG==1)
	{
		HC138_Select(1);//λѡ
		LED2 = 0;
	}
	else
	{
		HC138_Select(1);//λѡ
		LED2 = 1;
	}
	if(LED3_count >= 3)
	{
		HC138_Select(1);//λѡ
		LED3 = 0;
	}
	else if(LED3_count == 0)
	{
		HC138_Select(1);//λѡ
		LED3 = 1;
	}
	else
	{
		
	}
}
void Display()
{
		if(display_flag==0)
		{
			display_ain3();
		}
		else if(display_flag==1)
		{
			display_can();
		}
		else if(display_flag==2)
		{
			 display_count();
		}
		else
		{
			
		}
		
}
void Key_Scan()
{
	unsigned char i;
	C1 = C2 = C3 = C4 = 1;//ÿ���ж�ǰ��ȫ����������
	for(i=0;i<4;i++)
	{
		P3 |= 0x0f;
		P3 &= ~(1<<i);//���ΰ�ÿ�����ͣ�ÿ��ֻ��һ�����ͣ�����Ϊ��
		if(C1==0)
		{
			DelayK(20);
			if(C1==0)
			{
				while(C1==0)
				{
					Display();
				}
				keyVal = i*4+1;
				break;
			}
		}
		else if(C2==0)
		{
			DelayK(20);
			if(C2==0)
			{
				while(C2==0)
				{
					Display();
				}
				keyVal = i*4+2;
				break;
			}
		}
		else if(C3==0)
		{
			DelayK(20);
			if(C3==0)
			{
				while(C3==0)
				{
					Display();
				}
				keyVal = i*4+3;
				break;
			}
		}
		else if(C4==0)
		{
			DelayK(20);
			if(C4==0)
			{
				while(C4==0)
				{
					Display();
				}
				keyVal = i*4+4;
				break;
			}
		}
	}
	
	//return keyVal;

}
void Function_Key()
{

		Key_Scan();
		if(keyVal == 15)
		{
			keyVal = 0;
			LED3_count = 0;
			display_flag++;
			if(display_flag==3)
			{
				display_flag = 0;
			}
		}
		else if(keyVal==16)//����+
		{
			keyVal = 0;
			if(display_flag==1)//��Ч����
			{
				//Vp = Vp + 0.5;
				LED3_count = 0;
				if(Vp<5)
				{
					Vp = Vp + 0.5;
				}
				else
				{
					Vp = 0;
				}
				Write_AT24C02(0x00,(unsigned char)(Vp*10));
				
			}
			else//��Ч����
			{
				LED3_count++;
			}
		}
		else if(keyVal==12)//����-
		{
			keyVal = 0;
			if(display_flag==1)//��Ч����
			{
				//Vp = Vp - 0.5;
				LED3_count = 0;
				if(Vp>0)
				{
					Vp = Vp - 0.5;
				}
				else
				{
					Vp = 5;
				}
				Write_AT24C02(0x00,(unsigned char)(Vp*10));
			}
			else//��Ч����
			{
				LED3_count++;
			}
		}
		else if(keyVal==11)//��0
		{
			keyVal = 0;
			if(display_flag==2)//��Ч����
			{
				LED3_count = 0;
				count_cmp = 0;
			}
			else//��Ч����
			{
				LED3_count++;
			}
		}
		else if(keyVal!=0)//��Ч����
		{				
			keyVal = 0;
			LED3_count++;
		}
}
void Dianya_Comp()
{
	static int bd_flag;//����5V
	static int bx_flag;//����5V
	if(Vp==5)
	{
		if((unsigned int)(value_read_AIN3*100)<500)
		{
			bd_flag = 1;
		}
		if((bd_flag==1)&&((unsigned int)(value_read_AIN3*100)==(unsigned int)(Vp*100)))
		{
			bd_flag = 0;
			count_cmp++;
			if(count_cmp==100)
			{
				count_cmp = 99;
			}
		}
	}
	else
		{
			if((unsigned int)(value_read_AIN3*100)>(unsigned int)(Vp*100))
			{
				bx_flag = 1;
			}
			if((bx_flag == 1)&&((unsigned int)(value_read_AIN3*100)<=(unsigned int)(Vp*100)))
			{
			//����ֵ��1
				bx_flag = 0;
				count_cmp++;
				if(count_cmp==100)
				{
					count_cmp = 99;
				}
			}
		}
	if(count_cmp%2==1)
	{
		LED2_FLAG = 1;
	}
	else
	{
		LED2_FLAG = 0;
	}
}
void  Get_Time5s()
{
	if(value_read_AIN3<Vp)
	{
		TR0 = 1;
	}
	else
	{
		TR0 = 0;
		LED1_count = 0;
		LED1_FLAG = 0;
	}
}
void Display_SEG_Bit(unsigned char pos,unsigned char value)
{
	HC138_Select(3);//λѡ
	P0 = 0x00;//�ر����������
	P0 = (0x01<<(pos-1));
	HC138_Select(2);//��ѡ
	P0 = 0xFF;//�ر����ж�ѡ
	P0 = SMG_duanma[value];
	delay_seg(1);
  P0 = 0xFF;//��Ӱ
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

void delay_seg(unsigned int times)		//@12.000MHz
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
void DelayK(unsigned char t)
{
	while(t--)
	{
		Display();
	}
}
void display_ain3()
{
	unsigned int value_int_AIN3;
	value_int_AIN3 = (unsigned int)(value_read_AIN3*100);//3.67 367
	Display_SEG_Bit(1,19);
//	Display_SEG_Bit(6,19);
	HC138_Select(3);//λѡ
	P0 = 0x00;//�ر����������
	P0 = (0x01<<(6-1));
	HC138_Select(2);//��ѡ
	P0 = 0xFF;//�ر����ж�ѡ
	P0 = SMG_duanma[value_int_AIN3/100%10]&(~0x80);
	delay_seg(1);
	P0 = 0xFF;
	
	Display_SEG_Bit(7,value_int_AIN3/10%10);
	Display_SEG_Bit(8,value_int_AIN3%10);
}
void display_can()
{
	unsigned int Vp_can;
	Vp_can = (unsigned int)(Vp*100);
	Display_SEG_Bit(1,18);
//	Display_SEG_Bit(6,19);
	HC138_Select(3);//λѡ
	P0 = 0x00;//�ر����������
	P0 = (0x01<<(6-1));
	HC138_Select(2);//��ѡ
	P0 = 0xFF;//�ر����ж�ѡ
	P0 = SMG_duanma[Vp_can/100%10]&(~0x80);
	delay_seg(1);
	
	Display_SEG_Bit(7,Vp_can/10%10);
	Display_SEG_Bit(8,Vp_can%10);
}
void display_count()
{
	Display_SEG_Bit(1,20);
	Display_SEG_Bit(7,count_cmp/10%10);
	Display_SEG_Bit(8,count_cmp%10);
}
void Write_AT24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xA0); 
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop(); 
}
unsigned char Read_AT24C02(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xA0); 
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1); 
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop(); 
	return temp;
}
unsigned char Read_AIN3()
{
	unsigned char temp = 0;
	unsigned char i;
	for(i=0;i<2;i++)
	{
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0x90);//����PCF8591��д������ַ
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_SendByte(0x03);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��3)��
	//IIC_SendByte(0x01);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��1)��
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_Stop();//IIC������ʼ�ź�
	
	IIC_Start();
	IIC_SendByte(0x91);//����PCF8591�Ķ�������ַ
	IIC_WaitAck();
	temp = IIC_RecByte();//��ȡPCF8591ͨ�������� 
	IIC_SendAck(1);//������Ӧ���ź�
	IIC_Stop();
	}
	return temp;
}
void initTimer0()
{
	TMOD = 0X00;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	EA = 1;
	ET0 =1;
//	TR0 = 1;
}
void Timer0()interrupt 1
{
	LED1_count++;
	if(LED1_count>100)
	{
		LED1_count = 0;
		LED1_FLAG = 1;
	}
}