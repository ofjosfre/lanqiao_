/*******************************************************************************  
* 文件名称：通过计数器0和定时器1采集NF555生成的脉冲频率实验
* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J3配置为SIGNAL-P34（可拔下J2的任一跳线帽即可）
* 实验现象：通过计数器0和定时器1采集NF555生成的脉冲频率
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include "stc15.h"
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
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
unsigned char count_t = 0;
unsigned int count_f = 0;
unsigned int dat_f = 0;
unsigned int fre_can    = 60000;
unsigned int dianya_can = 600;
unsigned int dianya_ain_dianya;
unsigned int dianya_ain_light;
unsigned char display_flag;
unsigned char channel_flag;
unsigned char press_flag;
unsigned char press_count;
unsigned char led_function;
void display_fre();
void display_Time();
void display_adc();
void display();
unsigned char Read_AIN3();
unsigned char Read_AIN1();
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);
void Delay_K(unsigned char times);
void LED_Control();
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
			dianya_ain_light  = (unsigned int)((Read_AIN1()*5.0/255)*100);
			dianya_ain_dianya = (unsigned int)((Read_AIN3()*5.0/255)*100);
			Key_Scan();
			display();
			LED_Control();
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
void initTimer()
{
	TMOD = 0X04;//定时器0工作于计数模式且为16位自动重装载模式 定时器1工作于定时模式且为16位自动重装载模式
	TL0 = 0XFF;//差一个脉冲就会中断
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
	if(count_t==20)//每隔1s就计算数了多少个脉冲，即为频率
	{
		dat_f = count_f;
		count_t = 0;
		count_f = 0;
	}
	if(press_flag==1)
	{
		press_count++;
	}
}
void display_adc()
{
	Display_SEG_Bit(1,19);//U
	Display_SEG_Bit(2,16);//-
	if(channel_flag==0)
	{
		Display_SEG_Bit(3,1);//-
		HC138_Select(3);//位选
		P0 = 0x00;
		P0 = 0x01<<(6-1);
		HC138_Select(2);//段选
		P0 = 0xff;
		P0 = SMG_duanma[dianya_ain_light/100%10]&(~0x80);
		DelaySMG(1);
		P0 = 0xff;
		
		Display_SEG_Bit(7,dianya_ain_light/10%10);
		Display_SEG_Bit(8,dianya_ain_light%10);
	}
	else
	{
		Display_SEG_Bit(3,3);
		HC138_Select(3);//位选
		P0 = 0x00;
		P0 = 0x01<<(6-1);
		HC138_Select(2);//段选
		P0 = 0xff;
		P0 = SMG_duanma[dianya_ain_dianya/100%10]&(~0x80);
		DelaySMG(1);
		P0 = 0xff;
		
		Display_SEG_Bit(7,dianya_ain_dianya/10%10);
		Display_SEG_Bit(8,dianya_ain_dianya%10);
	}	
}
void display_fre()
{
		Display_SEG_Bit(1,15);//F
		if(dat_f>9999)//20000
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
		if(dat_f>=0)
		{
			Display_SEG_Bit(8,dat_f%10);
		}

}
void display_Time()
{
	unsigned int Tf = (unsigned int)((1.0/dat_f)*1000000);
	Display_SEG_Bit(1,20);//n
	
	if(Tf>9999)//20000
		{
			Display_SEG_Bit(4,Tf/10000%10);
		}
		if(Tf>999)
		{
			Display_SEG_Bit(5,Tf/1000%10);
		}
		if(Tf>99)
		{
			Display_SEG_Bit(6,Tf/100%10);
		}
		if(Tf>9)
		{
			Display_SEG_Bit(7,Tf/10%10);
		}
		if(Tf>=0)
		{
			Display_SEG_Bit(8,Tf%10);
		}
}
unsigned char Read_AIN3()
{
	unsigned char temp = 0;
	unsigned char i;
	for(i=0;i<2;i++)
	{
		IIC_Start();//IIC总线起始信号
		IIC_SendByte(0x90);//发送PCF8591的写操作地址
		IIC_WaitAck();//等待从机应答
		IIC_SendByte(0x03);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道3)。
		//IIC_SendByte(0x01);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道1)。
		IIC_WaitAck();//等待从机应答
		IIC_Stop();//IIC总线起始信号
		
		IIC_Start();
		IIC_SendByte(0x91);//发送PCF8591的读操作地址
		IIC_WaitAck();
		temp = IIC_RecByte();//读取PCF8591通道的数据 
		IIC_SendAck(1);//产生非应答信号
		IIC_Stop();
	}
	return temp;
}

unsigned char Read_AIN1()
{
	unsigned char temp = 0;
	unsigned char i;
	for(i=0;i<2;i++)
	{
		IIC_Start();//IIC总线起始信号
		IIC_SendByte(0x90);//发送PCF8591的写操作地址
		IIC_WaitAck();//等待从机应答
		//IIC_SendByte(0x03);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道3)。
		IIC_SendByte(0x01);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道1)。
		IIC_WaitAck();//等待从机应答
		IIC_Stop();//IIC总线起始信号
		
		IIC_Start();
		IIC_SendByte(0x91);//发送PCF8591的读操作地址
		IIC_WaitAck();
		temp = IIC_RecByte();//读取PCF8591通道的数据 
		IIC_SendAck(1);//产生非应答信号
		IIC_Stop();
	}
	return temp;
}
void Delay_K(unsigned char times)
{
	while(times--)
	{
		display();
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
				display();
			}
			display_flag++;
			if(display_flag==3)
			{
				display_flag = 0;
			}
			if(display_flag==2)
			{
				channel_flag = 0;
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
				display();
			}
			if(display_flag==2)
			{
				channel_flag = !channel_flag;
			}
		}
	}
	
		if(S7==0)
	{
		Delay_K(20);
		if(S7==0)
		{
			press_flag = 1;
			while(S7==0)
			{
				display();
			}
			press_flag = 0;
			if(press_count>20)
			{
				led_function = !led_function;
			}
			else
			{
				fre_can = dat_f;
			}
			press_count = 0;
		}
	}
	
	
	if(S6==0)
	{
		Delay_K(20);
		if(S6==0)
		{
			while(S6==0)
			{
				display();
			}
			dianya_can = dianya_ain_dianya;
		}
	}
}
void display()
{
	if(display_flag==0)
	{
		display_fre();
	}
	else if(display_flag==1)
	{
		display_Time();
	}
	else if(display_flag==2)
	{
		display_adc();
	}
}
void LED_Control()
{
	if(led_function==0)
	{
			if(dianya_ain_dianya > dianya_can)
			{
				HC138_Select(1);
				LED1 = 0;
			}
			else
			{
				HC138_Select(1);
				LED1 = 1;
			}
			if(dat_f > fre_can)
			{
				HC138_Select(1);
				LED2 = 0;
			}
			else
			{
				HC138_Select(1);
				LED2 = 1;
			}
			if(display_flag==0)
			{
				HC138_Select(1);
				LED3 = 0;
				LED4 = 1;
				LED5 = 1;
			}
			else if(display_flag==1)
			{
				HC138_Select(1);
				LED3 = 1;
				LED4 = 0;
				LED5 = 1;
			}
			else if(display_flag==2)
			{
				HC138_Select(1);
				LED3 = 1;
				LED4 = 1;
				LED5 = 0;
			}
			else
			{
				
			}
	}
	else
	{
		HC138_Select(1);
		P0 = 0xFF;//所有灯关掉
	}
}