/*******************************************************************************  
* �ļ����ƣ�ͨ��������0�Ͷ�ʱ��1�ɼ�NF555���ɵ�����Ƶ��ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪBTN��J3����ΪSIGNAL-P34���ɰ���J2����һ����ñ���ɣ�
* ʵ������ͨ��������0�Ͷ�ʱ��1�ɼ�NF555���ɵ�����Ƶ��
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include "stc15.h"
#include "iic.h"
unsigned char code SMG_duanma[19]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0xc1   //U
};
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;
sbit LED5 = P0^4;
unsigned char display_flag;//�����л���־λ
unsigned char xianshi_flag;//������Ƿ���ʾ��־λ
unsigned char ledgongneng_flag;
unsigned char LED1_FLAG,LED2_FLAG,LED3_FLAG,LED4_FLAG,LED5_FLAG;
unsigned char pout_flag;
unsigned char count_t = 0;
unsigned int count_f = 0;
unsigned int dat_f = 0;
void Key_Scan();
unsigned char Read_AIN3();
void pout_dianya_control();
unsigned int AIN3_Int;
void display_fre();
void display_AIN3();
void display_AIN3_FRE();
void control_LED34();
void control_LEDALL();
void Put_DA(unsigned char temp);
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);
void Delay_K(unsigned char times);
void initTimer();
void Key_Scan();
void main()
{
		HC138_Select(4);
		P0=0X00;
		HC138_Select(1);
		P0=0XFF;
		initTimer();
		while(1)
		{
			AIN3_Int = (unsigned int)((Read_AIN3()*5.0/255)*100);
			pout_dianya_control();
			Key_Scan();
			control_LED34();
			control_LEDALL();
			display_AIN3_FRE();
		}
}
void control_LEDALL()
{
	if(ledgongneng_flag==0)
	{
	if(LED1_FLAG==0)
	{
		HC138_Select(1);
		LED1=1;
	}
	else
	{
		HC138_Select(1);
		LED1=0;
	}
	
	
	
		if(LED2_FLAG==0)
	{
		HC138_Select(1);
		LED2=1;
	}
	else
	{
		HC138_Select(1);
		LED2=0;
	}
	
		if(LED3_FLAG==0)
	{
		HC138_Select(1);
		LED3=1;
	}
	else
	{
		HC138_Select(1);
		LED3=0;
	}
	
	
		if(LED4_FLAG==0)
	{
		HC138_Select(1);
		LED4=1;
	}
	else
	{
		HC138_Select(1);
		LED4=0;
	}
	
	
		if(LED5_FLAG==0)
	{
		HC138_Select(1);
		LED5=1;
	}
	else
	{
		HC138_Select(1);
		LED5=0;
	}
	}
	
	else
	{
		HC138_Select(1);
		LED1=1;
		LED2=1;
		LED3=1;
		LED4=1;
		LED5=1;
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
void control_LED34()
{
	if(AIN3_Int<150)
	{
		LED3_FLAG = 0;
	}
	else if((AIN3_Int>=150)&&(AIN3_Int<250))
	{
		LED3_FLAG = 1;
	}
	else if((AIN3_Int>=250)&&(AIN3_Int<350))
	{
		LED3_FLAG = 0;
	}
	else
	{
		LED3_FLAG = 1;
	}
	
	if(dat_f<1000)
	{
		LED4_FLAG = 0;
	}
	else if((dat_f>=1000)&&(dat_f<5000))
	{
		LED4_FLAG = 1;
	}
	else if((dat_f>=5000)&&(dat_f<10000))
	{
		LED4_FLAG = 0;
	}
	else
	{
		LED4_FLAG = 1;
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
void initTimer()
{
	TMOD = 0X04;
	TL0 = 0XFF;
	TH0 = 0XFF;
	TL1 = (65536-50000)%256;
	TH1 = (65536-50000)/256;
	EA = 1;
	ET0 = 1;
	ET1 = 1;
	TR0 = 1;
	TR1 = 1;
}
void Timer0 () interrupt 1
{
	count_f++;
}
void Timer1() interrupt 3
{
	count_t++;
	if(count_t==20)
	{
		dat_f = count_f;
		count_t = 0;
		count_f = 0;
	}
}
void display_fre()
{
		Display_SEG_Bit(1,15);
		if(dat_f>9999)
		{
			Display_SEG_Bit(4,dat_f/10000%10);
		}
		if(dat_f>999)
		{
			Display_SEG_Bit(5,dat_f/1000%10);
		}
		if(dat_f>99)
		{
			Display_SEG_Bit(6,dat_f/100%10);
		}
		if(dat_f>9)
		{
			Display_SEG_Bit(7,dat_f/10%10);
		}
		if(dat_f>0)
		{
			Display_SEG_Bit(8,dat_f%10);
		}

}
void display_AIN3_FRE()
{
	if(xianshi_flag==0)
	{
		if(display_flag==0)
		{
			display_AIN3();
			LED1_FLAG = 1;
			LED2_FLAG = 0;
		}
		else
		{
			display_fre();
			LED1_FLAG = 0;
			LED2_FLAG = 1;
		}
	}
	else
	{
		//�ر������
		HC138_Select(3);//λѡ
		P0 = 0x00;
		HC138_Select(2);//��ѡ
		P0 = 0xff;
		DelaySMG(1);
	}
}
void display_AIN3()
{
	Display_SEG_Bit(1,18);
	
	//Display_SEG_Bit(6,18);//С����
	HC138_Select(3);//λѡ
	P0 = 0x00;
	P0 = 0x01<<(6-1);
	HC138_Select(2);//��ѡ
	P0 = 0xff;
	P0 = SMG_duanma[AIN3_Int/100%10]&(~0x80);
	DelaySMG(1);
	
	Display_SEG_Bit(7,AIN3_Int/10%10);//С����
	Display_SEG_Bit(8,AIN3_Int%10);//С����
}
unsigned char Read_AIN3()
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
	return temp;
}
void Put_DA(unsigned char temp)
{
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0x90);//����PCF8591��д�豸��ַ
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_SendByte(0x40);//���Ϳ����ֽڣ�ѡ��DAģʽ
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_SendByte(temp);//����
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_Stop();//IIC������ʼ�ź�
}
void Delay_K(unsigned char times)
{
	while(times--)
	{
		display_AIN3_FRE();
	}
}
void Key_Scan()
{
	if(S4==0)
	{
		Delay_K(20);
		if(S4==0)
		{
			while(S4==0)
			{
				display_AIN3_FRE();
			}
			display_flag = !display_flag;
		}
	}
		if(S5==0)
	{
		Delay_K(20);
		if(S5==0)
		{
			while(S5==0)
			{
				display_AIN3_FRE();
			}
			pout_flag = !pout_flag;
		}
	}
	
		if(S7==0)
	{
		Delay_K(20);
		if(S7==0)
		{
			while(S7==0)
			{
				display_AIN3_FRE();
			}
			xianshi_flag = !xianshi_flag;
		}
	}
	
	
	if(S6==0)
	{
		Delay_K(20);
		if(S6==0)
		{
			while(S6==0)
			{
				display_AIN3_FRE();
			}
			ledgongneng_flag = !ledgongneng_flag;
		}
	}
}
void pout_dianya_control()
{
	if(pout_flag==0)
	{
		Put_DA(2*51);
		LED5_FLAG = 0;
	}
	else
	{
		Put_DA((AIN3_Int/100.0)*51);
		LED5_FLAG = 1;
	}
}