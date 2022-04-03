/*******************************************************************************  
* 文件名称：通过时钟芯片DS1302显示及调整时间实验
* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 实验现象：按下S7按键切换调整时间界面、S6加时/分/秒、S5减时/分/秒
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include "stc15.h"
#include "ds1302.h"
#include "onewire.h"
unsigned char  inittime[] = {58,59,23,5,1,3,22};//秒-分-时-日-月-星期-年份
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
void displaytime()
{
	if(time_alarm_flag==0)
	{
	if(displaytime_flag==0)
	{
		Display_SEG_Bit(1,gettime[2]/10);//时的十位
		Display_SEG_Bit(2,gettime[2]%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//分的十位
		Display_SEG_Bit(5,gettime[1]%10);//分的个位
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//秒的十位
		Display_SEG_Bit(8,gettime[0]%10);//秒的个位
	}
	else if(displaytime_flag==1)
	{
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//分的十位
		Display_SEG_Bit(5,gettime[1]%10);//分的个位
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//秒的十位
		Display_SEG_Bit(8,gettime[0]%10);//秒的个位
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(1,gettime[2]/10);//时的十位
		Display_SEG_Bit(2,gettime[2]%10);//时的个位
		
		}
		else
		{
				HC138_Select(3);//位选
				P0 = 0x00;
		}
	}
	else if(displaytime_flag==2)
	{
	
		Display_SEG_Bit(1,gettime[2]/10);//时的十位
		Display_SEG_Bit(2,gettime[2]%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,gettime[0]/10);//秒的十位
		Display_SEG_Bit(8,gettime[0]%10);//秒的个位
		if(shanshuo_flag==0)
		{

		Display_SEG_Bit(4,gettime[1]/10);//分的十位
		Display_SEG_Bit(5,gettime[1]%10);//分的个位

		}
		else
		{
				HC138_Select(3);//位选
				P0 = 0x00;

		}
	}
	else
	{
		Display_SEG_Bit(1,gettime[2]/10);//时的十位
		Display_SEG_Bit(2,gettime[2]%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,gettime[1]/10);//分的十位
		Display_SEG_Bit(5,gettime[1]%10);//分的个位
		Display_SEG_Bit(6,16);//-
		if(shanshuo_flag==0)
		{

		Display_SEG_Bit(7,gettime[0]/10);//秒的十位
		Display_SEG_Bit(8,gettime[0]%10);//秒的个位
		}
		else
		{
				HC138_Select(3);//位选
				P0 = 0x00;
		}
	}
}
	else
	{
		if(displayalarm_flag==0)
	{
		//闹钟-时
	
		Display_SEG_Bit(1,alarm_hour/10);//时的十位
		Display_SEG_Bit(2,alarm_hour%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//分的十位
		Display_SEG_Bit(5,alarm_min%10);//分的个位
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//秒的十位
		Display_SEG_Bit(8,alarm_sec%10);//秒的个位
	}
	else if(displayalarm_flag==1)
	{
		//闹钟-时
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//分的十位
		Display_SEG_Bit(5,alarm_min%10);//分的个位
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//秒的十位
		Display_SEG_Bit(8,alarm_sec%10);//秒的个位
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(1,alarm_hour/10);//时的十位
		Display_SEG_Bit(2,alarm_hour%10);//时的个位
		}
		else
		{
				HC138_Select(3);//位选
				P0 = 0x00;
		}
	}
	else if(displayalarm_flag==2)
	{
		//闹钟-分
		Display_SEG_Bit(1,alarm_hour/10);//时的十位
		Display_SEG_Bit(2,alarm_hour%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(6,16);//-
		Display_SEG_Bit(7,alarm_sec/10);//秒的十位
		Display_SEG_Bit(8,alarm_sec%10);//秒的个位
		if(shanshuo_flag==0)
		{
		Display_SEG_Bit(4,alarm_min/10);//分的十位
		Display_SEG_Bit(5,alarm_min%10);//分的个位
		}
		else
		{
				HC138_Select(3);//位选
				P0 = 0x00;
		}
	}
	else 
	{
		//闹钟-秒
		Display_SEG_Bit(1,alarm_hour/10);//时的十位
		Display_SEG_Bit(2,alarm_hour%10);//时的个位
		Display_SEG_Bit(3,16);//-
		Display_SEG_Bit(4,alarm_min/10);//分的十位
		Display_SEG_Bit(5,alarm_min%10);//分的个位
		Display_SEG_Bit(6,16);//-
		if(shanshuo_flag==0)
		{

			Display_SEG_Bit(7,alarm_sec/10);//秒的十位
			Display_SEG_Bit(8,alarm_sec%10);//秒的个位
		}
		else
		{
				HC138_Select(3);//位选
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
			if(alarm_ring_flag==0)//到达闹钟时间设定时间标志，0：闹钟正在响，1：闹钟暂未到时间或者将闹钟已关闭
			{
					ET1 = 0;
					TR1 = 0;
					ledtime_count = 0;
					led_flag = 0;
					alarm_ring_flag=1;
			}
			else
			{
					if(time_alarm_flag==1)//时间/闹钟显示标志，0：时间显示   1：闹钟显示
					{
						//当按下S7时,若此时在闹钟显示，则切换为时间显示界面
							time_alarm_flag = 0;
							starttime();
							displaytime_flag = 0;
							displayalarm_flag = 0;
					}
					else
					{
						//否则，则此按键为时间调整按键
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
			if(alarm_ring_flag==0)//到达闹钟时间设定时间标志，0：闹钟正在响，1：闹钟暂未到时间或者将闹钟已关闭
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
					if(time_alarm_flag==0)//时间/闹钟显示标志，0：时间显示   1：闹钟显示
					{
						//当按下S6时,若此时在时间显示，则切换为闹钟显示界面
							time_alarm_flag = 1;
							starttime();
							displayalarm_flag = 0;
							displaytime_flag = 0;
					}
					else
					{
						//否则，则此按键为时间调整按键
							time_alarm_flag = 1;
							displayalarm_flag++;
					}
					if(displayalarm_flag==4)
					{
							starttime();
							displayalarm_flag = 0;
						  ledtime_count = 0;//每次设置完闹钟后都要重新计数5s的时间
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
			if(alarm_ring_flag==0)//到达闹钟时间设定时间标志，0：闹钟正在响，1：闹钟暂未到时间或者将闹钟已关闭
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
				//显示温度
						//alarm_ring_flag=1;
						if((displaytime_flag==0)&&(displayalarm_flag==0))
						{
							temperature = (unsigned int)(read_temperature()); 
							display_temperature();
							readtime();
							Alarm_LED();
						}
					}
			if(alarm_ring_flag==0)//到达闹钟时间设定时间标志，0：闹钟正在响，1：闹钟暂未到时间或者将闹钟已关闭
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
	write(0x80,gettime[0]|0x80);//暂停
	write(0x8E,0x80);

}
void starttime(void)
{
	write(0x8E,0X00);
	write(0x84,gettime[2]);//开始
	write(0x82,gettime[1]);//开始
	write(0x80,gettime[0]&0x7f);//开始
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
	if(led_count>=4)//闹钟提示灯以0.2s为间隔闪烁
	{
		led_count = 0;
		led_flag = !led_flag;
	}
		if(ledtime_count>=110)//闹钟持续时间约5S，标准100为5s，实测110左右时更好
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
	if(alarm_ring_flag==0)//到达闹钟时间设定时间标志，0：闹钟正在响，1：闹钟暂未到设置时间或者将闹钟已关闭或者超过鸣响超过5s
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
