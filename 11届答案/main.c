/*******************************************************************************  
* 文件名称：矩阵键盘实验
* 实验配置：J13跳线配置为IO方式，J5配置为KBD、J2配置为1-3和2-4
* 实验现象：当按下并松开按键时第1、2个数码管显示相应的键值，键值范围为0-15
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include <stc15.h>
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
sbit LED6 = P0^5;
sbit LED7 = P0^6;
sbit LED8 = P0^7;
sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
sbit C4 = P3^4;

unsigned char LED1_FLAG,LED2_FLAG;
unsigned char display_flag;
unsigned char keyVal=0;
float Vp;
unsigned char count_cmp;
unsigned int LED1_count ;
unsigned int LED3_count ;
float value_read_AIN3;
//float compare_AIN3;
void Display();
void KBD_Scan();
void LED_Control();
void display_can();
void HC138_Select(unsigned char i);
void display_ain3();
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelayK(unsigned char t);
void delay_seg(unsigned int times);
void display_count();
void Key_Scan();
void Function_Key();
void Dianya_Comp();
void Get_Time5s();
unsigned char Read_AIN3();
unsigned char Read_AT24C02(unsigned char addr);
void Write_AT24C02(unsigned char addr,unsigned char dat);
void initTimer0();
void LED_Control();
void main()
{
	HC138_Select(4);
	P0=0X00;//关蜂鸣器、继电器
	HC138_Select(1);
	P0=0XFF;//关灯
	initTimer0();
	while(1)
	{
		Vp = Read_AT24C02(0x00);//读取AT24C02中存储的电压参数
		if(Vp>50)
		{
			Vp = 50;
		}
		Vp = Vp/10.0;//缩小10倍才是真正的参数
		value_read_AIN3 = Read_AIN3()*5.0/255;//读取模拟电压
		Function_Key();//按键扫描、处理函数 完成显示界面切换、电压参数调整、计数值清0等
		Get_Time5s();//根据读取到的电压值完成5S的计时
		LED_Control();//控制L1、L2、L3灯
		Dianya_Comp();//判断读取的电压值是否处于下降且等于电压参数
		Display();//界面显示
	}
}
void LED_Control()
{
	if(LED1_FLAG==1)
	{
		HC138_Select(1);//位选
		LED1 = 0;
	}
	else
	{
		HC138_Select(1);//位选
		LED1 = 1;
	}
	if(LED2_FLAG==1)
	{
		HC138_Select(1);//位选
		LED2 = 0;
	}
	else
	{
		HC138_Select(1);//位选
		LED2 = 1;
	}
	if(LED3_count >= 3)
	{
		HC138_Select(1);//位选
		LED3 = 0;
	}
	else if(LED3_count == 0)
	{
		HC138_Select(1);//位选
		LED3 = 1;
	}
	else
	{
		
	}
}
void Display()
{
		if(display_flag==0)
		{
			display_ain3();
		}
		else if(display_flag==1)
		{
			display_can();
		}
		else if(display_flag==2)
		{
			 display_count();
		}
		else
		{
			
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
			DelayK(20);
			if(C1==0)
			{
				while(C1==0)
				{
					Display();
				}
				keyVal = i*4+1;
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
					Display();
				}
				keyVal = i*4+2;
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
					Display();
				}
				keyVal = i*4+3;
				break;
			}
		}
		else if(C4==0)
		{
			DelayK(20);
			if(C4==0)
			{
				while(C4==0)
				{
					Display();
				}
				keyVal = i*4+4;
				break;
			}
		}
	}
	
	//return keyVal;

}
void Function_Key()
{

		Key_Scan();
		if(keyVal == 15)
		{
			keyVal = 0;
			LED3_count = 0;
			display_flag++;
			if(display_flag==3)
			{
				display_flag = 0;
			}
		}
		else if(keyVal==16)//参数+
		{
			keyVal = 0;
			if(display_flag==1)//有效按键
			{
				//Vp = Vp + 0.5;
				LED3_count = 0;
				if(Vp<5)
				{
					Vp = Vp + 0.5;
				}
				else
				{
					Vp = 0;
				}
				Write_AT24C02(0x00,(unsigned char)(Vp*10));
				
			}
			else//无效按键
			{
				LED3_count++;
			}
		}
		else if(keyVal==12)//参数-
		{
			keyVal = 0;
			if(display_flag==1)//有效按键
			{
				//Vp = Vp - 0.5;
				LED3_count = 0;
				if(Vp>0)
				{
					Vp = Vp - 0.5;
				}
				else
				{
					Vp = 5;
				}
				Write_AT24C02(0x00,(unsigned char)(Vp*10));
			}
			else//无效按键
			{
				LED3_count++;
			}
		}
		else if(keyVal==11)//清0
		{
			keyVal = 0;
			if(display_flag==2)//有效按键
			{
				LED3_count = 0;
				count_cmp = 0;
			}
			else//无效按键
			{
				LED3_count++;
			}
		}
		else if(keyVal!=0)//无效按键
		{				
			keyVal = 0;
			LED3_count++;
		}
}
void Dianya_Comp()
{
	static int bd_flag;//等于5V
	static int bx_flag;//等于5V
	if(Vp==5)
	{
		if((unsigned int)(value_read_AIN3*100)<500)
		{
			bd_flag = 1;
		}
		if((bd_flag==1)&&((unsigned int)(value_read_AIN3*100)==(unsigned int)(Vp*100)))
		{
			bd_flag = 0;
			count_cmp++;
			if(count_cmp==100)
			{
				count_cmp = 99;
			}
		}
	}
	else
		{
			if((unsigned int)(value_read_AIN3*100)>(unsigned int)(Vp*100))
			{
				bx_flag = 1;
			}
			if((bx_flag == 1)&&((unsigned int)(value_read_AIN3*100)<=(unsigned int)(Vp*100)))
			{
			//计数值加1
				bx_flag = 0;
				count_cmp++;
				if(count_cmp==100)
				{
					count_cmp = 99;
				}
			}
		}
	if(count_cmp%2==1)
	{
		LED2_FLAG = 1;
	}
	else
	{
		LED2_FLAG = 0;
	}
}
void  Get_Time5s()
{
	if(value_read_AIN3<Vp)
	{
		TR0 = 1;
	}
	else
	{
		TR0 = 0;
		LED1_count = 0;
		LED1_FLAG = 0;
	}
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
void DelayK(unsigned char t)
{
	while(t--)
	{
		Display();
	}
}
void display_ain3()
{
	unsigned int value_int_AIN3;
	value_int_AIN3 = (unsigned int)(value_read_AIN3*100);//3.67 367
	Display_SEG_Bit(1,19);
//	Display_SEG_Bit(6,19);
	HC138_Select(3);//位选
	P0 = 0x00;//关闭所有数码管
	P0 = (0x01<<(6-1));
	HC138_Select(2);//段选
	P0 = 0xFF;//关闭所有段选
	P0 = SMG_duanma[value_int_AIN3/100%10]&(~0x80);
	delay_seg(1);
	P0 = 0xFF;
	
	Display_SEG_Bit(7,value_int_AIN3/10%10);
	Display_SEG_Bit(8,value_int_AIN3%10);
}
void display_can()
{
	unsigned int Vp_can;
	Vp_can = (unsigned int)(Vp*100);
	Display_SEG_Bit(1,18);
//	Display_SEG_Bit(6,19);
	HC138_Select(3);//位选
	P0 = 0x00;//关闭所有数码管
	P0 = (0x01<<(6-1));
	HC138_Select(2);//段选
	P0 = 0xFF;//关闭所有段选
	P0 = SMG_duanma[Vp_can/100%10]&(~0x80);
	delay_seg(1);
	
	Display_SEG_Bit(7,Vp_can/10%10);
	Display_SEG_Bit(8,Vp_can%10);
}
void display_count()
{
	Display_SEG_Bit(1,20);
	Display_SEG_Bit(7,count_cmp/10%10);
	Display_SEG_Bit(8,count_cmp%10);
}
void Write_AT24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xA0); 
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop(); 
}
unsigned char Read_AT24C02(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xA0); 
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1); 
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop(); 
	return temp;
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
void initTimer0()
{
	TMOD = 0X00;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	EA = 1;
	ET0 =1;
//	TR0 = 1;
}
void Timer0()interrupt 1
{
	LED1_count++;
	if(LED1_count>100)
	{
		LED1_count = 0;
		LED1_FLAG = 1;
	}
}