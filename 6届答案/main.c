/*******************************************************************************  
* 文件名称：通过DS18B20采集环境温度实验
* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 实验现象：数码管显示采集的温度数值
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include "stc15.h"
#include "onewire.h"
#include "ds1302.h"
unsigned char code SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
};
unsigned char code inittime[7] = {50,59,23,5,1,3,22};//秒-分-时-日-月-星期-年份
unsigned char gettime[7];
unsigned char jiange=1;
unsigned char jinage_shuzu[] = {1,5,30,60};
unsigned char wendu_shuzu[10];
unsigned char display_flag;
unsigned char caiji_complete;
unsigned char wenduxian_ii;
unsigned char tishifu_count,tishifu_flag;
unsigned char led_count,led_flag;
unsigned int temperature;
//unsigned int count;
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit L1 = P0^0;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
unsigned int count;
void ds1302_init();
void readtime();
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);	
void Delay_K(unsigned char times);
void Key_Scan();
void TimerInit();
void Display_JianGe();
void Display_Time();
void Display_Temperature();
void Display();
void LED_Control();
void main()
{
		HC138_Select(4);
		P0=0X00;
		HC138_Select(1);
		P0=0XFF;
		TimerInit();
		ds1302_init();
		while(1)
		{
			temperature = (unsigned int)(read_temperature()); 
			readtime();
			Key_Scan();
			Display();
			LED_Control();
		}
}
void TimerInit()
{
	TMOD = 0X00;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	TH1 = (65536-50000)/256;
	TL1 = (65536-50000)%256;
	EA  = 1;
	ET0 = 1;
	ET1 = 1;
	//TR0 = 1;
	TR1 = 1;
}
void Timer0Service() interrupt 1
{
	static int mm;
	count++;
	if(count>=(jiange*20))
	{
		count = 0;
		wendu_shuzu[mm] = temperature;
		mm++;
		if(mm==10)
		{
			TR0 = 0;
			mm = 0;
			caiji_complete = 1;
			//display_flag = 2;//温度显示界面
		}
	}
}
void Timer1Service() interrupt 3
{
	tishifu_count++;
	led_count++;
	if(led_count>=4)
	{
		led_count = 0;
		led_flag = !led_flag;
	}
	if(tishifu_count>=20)
	{
		tishifu_count = 0;
		tishifu_flag = !tishifu_flag;
		if(display_flag==2)
		{
		if(wenduxian_ii<=8)
		{	
			wenduxian_ii++;
		}
		}
	}
}
void LED_Control()
{
	if((caiji_complete==1)&&(display_flag==1))
	{
		if(led_flag==0)
		{
			P0 = 0XFF;
			HC138_Select(1);
			P0 = 0XFF;
			L1 = 0;
		}
		else
		{
			P0 = 0XFF;
			HC138_Select(1);
			P0 = 0XFF;
			L1 = 1;
		}
	}
	else
	{
		P0 = 0XFF;
		HC138_Select(1);
		P0 = 0XFF;
		L1 = 1;
	}
}
void Delay_K(unsigned char times)
{
	while(times--)
	{
		//display_AIN3_FRE();
		Display();
	}
}
void Key_Scan()
{
	static int ii;
	if(S4==0)
	{
		Delay_K(20);
		if(S4==0)
		{
			while(S4==0)
			{
				Display();
			}
			if(display_flag==0)
			{
				jiange = jinage_shuzu[ii];
				ii++;
				if(ii==4)
				{
					ii = 0;
				}
			}
		}
	}
	
	if(S5==0)
	{
		Delay_K(20);
		if(S5==0)
		{
			while(S5==0)
			{
				Display();
			}
			if(display_flag==0)
			{
				TR0 = 1;
				display_flag = 1;//时钟显示界面
				caiji_complete = 0;
				wenduxian_ii = 0;
			}
		}
	}
	
		if(S7==0)
	{
		Delay_K(20);
		if(S7==0)
		{
			while(S7==0)
			{
				Display();
			}
			if(display_flag==2)
			{
				display_flag = 0;
				wenduxian_ii = 0;
			}
		}
	}
	
		if(S6==0)
	{
		Delay_K(20);
		if(S6==0)
		{
			while(S6==0)
			{
				Display();
			}
			if(caiji_complete==1)
			{
				display_flag = 2;
				tishifu_count = 0;
			}
		}
	}
}
void Display_SEG_Bit(unsigned char pos,unsigned char value)
{
	HC138_Select(3);//位选
	P0 = 0x00;
	P0 = 0x01<<(pos-1);
	HC138_Select(2);//段选
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
		case 4://达林顿管阵列 取反门
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
void Display_JianGe()
{
	Display_SEG_Bit(6,16);
	Display_SEG_Bit(7,jiange/10%10);
	Display_SEG_Bit(8,jiange%10);
}
void Display_Time()
{
	Display_SEG_Bit(1,gettime[2]/10%10);
	Display_SEG_Bit(2,gettime[2]%10);
	//Display_SEG_Bit(3,16);
	Display_SEG_Bit(4,gettime[1]/10%10);
	
	Display_SEG_Bit(5,gettime[1]%10);
	//Display_SEG_Bit(6,16);
	Display_SEG_Bit(7,gettime[0]/10%10);
	Display_SEG_Bit(8,gettime[0]%10);
	if(tishifu_flag==0)
	{
		Display_SEG_Bit(3,16);
		Display_SEG_Bit(6,16);
	}
	else
	{
		HC138_Select(3);//位选
		P0 = 0x00;
	}
}
void Display_Temperature()
{
	
	Display_SEG_Bit(1,16);
	Display_SEG_Bit(2,wenduxian_ii/10);
	Display_SEG_Bit(3,wenduxian_ii%10);
	Display_SEG_Bit(6,16);
	Display_SEG_Bit(7,wendu_shuzu[wenduxian_ii]/10%10);
	Display_SEG_Bit(8,wendu_shuzu[wenduxian_ii]%10);
}
void Display()
{
	if(display_flag==0)
	{
		 Display_JianGe();
	}
	else if(display_flag==1)
	{
		 Display_Time();
	}
	else if(display_flag==2)
	{
		 Display_Temperature();
	}
	else
	{
		
	}
}