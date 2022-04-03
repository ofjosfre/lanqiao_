/*******************************************************************************  
* 文件名称：矩阵键盘实验
* 实验配置：J13跳线配置为IO方式，J5配置为KBD、J2配置为1-3和2-4
* 实验现象：当按下并松开按键时第1、2个数码管显示相应的键值，键值范围为0-15
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
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
	P0=0X00;//初始化程序--关蜂鸣器、关继电器
	HC138_Select(1);
	P0=0XFF;//初始化程序--关灯
	while(1)
	{
		temperature = (unsigned int)(read_temperature()*100); //采集温度、并将其放大100倍
		//display_temperature();
		Function_Key();//矩阵按键功能判断函数
		Display();//界面显示函数
		Panduan_Mode();//根据不同的模式 输出电压不同
		
	}
}
//切换显示界面
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
		Key_Scan();//行列扫描
		if(keyVal == 13)
		{
			keyVal = 0;
			display_flag++;//S4按键切换显示的界面
			if(display_flag==3)
			{
				display_flag = 0;
			}
		}
		if(keyVal == 9)//切换模式
		{
			keyVal = 0;
			mode_flag = !mode_flag;
		}
		if(display_flag==1)//保证加、减按键仅在参数设置界面有效
		{
			if(keyVal == 10)//+++
			{
				keyVal = 0;
				if(temp_can<=98)//合理设置边界 数据最大增加到99
					temp_can++;
			}
			if(keyVal == 14)//----
			{
				keyVal = 0;
				if(temp_can>=1)//合理设置边界 数据最小减小到0
					temp_can--;
			}
		}
}
void Panduan_Mode()
{
	if(mode_flag==0)//模式1
	{
		HC138_Select(1);
		LED1 = 0;
		if(temperature!=8500)//此值DS18B20刚转换时未转换完成可能出现的值
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
	HC138_Select(3);//位选
	P0 = 0x00;//关闭所有数码管
	P0 = (0x01<<(pos-1));
	HC138_Select(2);//段选
	P0 = 0xFF;//关闭所有段选
	P0 = SMG_duanma[value];
	delay_seg(1);
	P0 = 0xFF;//消影
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
void Key_Scan()
{
	unsigned char i;
	C1 = C2 = C3 = C4 = 1;//每次判断前把全部列线拉高
	for(i=0;i<4;i++)
	{
		P3 |= 0x0f;
		P3 &= ~(1<<i);//依次把每行拉低：每次只有一行拉低，其余为高
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
			/*显示小数点*/
			HC138_Select(3);//位选
			P0 = 0x00;
			P0 = 0x01<<(6-1);
			HC138_Select(2);//段选
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
	
	/*显示小数点*/
	HC138_Select(3);//位选
	P0 = 0x00;
	P0 = 0x01<<(6-1);
	HC138_Select(2);//段选
	P0 = 0xff;
	P0 = SMG_duanma[temp_dianya/100%10]&(~0x80);
	delay_seg(1);
	P0 = 0xff;
	Display_SEG_Bit(7,temp_dianya/10%10);
	Display_SEG_Bit(8,temp_dianya%10);
	
}
void Pout_DAC(unsigned char temp)
{
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0x90);//发送PCF8591的写设备地址
	IIC_WaitAck();//等待从机应答
	IIC_SendByte(0x40);//发送控制字节，选择DA模式
	IIC_WaitAck();//等待从机应答
	IIC_SendByte(temp);//发送
	IIC_WaitAck();//等待从机应答
	IIC_Stop();//IIC总线起始信号
}