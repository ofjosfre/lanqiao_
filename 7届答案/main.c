/*******************************************************************************  
* �ļ����ƣ�PWMʵ��LED���ȵ���ʵ��
* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪBTN��J2����Ϊ1-3��2-4
* ʵ�����󣺰���S7������LED��������
*						����S6������LED���ȼ�С
* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
*******************************************************************************/
#include <stc15.h>
#include "onewire.h"
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
unsigned char code SMG_duanma[19]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0x8C
};
void initTimer(void);
//void Delay10ms();
void DelayK(unsigned char t);
void Key_Scan();
void Fan_ModeControl();
void LED_Control();
void FAN_Control();
void Display();
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
unsigned char fan_mode = 1;
unsigned char pwm_count=0;
unsigned int  time_ms_count=0;
unsigned int  time_s_count=0;
unsigned char  time_flag=1;
unsigned char PWM_MAX = 20;
unsigned int  TIM_MAX=0;//300
unsigned char remain_time;
unsigned char pwm_fan_flag;
unsigned char display_flag;
unsigned int temperature;
void main()
{
	
	HC138_Select(4);
	P0=0X00;
	HC138_Select(1);
	P0=0XFF;
	initTimer();
	while(1)
	{
		temperature = (unsigned int)(read_temperature()); 
		Key_Scan();
		LED_Control();
		Fan_ModeControl();
		Display();
		FAN_Control();
	}
}
void LED_Control()
{
	if(time_flag==0)//�Ƿ�ʣ��ʱ���־λ 0������ʣ��ʱ��  1����ʣ��ʱ��
	{
		if(remain_time>0)//�����ʣ��ʱ��(��λS)
		{
				if(fan_mode==1)
				{
						HC138_Select(1);
						P0 = 0Xff;
						L1 = 0;
				}
				else if(fan_mode==2)
				{
						HC138_Select(1);
						P0 = 0Xff;
						L2 = 0;
				}
				else
				{
						HC138_Select(1);
						P0 = 0Xff;
						L3 = 0;
				}
		}
		else//��ʣ��ʱ��ʱ ȫ��LED����
		{
				HC138_Select(1);
				P0 = 0Xff;
		}
	}
	else//��ʣ��ʱ��ʱ ȫ��LED����
	{
				HC138_Select(1);
				P0 = 0Xff;
	}
}
void Key_Scan()
{
	if(S5==0)
	{
			DelayK(20);
			if(S5==0)
			{
					while(S5==0)
					{
						Display();
					}
					TIM_MAX = TIM_MAX + 60;
					if(TIM_MAX==180)
					{
						TIM_MAX = 0;
					}
					time_ms_count = 0;
					time_s_count = 0;
					if(TIM_MAX!=0)
					{
						time_flag = 0;	
					}
			}
		}
		if(S4==0)//ģʽ�л�
		{
			DelayK(20);
			if(S4==0)
			{
					while(S4==0)
					{
						Display();
					}
					fan_mode++;
					if(fan_mode>=4)
					{
						fan_mode = 1;
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
					time_ms_count = 0;
					time_s_count = 0;
					time_flag  = 1;
				}
				
		}
		if(S7==0)
		{
				DelayK(20);
				if(S7==0)
				{
					while(S7==0)
					{
						Display();
					}
					display_flag = !display_flag;
				}
				
		}

}
void Display()
{
	if(display_flag==0)
	{
			Display_SEG_Bit(1,16);
			Display_SEG_Bit(3,16);
			Display_SEG_Bit(2,fan_mode);
			Display_SEG_Bit(5,remain_time/1000%10);
			Display_SEG_Bit(6,remain_time/100%10);
			Display_SEG_Bit(7,remain_time/10%10);
			Display_SEG_Bit(8,remain_time%10);
	}
	else
	{
			Display_SEG_Bit(1,16);
			Display_SEG_Bit(3,16);
			Display_SEG_Bit(2,4);
			Display_SEG_Bit(6,temperature/10%10);
			Display_SEG_Bit(7,temperature%10);
			Display_SEG_Bit(8,12);
	}
}
void Fan_ModeControl()
{
	if(fan_mode==1)
	{
		PWM_MAX = 20;
	}
	else if(fan_mode==2)
	{
		PWM_MAX = 30;
	}
	else
	{
		PWM_MAX = 70;
	}
}
void FAN_Control()
{
	if(time_flag==0)//��ʱʣ��ʱ���־λ 0������ʣ��ʱ��  1����ʣ��ʱ��
	{
			remain_time = TIM_MAX-time_s_count;//���㶨ʱʣ��ʱ��
			if(pwm_fan_flag==1)
			{
				P34 = 1;
			}
			else
			{
				P34 = 0;
			}
	}
	else//��ʣ��ʱ��ʱ P34�͵�ƽ
	{
			remain_time = 0;
			P34 = 0;
	}
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
void initTimer(void)
{
	//��ʱ10us
	TMOD=0X00;//������ʱ������ʱ��������ģʽ0����16λ�Զ���װ�ض�ʱ��
	TH0=(65536-10)/256;//��ʱ��0��8λ�Ĵ���
	TL0=(65536-10)%256;
	EA=1;
	ET0=1;//��ʱ��0�жϿ���
	TR0=1;//��ʱ��0��ʼ����
}
void Timer0() interrupt 1
{
	pwm_count++;
	if(pwm_count==100)//ʵ�ֶ�ʱ����T=10*100=1000us=1ms����Ƶ��f=1KHz
	{
		pwm_count=0;
		if(TIM_MAX!=0)
		{
			time_ms_count++;
			if(time_ms_count>=1000)
			{
				time_ms_count = 0;
				time_s_count++;
				if(time_s_count>=TIM_MAX)
				{
					time_s_count = 0;
					time_flag  = 1;
				}
			}
		}
		else
		{
				time_ms_count = 0;
				time_s_count = 0;
				time_flag = 1;
				remain_time = 0;
		}
		
	}
	if(pwm_count<PWM_MAX)//����MAX�ı�ռ�ձ�
	{
			pwm_fan_flag=1;
	}
	else
	{
			pwm_fan_flag=0;
	}
}
void DelayK(unsigned char t)
{
	while(t--)
	{
		Display();
	}
}
void Delay_SEG(unsigned int times)		//@12.000MHz
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
void Display_SEG_Bit(unsigned char pos,unsigned char value)
{
	HC138_Select(3);//λѡ
	P0 = 0x00;//�ر����������
	P0 = (0x01<<(pos-1));
	HC138_Select(2);//��ѡ
	P0 = 0xFF;//�ر����ж�ѡ
	P0 = SMG_duanma[value];
	Delay_SEG(1);
	P0 = 0xFF;//��Ӱ
}