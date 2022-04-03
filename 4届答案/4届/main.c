/*******************************************************************************  
* 文件名称：通过时钟芯片DS1302显示时间实验
* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 实验现象：按下S7按键切换时间与日期的显示、S6暂停时间计时
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
*******************************************************************************/
#include "stc15.h"
#include "ds1302.h"
#include "iic.h"
void ds1302_init();
void readtime();
void stoptime(void);
void starttime(void);
unsigned char  code inittime[] = {57,30,8,5,1,3,22};//秒-分-时-日-月-星期-年份
unsigned char gettime[7];
unsigned char  code SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
};
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit RELAY  = P0^4;
sbit BUZZER = P0^6;
unsigned char Humidity_threshold = 50;
unsigned char Humidity;
unsigned char mode_flag;//0：自动模式   1：手动模式
unsigned char alarm_flag;//0:开蜂鸣器提醒功能 1：关蜂鸣器提醒功能
unsigned char relay_flag;//0:关继电器 1：开继电器
unsigned char threshold_flag;//0:阈值调整完成 1：正在调整
void Key_Scan();
float Read_AIN3();
unsigned char Read_AT24C02(unsigned char addr);
void Write_AT24C02(unsigned char addr,unsigned char dat);
void HC138_Select(unsigned char i);
void DelayK(unsigned char t);
void Display_SEG_Bit(unsigned char pos,unsigned char value);
void DelaySMG(unsigned int times);
unsigned char Read_Humidity();
void Display_Humidity_Time();
void Display_Threshold();
void Display();
void LED_Control();
void RELAY_Control();
void ALARM_Control();
void main()
{
	HC138_Select(4);
	P0=0X00;
	HC138_Select(1);
	P0=0XFF;
	Humidity_threshold = Read_AT24C02(0x00);
	ds1302_init();
	while(1)
	{
		readtime();
		Humidity = Read_Humidity();
		Key_Scan();
		Display();
		LED_Control();
		RELAY_Control();
		ALARM_Control();
	}
}
void RELAY_Control()
{
	if(mode_flag==1)
	{
		if(relay_flag==1)
		{
			HC138_Select(4);
			RELAY = 1;
			BUZZER = 0;

			P2&=0x1f;
			
		}
		else
		{	
			HC138_Select(4);
			RELAY = 0;
			BUZZER = 0;

			P2&=0x1f;
		}
	}
	else
	{
		if(threshold_flag==0)
		{
			if(Humidity < Humidity_threshold)
			{	
				HC138_Select(4);
				RELAY = 1;
				BUZZER = 0;
				
				P2&=0x1f;
			}
			else
			{
				HC138_Select(4);
				RELAY = 0;
				BUZZER = 0;

				P2&=0x1f;
			}
		}
		else
		{	
			HC138_Select(4);
			RELAY = 0;
			BUZZER = 0;

			P2&=0x1f;
		}
	}
}
void ALARM_Control()
{
	if(mode_flag==1)
	{
		if(alarm_flag==0)
		{
			if(Humidity < Humidity_threshold)
			{
				HC138_Select(4);
				BUZZER = 1;
//				RELAY = 0;
				P2&=0x1f;
			}
		}
		else
		{
			HC138_Select(4);
			BUZZER = 0;
//			RELAY = 0;
			P2&=0x1f;
		}
	}
	else
	{
		HC138_Select(4);
		BUZZER = 0;
//		RELAY = 0;
		P2&=0x1f;
	}
}
void LED_Control()
{
	if(mode_flag==0)
	{
		HC138_Select(1);
		P0=0XFF;
		L1 = 0;
	}
	else
	{
		HC138_Select(1);
		P0=0XFF;
		L2 = 0;
	}
}
void Display()
{
	if((mode_flag==0)&&(threshold_flag==1))
	{
		Display_Threshold();
	}
	else
	{
		Display_Humidity_Time();
	}
}
void Display_Humidity_Time()
{
	Display_SEG_Bit(1,gettime[2]/10%10);
	Display_SEG_Bit(2,gettime[2]%10);
	Display_SEG_Bit(3,16);
	Display_SEG_Bit(4,gettime[1]/10%10);
	Display_SEG_Bit(5,gettime[1]%10);
	Display_SEG_Bit(7,Humidity/10%10);
	Display_SEG_Bit(8,Humidity%10);
}
void Display_Threshold()
{
	Display_SEG_Bit(1,16);
	Display_SEG_Bit(2,16);
	Display_SEG_Bit(7,Humidity_threshold/10%10);
	Display_SEG_Bit(8,Humidity_threshold%10);
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
void DelayK(unsigned char t)
{
	while(t--)
	{
	}
}
void Key_Scan()
{
	if(S7==0)
	{
		DelayK(20);
		if(S7==0)
		{
			while(S7==0)
			{
				Display();
			}
			mode_flag = !mode_flag;
			if(mode_flag==1)
			{
				threshold_flag = 0;
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
			if(mode_flag==0)//自动模式下进入/退出湿度阈值调整界面
			{
				threshold_flag = !threshold_flag;
			}
			else
			{
				alarm_flag = !alarm_flag;
			}
		}
	}

		if(S5==0)
	{
		DelayK(20);
		if(S5==0)
		{
			while(S5==0)
			{
				Display();
			}
			if(mode_flag==1)
			{
					//手动模式下开继电器
				relay_flag = 1;
			}
			else
			{
					//自动模式加阈值
				if(threshold_flag==1)
				{
					if(Humidity_threshold<=98)
						Humidity_threshold++;
				}
				Write_AT24C02(0x00,Humidity_threshold);
			}
		}
	}
	if(S4==0)
	{
		DelayK(20);
		if(S4==0)
		{
			while(S4==0)
			{
				Display();
			}
			if(mode_flag==1)
			{
				//手动模式下关继电器
				relay_flag = 0;
			}
			else
			{
				//自动模式减阈值
				if(threshold_flag==1)
				{
					if(Humidity_threshold>=1)
						Humidity_threshold--;
				}
				//EEPROM存储阈值
				Write_AT24C02(0x00,Humidity_threshold);
			}
			
		}
	}		
}
unsigned char Read_Humidity()
{
	unsigned char temp = 0;
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
	return (unsigned char)(temp*5.0/255*19.8);
}
void Write_AT24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0xA0);//AT24C02的写设备地址 
	IIC_WaitAck();//等待从机PCF8591应答
	IIC_SendByte(addr);//要写入的内存地址
	IIC_WaitAck();//等待从机PCF8591应答
	IIC_SendByte(dat);//要写入的数据
	IIC_WaitAck();//等待从机PCF8591应答
	IIC_Stop();//IIC总线停止信号 
}
unsigned char Read_AT24C02(unsigned char addr)
{
	unsigned char temp;
	//以下5行为伪写操作
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0xA0); //AT24C02的写设备地址
	IIC_WaitAck();//等待从机PCF8591的应答
	IIC_SendByte(addr);//要读取的内存地址
	IIC_WaitAck();//等待从机PCF8591应答
	
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0xA1);//AT24C02的读设备地址 
	IIC_WaitAck();//等待从机PCF8591的应答
	temp = IIC_RecByte();//读取目标数据
	IIC_SendAck(1);//产生一个非应答信号
	IIC_Stop(); //IIC总线停止信号 
	return temp;
}


