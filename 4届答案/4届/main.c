/*******************************************************************************  
* �ļ����ƣ�ͨ��ʱ��оƬDS1302��ʾʱ��ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪBTN��J2����Ϊ1-3��2-4
* ʵ�����󣺰���S7�����л�ʱ�������ڵ���ʾ��S6��ͣʱ���ʱ
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include "stc15.h"
#include "ds1302.h"
#include "iic.h"
void ds1302_init();
void readtime();
void stoptime(void);
void starttime(void);
unsigned char  code inittime[] = {57,30,8,5,1,3,22};//��-��-ʱ-��-��-����-���
unsigned char gettime[7];
unsigned char  code SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
};
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit RELAY  = P0^4;
sbit BUZZER = P0^6;
unsigned char Humidity_threshold = 50;
unsigned char Humidity;
unsigned char mode_flag;//0���Զ�ģʽ   1���ֶ�ģʽ
unsigned char alarm_flag;//0:�����������ѹ��� 1���ط��������ѹ���
unsigned char relay_flag;//0:�ؼ̵��� 1�����̵���
unsigned char threshold_flag;//0:��ֵ������� 1�����ڵ���
void Key_Scan();
float Read_AIN3();
unsigned char Read_AT24C02(unsigned char addr);
void Write_AT24C02(unsigned char addr,unsigned char dat);
void HC138_Select(unsigned char i);
void DelayK(unsigned char t);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);
unsigned char Read_Humidity();
void Display_Humidity_Time();
void Display_Threshold();
void Display();
void LED_Control();
void RELAY_Control();
void ALARM_Control();
void main()
{
	HC138_Select(4);
	P0=0X00;
	HC138_Select(1);
	P0=0XFF;
	Humidity_threshold = Read_AT24C02(0x00);
	ds1302_init();
	while(1)
	{
		readtime();
		Humidity = Read_Humidity();
		Key_Scan();
		Display();
		LED_Control();
		RELAY_Control();
		ALARM_Control();
	}
}
void RELAY_Control()
{
	if(mode_flag==1)
	{
		if(relay_flag==1)
		{
			HC138_Select(4);
			RELAY = 1;
			BUZZER = 0;

			P2&=0x1f;
			
		}
		else
		{	
			HC138_Select(4);
			RELAY = 0;
			BUZZER = 0;

			P2&=0x1f;
		}
	}
	else
	{
		if(threshold_flag==0)
		{
			if(Humidity < Humidity_threshold)
			{	
				HC138_Select(4);
				RELAY = 1;
				BUZZER = 0;
				
				P2&=0x1f;
			}
			else
			{
				HC138_Select(4);
				RELAY = 0;
				BUZZER = 0;

				P2&=0x1f;
			}
		}
		else
		{	
			HC138_Select(4);
			RELAY = 0;
			BUZZER = 0;

			P2&=0x1f;
		}
	}
}
void ALARM_Control()
{
	if(mode_flag==1)
	{
		if(alarm_flag==0)
		{
			if(Humidity < Humidity_threshold)
			{
				HC138_Select(4);
				BUZZER = 1;
//				RELAY = 0;
				P2&=0x1f;
			}
		}
		else
		{
			HC138_Select(4);
			BUZZER = 0;
//			RELAY = 0;
			P2&=0x1f;
		}
	}
	else
	{
		HC138_Select(4);
		BUZZER = 0;
//		RELAY = 0;
		P2&=0x1f;
	}
}
void LED_Control()
{
	if(mode_flag==0)
	{
		HC138_Select(1);
		P0=0XFF;
		L1 = 0;
	}
	else
	{
		HC138_Select(1);
		P0=0XFF;
		L2 = 0;
	}
}
void Display()
{
	if((mode_flag==0)&&(threshold_flag==1))
	{
		Display_Threshold();
	}
	else
	{
		Display_Humidity_Time();
	}
}
void Display_Humidity_Time()
{
	Display_SEG_Bit(1,gettime[2]/10%10);
	Display_SEG_Bit(2,gettime[2]%10);
	Display_SEG_Bit(3,16);
	Display_SEG_Bit(4,gettime[1]/10%10);
	Display_SEG_Bit(5,gettime[1]%10);
	Display_SEG_Bit(7,Humidity/10%10);
	Display_SEG_Bit(8,Humidity%10);
}
void Display_Threshold()
{
	Display_SEG_Bit(1,16);
	Display_SEG_Bit(2,16);
	Display_SEG_Bit(7,Humidity_threshold/10%10);
	Display_SEG_Bit(8,Humidity_threshold%10);
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
		write(0x8e,0x00);
		for(i=0;i<7;i++)
		{
			gettime[i] = read(add);
			add = add+2;
		}
		write(0x8e,0x80);
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
void DelayK(unsigned char t)
{
	while(t--)
	{
	}
}
void Key_Scan()
{
	if(S7==0)
	{
		DelayK(20);
		if(S7==0)
		{
			while(S7==0)
			{
				Display();
			}
			mode_flag = !mode_flag;
			if(mode_flag==1)
			{
				threshold_flag = 0;
			}
		}
	}
	
		if(S6==0)
	{
		DelayK(20);
		if(S6==0)
		{
			while(S6==0)
			{
				Display();
			}
			if(mode_flag==0)//�Զ�ģʽ�½���/�˳�ʪ����ֵ��������
			{
				threshold_flag = !threshold_flag;
			}
			else
			{
				alarm_flag = !alarm_flag;
			}
		}
	}

		if(S5==0)
	{
		DelayK(20);
		if(S5==0)
		{
			while(S5==0)
			{
				Display();
			}
			if(mode_flag==1)
			{
					//�ֶ�ģʽ�¿��̵���
				relay_flag = 1;
			}
			else
			{
					//�Զ�ģʽ����ֵ
				if(threshold_flag==1)
				{
					if(Humidity_threshold<=98)
						Humidity_threshold++;
				}
				Write_AT24C02(0x00,Humidity_threshold);
			}
		}
	}
	if(S4==0)
	{
		DelayK(20);
		if(S4==0)
		{
			while(S4==0)
			{
				Display();
			}
			if(mode_flag==1)
			{
				//�ֶ�ģʽ�¹ؼ̵���
				relay_flag = 0;
			}
			else
			{
				//�Զ�ģʽ����ֵ
				if(threshold_flag==1)
				{
					if(Humidity_threshold>=1)
						Humidity_threshold--;
				}
				//EEPROM�洢��ֵ
				Write_AT24C02(0x00,Humidity_threshold);
			}
			
		}
	}		
}
unsigned char Read_Humidity()
{
	unsigned char temp = 0;
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
	return (unsigned char)(temp*5.0/255*19.8);
}
void Write_AT24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0xA0);//AT24C02��д�豸��ַ 
	IIC_WaitAck();//�ȴ��ӻ�PCF8591Ӧ��
	IIC_SendByte(addr);//Ҫд����ڴ��ַ
	IIC_WaitAck();//�ȴ��ӻ�PCF8591Ӧ��
	IIC_SendByte(dat);//Ҫд�������
	IIC_WaitAck();//�ȴ��ӻ�PCF8591Ӧ��
	IIC_Stop();//IIC����ֹͣ�ź� 
}
unsigned char Read_AT24C02(unsigned char addr)
{
	unsigned char temp;
	//����5��Ϊαд����
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0xA0); //AT24C02��д�豸��ַ
	IIC_WaitAck();//�ȴ��ӻ�PCF8591��Ӧ��
	IIC_SendByte(addr);//Ҫ��ȡ���ڴ��ַ
	IIC_WaitAck();//�ȴ��ӻ�PCF8591Ӧ��
	
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0xA1);//AT24C02�Ķ��豸��ַ 
	IIC_WaitAck();//�ȴ��ӻ�PCF8591��Ӧ��
	temp = IIC_RecByte();//��ȡĿ������
	IIC_SendAck(1);//����һ����Ӧ���ź�
	IIC_Stop(); //IIC����ֹͣ�ź� 
	return temp;
}


