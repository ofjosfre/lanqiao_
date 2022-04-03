/*******************************************************************************  
* 文件名称：通过DS18B20采集环境温度实验
* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 实验现象：数码管显示采集的温度数值
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include "stc15.h"
#include "onewire.h"
#include "iic.h"
unsigned char code SMG_duanma[21]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0x8C,0xc1,0xc8
};//- . P U n
unsigned int temperature;//0-65535
//unsigned int count;
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
sbit L4 = P0^3; 
unsigned char display_flag;
unsigned char Tmax_Tmin_flag;
unsigned char Tmax = 30,Tmin = 20;
unsigned char TmaxLast ,TminLast ;
void HC138_Select(unsigned char i);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);	
void Display_Temp_Can();
void DelayK(unsigned char t);
void Key_Scan();
void DAC_OUT();
void main()
{
		HC138_Select(4);
		P0=0X00;
		HC138_Select(1);
		P0=0XFF;
		TmaxLast = Tmax;
		TminLast = Tmin;
		while(1)
		{
			temperature = (unsigned int)(read_temperature()); 
			Key_Scan();
			Display_Temp_Can();
			DAC_OUT();
		}
}
void Display_Temp_Can()
{
	if(display_flag==0)
	{
		if(temperature!=85)
		{	
			Display_SEG_Bit(1,12);
			Display_SEG_Bit(7,temperature/10%10);
			Display_SEG_Bit(8,temperature%10);
		}
	}
	else
	{
		Display_SEG_Bit(1,18);
		Display_SEG_Bit(4,Tmax/10);
		Display_SEG_Bit(5,Tmax%10);
		Display_SEG_Bit(7,Tmin/10);
		Display_SEG_Bit(8,Tmin%10);
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
void Key_Scan()
{
	if(S4==0)
	{
		DelayK(30);
		if(S4==0)
		{
			while(S4==0)
			{
				Display_Temp_Can();
			}
			display_flag = !display_flag;
			if(display_flag==1)
			{
				TmaxLast = Tmax;
				TminLast = Tmin;
			}
			else
			{
				if(Tmin>Tmax)
				{
					Tmax = TmaxLast;
					Tmin = TminLast;
				}
			}
		}
	}
	
		if(S5==0)
	{
		DelayK(30);
		if(S5==0)
		{
			while(S5==0)
			{
				Display_Temp_Can();
			}
			if(display_flag==1)
			{
				Tmax_Tmin_flag = !Tmax_Tmin_flag;
			}
		}
	}
	
		if(S6==0)//+1
	{
		DelayK(30);
		if(S6==0)
		{
			while(S6==0)
			{
				Display_Temp_Can();
			}
			if(display_flag==1)
			{
			if(Tmax_Tmin_flag==0)
			{
				if(Tmin<=98)
				{
					Tmin++;
				}
			}
			else
			{
				if(Tmax<=98)
				{
					Tmax++;
				}
			}
		}
		}
	}
	
	
	
		if(S7==0)//-1
	{
		DelayK(30);
		if(S7==0)
		{
			while(S7==0)
			{
				Display_Temp_Can();
			}
			if(display_flag==1)
			{
			if(Tmax_Tmin_flag==0)
			{
				if(Tmin>=1)
				{
					Tmin--;
				}
			}
			else
			{
				if(Tmax>=1)
				{
					Tmax--;
				}
			}
		}}
	}
}
void DelayK(unsigned char t)
{
	while(t--)
	{
		Display_Temp_Can();
	}
}
void DAC_OUT()
{
	unsigned char temp;
	if(Tmin<=Tmax)
	{
		if(temperature>Tmax)
		{
			temp = 51*4.1;//51*4
			HC138_Select(1);
			L1 = 0;
		}
		else if((temperature>=Tmin)&&(temperature<=Tmax))
		{
			temp = 51*3.1;//51*3
			HC138_Select(1);
			L2 = 0;
		}
		else if(temperature<Tmin)
		{
			temp = 51*2.1;//51*2
			HC138_Select(1);
			L3 = 0;
		}
	}
	else
	{
		temp = 0;
		HC138_Select(1);
		L4 = 0;
	}
	
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0x90);//发送PCF8591的写设备地址
	IIC_WaitAck();//等待从机应答
	IIC_SendByte(0x40);//发送控制字节，选择DA模式
	IIC_WaitAck();//等待从机应答
	IIC_SendByte(temp);//发送0-255
	IIC_WaitAck();//等待从机应答
	IIC_Stop();//IIC总线起始信号
}
