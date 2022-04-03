/*******************************************************************************  
* �ļ����ƣ��������ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪKBD��J2����Ϊ1-3��2-4
* ʵ�����󣺵����²��ɿ�����ʱ��1��2���������ʾ��Ӧ�ļ�ֵ����ֵ��ΧΪ0-15
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include <stc15.h>
#include "iic.h"
#include "onewire.h"
unsigned char code SMG_duanma[19]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0x8C
};
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;
sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
sbit C4 = P3^4;
unsigned int temperature;
unsigned int temp_can = 25;
float dianya;
unsigned char display_flag;
unsigned char mode_flag;
unsigned char keyVal=0;
void Display();
void display_temp_can();
void HC138_Select(unsigned char i);
void display_temperature();
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelayK(unsigned char t);
void delay_seg(unsigned int times);
void display_dac();
void Key_Scan();
void Function_Key();
void Panduan_Mode();
void Pout_DAC(unsigned char temp);
void main()
{
	
	HC138_Select(4);
	P0=0X00;//��ʼ������--�ط��������ؼ̵���
	HC138_Select(1);
	P0=0XFF;//��ʼ������--�ص�
	while(1)
	{
		temperature = (unsigned int)(read_temperature()*100); //�ɼ��¶ȡ�������Ŵ�100��
		//display_temperature();
		Function_Key();//���󰴼������жϺ���
		Display();//������ʾ����
		Panduan_Mode();//���ݲ�ͬ��ģʽ �����ѹ��ͬ
		
	}
}
//�л���ʾ����
void Display()
{
		if(display_flag==0)
		{
			display_temperature();
		}
		else if(display_flag==1)
		{
			display_temp_can();
		}
		else if(display_flag==2)
		{
			 display_dac();
		}
		else
		{
			
		}
		
}
void Function_Key()
{
		Key_Scan();//����ɨ��
		if(keyVal == 13)
		{
			keyVal = 0;
			display_flag++;//S4�����л���ʾ�Ľ���
			if(display_flag==3)
			{
				display_flag = 0;
			}
		}
		if(keyVal == 9)//�л�ģʽ
		{
			keyVal = 0;
			mode_flag = !mode_flag;
		}
		if(display_flag==1)//��֤�ӡ����������ڲ������ý�����Ч
		{
			if(keyVal == 10)//+++
			{
				keyVal = 0;
				if(temp_can<=98)//�������ñ߽� ����������ӵ�99
					temp_can++;
			}
			if(keyVal == 14)//----
			{
				keyVal = 0;
				if(temp_can>=1)//�������ñ߽� ������С��С��0
					temp_can--;
			}
		}
}
void Panduan_Mode()
{
	if(mode_flag==0)//ģʽ1
	{
		HC138_Select(1);
		LED1 = 0;
		if(temperature!=8500)//��ֵDS18B20��ת��ʱδת����ɿ��ܳ��ֵ�ֵ
		{
			if(temperature<(temp_can*100))
			{
				dianya = 0;
			}
			else
			{
				dianya = 5;
			}
		}
	}
	else
	{
		HC138_Select(1);
		LED1 = 1;
		if(temperature!=8500)
		{
			if(temperature<=2000)
			{
				dianya = 1;
			}
			else if(temperature>=4000)
			{
				dianya = 4;
			}
			else
			{
				dianya = temperature/100.0 * 0.15-2;
			}
		}
	}
	//temp_dianya = (int)(dianya*100);
	Pout_DAC(dianya*51);
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
			DelayK(10);
			if(C1==0)
			{
				while(C1==0)
				{
					Display();
					Panduan_Mode();
				}
				keyVal = i*4+1;
				break;
			}
		}
		else if(C2==0)
		{
			DelayK(10);
			if(C2==0)
			{
				while(C2==0)
				{
					Display();
					Panduan_Mode();
				}
				keyVal = i*4+2;
				break;
			}
		}
		else if(C3==0)
		{
			DelayK(10);
			if(C3==0)
			{
				while(C3==0)
				{
					Display();
					Panduan_Mode();
				}
				keyVal = i*4+3;
				break;
			}
		}
		else if(C4==0)
		{
			DelayK(10);
			if(C4==0)
			{
				while(C4==0)
				{
					Display();
					Panduan_Mode();
				}
				keyVal = i*4+4;
				break;
			}
		}
	}

}
void DelayK(unsigned char t)
{
	while(t--)
	{
		Display();
		Panduan_Mode();
	}
}
void display_temperature()
{
	
	HC138_Select(1);
	LED2 = 0;
	LED3 = 1;
	LED4 = 1;
	Display_SEG_Bit(1,12);
	if(temperature!=8500)//23.45 2345
	{
			Display_SEG_Bit(5,temperature/1000%10);
			/*��ʾС����*/
			HC138_Select(3);//λѡ
			P0 = 0x00;
			P0 = 0x01<<(6-1);
			HC138_Select(2);//��ѡ
			P0 = 0xff;
			P0 = SMG_duanma[temperature/100%10]&(~0x80);
			delay_seg(1);
			P0 = 0xFF;
			Display_SEG_Bit(7,temperature/10%10);
			Display_SEG_Bit(8,temperature%10);

	}
}
void display_temp_can()
{
	HC138_Select(1);
	LED2 = 1;
	LED3 = 0;
	LED4 = 1;
	Display_SEG_Bit(1,18);
	Display_SEG_Bit(7,temp_can/10%10);
	Display_SEG_Bit(8,temp_can%10);
}
void display_dac()
{
	unsigned int temp_dianya = 0;
	HC138_Select(1);
	LED2 = 1;
	LED3 = 1;
	LED4 = 0;
	temp_dianya = (unsigned int)(dianya*100);//3.67 367
	Display_SEG_Bit(1,10);
	
	/*��ʾС����*/
	HC138_Select(3);//λѡ
	P0 = 0x00;
	P0 = 0x01<<(6-1);
	HC138_Select(2);//��ѡ
	P0 = 0xff;
	P0 = SMG_duanma[temp_dianya/100%10]&(~0x80);
	delay_seg(1);
	P0 = 0xff;
	Display_SEG_Bit(7,temp_dianya/10%10);
	Display_SEG_Bit(8,temp_dianya%10);
	
}
void Pout_DAC(unsigned char temp)
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