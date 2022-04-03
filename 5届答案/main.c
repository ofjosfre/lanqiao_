/*******************************************************************************  
* 文件名称：矩阵键盘实验（行列扫描法）
* 实验配置：J13跳线配置为IO方式，J5配置为KBD、J2配置为1-3和2-4
* 实验现象：当按下并松开按键时第1、2个数码管显示相应的键值，键值范围为0-15
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include <stc15.h>
#include "onewire.h"
unsigned char code SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
};
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
unsigned char keyVal;
unsigned char shezhi_count;
unsigned char shezhi_flag;
unsigned char qujian_mode;
unsigned char TIME_MAX;
unsigned char led_count;
unsigned char led_flag;
unsigned int temperature;
unsigned int Tmax=30,Tmin=20;
unsigned int TmaxLast=30,TminLast=20;
void Display_KBD();
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelayK(unsigned char t);
void delay_seg(unsigned int times);
void Key_Scan();
void LED_Control();
void Timer0_Init();
void Function_Key();
void main()
{
	HC138_Select(4);
	P0=0X00;
	HC138_Select(1);
	P0=0XFF;
	Timer0_Init();
	while(1)
	{
		temperature = (unsigned int)(read_temperature()); 
		Function_Key();
		Display_KBD();
		LED_Control();
	}
}
void LED_Control()
{
	if(TR0==1)
	{
	if(led_flag==0)
	{
		HC138_Select(1);
		P0 = 0xFF;
		P00 = 0;
	}
	else
	{
		HC138_Select(1);
		P0 = 0xFF;
		P00 = 1;
	}}
//	else
//	{
//		HC138_Select(1);
//		P0 = 0xFF;
//	}
}
void Display_KBD()
{
	if(temperature!=85)	
	{
	if(shezhi_count==0)
	{
	if(Tmin<=Tmax)
	{
	
	if(temperature<Tmin)
	{
		qujian_mode = 0;
		TIME_MAX    = 16;
//		HC138_Select(4);
//		P0 = 0x00;
	}
	else if((temperature>=Tmin)&&(temperature<=Tmax))
	{
		qujian_mode = 1;
		TIME_MAX    = 8;
//		HC138_Select(4);
//		P0 = 0x00;
	}
	else if(temperature>Tmax)
	{
		qujian_mode = 2;
		TIME_MAX    = 4;
		if(TR0==1)
		{
				HC138_Select(4);
				P04 = 1;
				P06 = 0;
				HC138_Select(1);
				P06 = 1;
		}
	}
	else
	{
		
	}
	}

	
	}
	if(shezhi_flag==0)
	{
		Display_SEG_Bit(1,16);
		if(qujian_mode!=3)
		{
			Display_SEG_Bit(2,qujian_mode);
		}
		else
		{
			Display_SEG_Bit(2,15);
		}
		Display_SEG_Bit(3,16);
		Display_SEG_Bit(7,temperature/10%10);
		Display_SEG_Bit(8,temperature%10);
	}
	else
	{
		Display_SEG_Bit(1,16);
		Display_SEG_Bit(6,16);
		Display_SEG_Bit(2,Tmax/10%10);
		Display_SEG_Bit(3,Tmax%10);
		Display_SEG_Bit(7,Tmin/10%10);
		Display_SEG_Bit(8,Tmin%10);
	}
	
}
}
void Timer0_Init()
{
	TMOD = 0X00;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	EA  = 1;
	ET0 = 1;
	TR0 = 1;
}
void Timer0Service()interrupt 1
{
	led_count++;
	if(led_count>=TIME_MAX)
	{
		led_count = 0;
		led_flag = !led_flag;
	}
}
void Function_Key()
{
	unsigned char temp;
	Key_Scan();
	if(keyVal==11)
	{
		keyVal = 0;
		shezhi_flag = !shezhi_flag;
		shezhi_count = 0;
		if(shezhi_flag==1)
		{
			TmaxLast = Tmax;
			TminLast = Tmin;
			TR0 = 0;
			led_count = 0;
//			HC138_Select(1);
//			P0 = 0xFF;
			HC138_Select(4);
			P04 = 0;
			P06 = 0;
//			HC138_Select(1);
//			P06 = 1;
			HC138_Select(1);
			P0 = 0xFF;
		}
		else
		{
			TR0 = 1;
		}
	}
	else if(keyVal==12)
	{
		keyVal = 0;
		if(shezhi_flag==1)
		{
			Tmax = TmaxLast;
			Tmin = TminLast;
			shezhi_count = 0;
		}
	}
	else if(keyVal!=0)
	{
		temp = keyVal;
		keyVal = 0;
		if(shezhi_flag==1)
		{
			shezhi_count++;
			if(shezhi_count>=5)
			{
				shezhi_count = 66;//继续按键不再视为有效输入
			}
			if(shezhi_count==1)
			{
				Tmax = (temp-1)*10+Tmax%10;
			}
			else if(shezhi_count==2)
			{
				Tmax = Tmax/10*10+(temp-1);
			}
			else if(shezhi_count==3)
			{
				Tmin = (temp-1)*10+Tmin%10;
			}
			else if(shezhi_count==4)
			{
				Tmin = Tmin/10*10+(temp-1);
			}
			else
			{
				
			}
		}
	}
	if(shezhi_count==0)
	{
	if(Tmin>Tmax)
	{
		TR0 = 0;
		led_count = 0;
		HC138_Select(1);
		P0 = 0xFF;
		P01 = 0;
		qujian_mode = 3;
	}
	else
	{
		HC138_Select(1);
		P0 = 0xFF;
		P01 = 1;
	}
	}
}
void Key_Scan()
{
	unsigned char i;
	C1 = C2 = C3 = 1;//每次判断前把全部列线拉高
	for(i=0;i<4;i++)
	{
		P3 |= 0x0f;
		P3 &= ~(1<<i);//依次把每行拉低：每次只有一行拉低，其余为高
		if(C1==0)
		{
			DelayK(20);
			if(C1==0)
			{
				while(C1==0)
				{
					Display_KBD();
				}
				keyVal = i*3+1;
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
					Display_KBD();
				}
				keyVal = i*3+2;
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
					Display_KBD();
				}
				keyVal = i*3+3;
				break;
			}
		}
	}
	
	//return keyVal;

}


void Display_SEG_Bit(unsigned char pos,unsigned char value)
{
	HC138_Select(3);//位选
	P0 = 0x00;//关闭所有数码管
	P0 = (0x01<<(pos-1));
	HC138_Select(2);//段选
	P0 = 0xFF;//关闭所有段选
	P0 = SMG_duanma[value];
	delay_seg(1);
	// P0 = 0x00;//消影
	P0 = 0xFF;
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
		case 4://达林顿管阵列 取反门
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
		Display_KBD();
	}
}