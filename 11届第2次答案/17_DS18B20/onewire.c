/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include "reg52.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	while(t--);
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(50);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(50);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(50);
	}
	return dat;
}

//DS18B20设备初始化
bit Init_DS18B20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(120);
  	DQ = 0;
  	Delay_OneWire(800);
  	DQ = 1;
  	Delay_OneWire(100); 
    initflag = DQ;     
  	Delay_OneWire(50);
  
  	return initflag;
}

float read_temperature()
{
	unsigned char low,high;
	unsigned int temp;
	float value;
	Init_DS18B20();//DS18B20设备复位初始化
	Write_DS18B20(0xCC);//跳过ROM操作指令
	Write_DS18B20(0x44);//开始温度转换
	
	Init_DS18B20();//DS18B20设备复位初始化
	Write_DS18B20(0xCC);//跳过ROM操作指令
	Write_DS18B20(0xBE);//开始读取高速暂存器
	low = Read_DS18B20();//读取温度数据的低8位
	high = Read_DS18B20();//读取温度数据的高8位
	temp = high&0x07;//MSB的低3位是温度位，其他5位为符号位
	temp = temp<<8;//左移8位
	temp = temp | low;
	value = temp * 0.0625;//分辨率
	return value;
}






