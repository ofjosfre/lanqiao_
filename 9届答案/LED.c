///*******************************************************************************  
//* 文件名称：LED跑马灯实验
//* 实验配置：J13跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
//* 实验现象：实现LED的跑马灯效果
//* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：12M
//* 注意事项：要用到_nop_()必须要包含intrins.h头文件
//*******************************************************************************/
//#include <stc15.h>
//#include "iic.h"
//sbit HC138_A = P2^5;
//sbit HC138_B = P2^6;
//sbit HC138_C = P2^7;
//sbit LED1 = P0^0;
//sbit LED2 = P0^1;
//sbit LED3 = P0^2;
//sbit LED4 = P0^3;
//sbit LED5 = P0^4;
//sbit LED6 = P0^5;
//sbit LED7 = P0^6;
//sbit LED8 = P0^7;
//sbit S7 = P3^0;
//sbit S6 = P3^1;
//sbit S5 = P3^2;
//sbit S4 = P3^3;
//unsigned char code SMG_duanma[18]=
//{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
//     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f
//};
////unsigned int count=0;
//unsigned int mode1_count=0;
//unsigned int mode2_count=0;
//unsigned int mode3_count=0;
//unsigned int mode4_count=0;
//unsigned char i = 0;
//unsigned char j = 8;
//unsigned char m = 0;
//unsigned char n = 0;
//unsigned char mode = 0;
//unsigned char shezhi = 0;
//unsigned char liangdu_count = 0;
//unsigned char MAX = 0;
//unsigned char liuzhuan = 0;
//unsigned char AIN3_Value;
//unsigned char mode1_jiange = 4;
//unsigned char mode2_jiange = 4;
//unsigned char mode3_jiange = 4;
//unsigned char mode4_jiange = 4;
//unsigned char mode_xianshi = 1;
//unsigned char liangdeng ;
//void initTimer0(void);
//void PWM_MAX();
//void LED_Mode2();
//void LED_Mode1();
//void LED_Mode3();
//void LED_Mode4();
//void Delay10ms();
//unsigned char Read_AIN3();
////void LED_Mode_Select();
//void LED_LD_Secect();
//void Delay_K(unsigned char time);
//void HC138_Select(unsigned char i);
//void LED_LD_Secect();
//void Delaynms(unsigned int times)	;
//void KEY_Scan();
//void display_data();
//void Display_SEG_Bit(unsigned char pos,unsigned char value);
//void DelaySMG(unsigned int times);
//void liuzhuandis();
//void main()
//{
//	HC138_Select(4);
//	P0=0X00;
//	HC138_Select(1);
//	P0=0XFF;
//	initTimer0();
//	while(1)
//	{
//		PWM_MAX();
//		KEY_Scan();
//		if(liangdeng ==1)
//		{
//		switch(mode)
//	{
//			case 0:
//			HC138_Select(1);
//			P0 = 0XFF&(~(1<<i));
//			break;
//			case 1:
//			HC138_Select(1);
//   		P0 = 0XFF&(~(1<<(j-1)));
//			break;
//			case 2:
//			switch(m)
//			{
//					case 0:
//					HC138_Select(1);
//					P0 = 0xFF;
//					LED1=0;LED8=0;
//					break;
//					
//					case 1:
//					HC138_Select(1);
//					P0 = 0xFF;
//					LED2=0;LED7=0;
//					break;
//					
//					case 2:
//					HC138_Select(1);
//					P0 = 0xFF;
//					LED3=0;LED6=0;
//					break;
//					
//					case 3:
//					HC138_Select(1);
//					P0 = 0xFF;
//					LED4=0;LED5=0;
//					break;
//					default:break;
//		}
//			break;
//			case 3:
//			switch(n)
//			{
//				case 0:
//				HC138_Select(1);
//				P0 = 0xFF;
//				LED4=0;LED5=0;
//				break;
//				
//				case 1:
//				HC138_Select(1);
//				P0 = 0xFF;
//				LED3=0;LED6=0;
//				break;
//				
//				case 2:
//				HC138_Select(1);
//				P0 = 0xFF;
//				LED2=0;LED7=0;
//				break;
//				
//				case 3:
//				HC138_Select(1);
//				P0 = 0xFF;
//				LED1=0;LED8=0;
//				break;
//				default:break;
//			}
//			break;
//			default:break;
//	}
//		}
//		else
//		{
//				HC138_Select(1);
//				P0 = 0xFF;
//		}
//		//display_data();
//	}

//}
//void HC138_Select(unsigned char i)
//{
//	switch(i)
//	{
//		case 1://LED
//				HC138_A = 0;
//				HC138_B = 0;
//				HC138_C = 1;
//				break;
//		case 2://SEG
//				HC138_A = 1;
//				HC138_B = 1;
//				HC138_C = 1;
//				break;
//		case 3://Bit
//				HC138_A = 0;
//				HC138_B = 1;
//				HC138_C = 1;
//				break;
//		case 4://达林顿管阵列 取反门
//				HC138_A = 1;
//				HC138_B = 0;
//				HC138_C = 1;
//				break;
//		default:break;
//	}
//}
//void initTimer0(void)
//{
//	TMOD=0X00;//用作定时器，定时器工作于模式0：即16位自动重装载定时器
//	//TMOD=0X01;
//	TH0=(65536-200)/256;//定时器0高8位寄存器
//	TL0=(65536-200)%256;
//	EA=1;
//	ET0=1;//定时器0中断开关
//	TR0=1;//定时器0开始计数
//}
//void Timer0() interrupt 1
//{

//	 liuzhuandis();
//	//亮度控制
//}
//void liuzhuandis()
//{
//	
//		if(liuzhuan==1)
//	{
//		
//			liangdu_count++;
//			LED_LD_Secect();
//			if(liangdu_count==100)
//			{
//				liangdu_count=0;
//				mode1_count++;
//				mode2_count++;
//				mode3_count++;
//				mode4_count++;
//				if((mode1_count==20)&&(mode==0))
//				{
//					mode1_count = 0;
//					mode2_count = 0;
//					mode3_count = 0;
//					mode4_count = 0;
//					LED_Mode1();
//				}
//				
//				if((mode2_count==40)&&(mode==1))
//				{
//					mode1_count = 0;
//					mode2_count = 0;
//					mode3_count = 0;
//					mode4_count = 0;
//					LED_Mode2();
//				}
//				
//				if((mode3_count==80)&&(mode==2))
//				{
//					mode1_count = 0;
//					mode2_count = 0;
//					mode3_count = 0;
//					mode4_count = 0;
//					LED_Mode3();
//				}
//				
//				if((mode4_count==200)&&(mode==3))
//				{
//					mode1_count = 0;
//					mode2_count = 0;
//					mode3_count = 0;
//					mode4_count = 0;
//					LED_Mode4();
//					
//				}
//			}
//	}
//	else
//	{
//		liangdu_count = 0;
//		mode = 8;
//		mode1_count = 0;
//		mode2_count = 0;
//		mode3_count = 0;
//		mode4_count = 0;
//		i = 0;
//		m= 0;
//		n = 0;
//		j = 8;
//		HC138_Select(1);
//		P0 = 0XFF;
//	}
//}
//void LED_Mode1()//单个LED右移
//{
//		i++;
//		if(i==8)
//		{
//			i = 0;
//			mode = 1;
//		}
//}
//void LED_Mode2()
//{
//		j--;
//		if(j==0)
//		{
//			j = 8;
//			mode = 2;
//		}
//}
//void LED_Mode3()
//{
//	m++;
//	if(m==4)
//	{
//		m = 0;
//		mode = 3;
//	}
//}

//void LED_Mode4()
//{
//	n++;
//	if(n==4)
//	{
//		n = 0;
//		mode = 0;
//	}
//}
////void LED_Mode_Select()
////{
////	switch(mode)
////	{
////			case 0:LED_Mode1();break;
////			case 1:LED_Mode2();break;
////			case 2:LED_Mode3();break;
////			case 3:LED_Mode4();break;
////			default:break;
////	}
////}
//void LED_LD_Secect()
//{
//	if(liangdu_count<MAX)//控制亮度
//	{
//		liangdeng = 1;
////		switch(mode)
////	{
////			case 0:
////			HC138_Select(1);
////			P0 = 0XFF&(~(1<<i));
////			break;
////			case 1:
////			HC138_Select(1);
////   		P0 = 0XFF&(~(1<<(j-1)));
////			break;
////			case 2:
////			switch(m)
////			{
////					case 0:
////					HC138_Select(1);
////					P0 = 0xFF;
////					LED1=0;LED8=0;
////					break;
////					
////					case 1:
////					HC138_Select(1);
////					P0 = 0xFF;
////					LED2=0;LED7=0;
////					break;
////					
////					case 2:
////					HC138_Select(1);
////					P0 = 0xFF;
////					LED3=0;LED6=0;
////					break;
////					
////					case 3:
////					HC138_Select(1);
////					P0 = 0xFF;
////					LED4=0;LED5=0;
////					break;
////					default:break;
////		}
////			break;
////			case 3:
////			switch(n)
////			{
////				case 0:
////				HC138_Select(1);
////				P0 = 0xFF;
////				LED4=0;LED5=0;
////				break;
////				
////				case 1:
////				HC138_Select(1);
////				P0 = 0xFF;
////				LED3=0;LED6=0;
////				break;
////				
////				case 2:
////				HC138_Select(1);
////				P0 = 0xFF;
////				LED2=0;LED7=0;
////				break;
////				
////				case 3:
////				HC138_Select(1);
////				P0 = 0xFF;
////				LED1=0;LED8=0;
////				break;
////				default:break;
////			}
////			break;
////			default:break;
////	}
//	}
//	
//	else
//	{
//		liangdeng = 0;
////		HC138_Select(1);
////		P0 = 0xFF;

//	}
//}

//void Delay10ms()		//@12.000MHz
//{
//	unsigned char i, j;

//	i = 117;
//	j = 184;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
//void Delay_K(unsigned char time)
//{
//	while(time--)
//	{
//		display_data();
//	}
//}
//void KEY_Scan()
//{
//	if(S7==0)
//	{
//		Delay_K(30);
////		Delay10ms();Delay10ms();Delay10ms();
////		Delay10ms();						Delay10ms();
//		if(S7==0)
//		{
//			while(S7==0){
//				display_data();
//			}
//			liuzhuan = !liuzhuan;
//			mode = 0;
//		}
//	}
//	
//		if(S6==0)
//	{
////		Delay10ms();Delay10ms();Delay10ms();
////		Delay10ms();						Delay10ms();
//		Delay_K(30);
//		if(S6==0)
//		{
//			while(S6==0)
//			{
//				display_data();
//			}
//			shezhi++;
//			if(shezhi>=3)
//			{
//				shezhi = 0;
//			}
//		}
//	}
//	
//			if(S5==0)
//	{
////		Delay10ms();Delay10ms();Delay10ms();
////		Delay10ms();						Delay10ms();
//		Delay_K(30);
//		if(S5==0)
//		{
//			while(S5==0)
//			{
//				display_data();
//			}
//			if(shezhi==1)
//			{
//				//模式加1
//				//控制边界
//				if(mode_xianshi<=3)
//				{
//					mode_xianshi++;
//				}
//			}
//			else if(shezhi==2)
//			{
//				//流转间隔加100ms
//				switch(mode_xianshi)
//				{
//					case 1:
//					if(mode1_jiange<=11)
//						mode1_jiange++;
//					break;
//					
//					case 2:
//					if(mode2_jiange<=11)
//						mode2_jiange++;
//					break;
//					
//					case 3:
//					if(mode3_jiange<=11)
//						mode3_jiange++;
//					break;
//					
//					case 4:
//					if(mode4_jiange<=11)
//						mode4_jiange++;
//					break;
//					default:break;
//				}
//			}
//			else
//			{
//				
//			}
//		}
//	}
//	
//			if(S4==0)
//	{
////		Delay10ms();Delay10ms();Delay10ms();
////		Delay10ms();						Delay10ms();
//		Delay_K(30);
//		if(S4==0)
//		{
//			while(S4==0)
//			{
//				display_data();
//			}
//			if(shezhi==1)
//			{
//				//模式减1
//				//控制边界
//				if(mode_xianshi>=2)
//				{	
//					mode_xianshi--;
//				}
//				
//			}
//			else if(shezhi==2)
//			{
//				//流转间隔减100ms
//							switch(mode_xianshi)
//				{
//					case 1:
//					if(mode1_jiange>=2)
//						mode1_jiange--;
//					break;
//					
//					case 2:
//					if(mode2_jiange>=2)
//						mode2_jiange--;
//					break;
//					
//					case 3:
//					if(mode3_jiange>=2)
//						mode3_jiange--;
//					break;
//					
//					case 4:
//					if(mode4_jiange>=2)
//						mode4_jiange--;
//					break;
//					default:break;
//				}
//			}
//			else
//			{
//				
//			}
//		}
//	}
//	
//}
//void PWM_MAX()
//{
//		AIN3_Value = Read_AIN3();
//		if(AIN3_Value<64)
//		{
//			MAX =25;
//		}
//		else if(AIN3_Value<128)
//		{
//			MAX =50;
//		}
//		else if(AIN3_Value<192)
//		{
//			MAX =50;
//		}
//		else
//		{
//			MAX = 100;
//		}
//}
//unsigned char Read_AIN3()
//{
//	unsigned char temp = 0;
//	IIC_Start();//IIC总线起始信号
//	IIC_SendByte(0x90);//发送PCF8591的写操作地址
//	IIC_WaitAck();//等待从机应答
//	IIC_SendByte(0x03);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道3)。
//	//IIC_SendByte(0x01);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道1)。
//	IIC_WaitAck();//等待从机应答
//	IIC_Stop();//IIC总线起始信号
//	
//	IIC_Start();
//	IIC_SendByte(0x91);//发送PCF8591的读操作地址
//	IIC_WaitAck();
//	temp = IIC_RecByte();//读取PCF8591通道的数据 
//	IIC_SendAck(1);//产生非应答信号
//	IIC_Stop();
//	return temp;
//}
//void display_data()
//{
////	HC138_Select(4);
////	P0=0X00;
////	HC138_Select(1);
////	P0=0XFF;
//	if(shezhi!=0)
//	{
//		Display_SEG_Bit(1,16);
//		Display_SEG_Bit(2,mode_xianshi);
//		Display_SEG_Bit(3,16);
//		switch(mode_xianshi)
//		{
//			case 1:
//			if(mode1_jiange>=10)
//			{
//				Display_SEG_Bit(5,mode1_jiange/10%10);
//				Display_SEG_Bit(6,mode1_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			else
//			{
//				Display_SEG_Bit(6,mode1_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			break;
//			case 2:
//			if(mode2_jiange>=10)
//			{
//				Display_SEG_Bit(5,mode2_jiange/10%10);
//				Display_SEG_Bit(6,mode2_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			else
//			{
//				Display_SEG_Bit(6,mode2_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			break;
//			case 3:
//			if(mode3_jiange>=10)
//			{
//				Display_SEG_Bit(5,mode3_jiange/10%10);
//				Display_SEG_Bit(6,mode3_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			else
//			{
//				Display_SEG_Bit(6,mode3_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			break;
//			case 4:
//			if(mode4_jiange>=10)
//			{
//				Display_SEG_Bit(5,mode4_jiange/10%10);
//				Display_SEG_Bit(6,mode4_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			else
//			{
//				Display_SEG_Bit(6,mode4_jiange%10);
//				Display_SEG_Bit(7,0);
//				Display_SEG_Bit(8,0);
//			}
//			break;
//		}
//	}

//}
//void DelaySMG(unsigned int times)		//@12.000MHz
//{
//	unsigned char i,j;
//	unsigned int n;
//	for(n = 0;n < times;n++)
//	{
//		i = 12;
//		j = 50;
//		do
//		{
//			while (--j);
//		}
//		while (--i);
//	}

//}
//void Display_SEG_Bit(unsigned char pos,unsigned char value)
//{
//	HC138_Select(3);//位选
//	P0 = 0x00;
//	P0 = 0x01<<(pos-1);
//	HC138_Select(2);//段选
//	P0 = 0xff;
//	P0 = SMG_duanma[value];
//	DelaySMG(1);
//	P0 = 0xff;
//}


//下载时注意晶振12MHz
#include "stc15.h"
#include "iic.h"
#include <stdio.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int

#define KEYCOM P3                    //按键IO
#define KEY_S7 0x01
#define KEY_S6 0x02
#define KEY_S5 0x04
#define KEY_S4 0x08
#define PWM_MAX 19            //pwm周期 <20

uchar code smg_dis[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};
uchar code LED_Max[]= {7,7,3,3}; //模式轮转时 数组最大下标
uchar code LED_MODE1[]= {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar code LED_MODE2[]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
uchar code LED_MODE3[]= {0x81,0x42,0x24,0x18};
uchar code LED_MODE4[]= {0x18,0x24,0x42,0x81};
uchar code PWM_Val[]= {0,5,10,15,20}; //亮度调节PWM值

uchar Smg_Buf[8]= {10,10,10,10,10,10,10,10};
uchar Led_Time[4]= {4,4,4,4}; //轮转时间

bit LED_Switch=0;//led轮转开关
bit KEY_Flag=0;
bit ADC_Flag=0;
bit SET_Flag=0;
bit Blink_Flag=0;//0.8s闪烁

uchar Trg, Cont; //键值
uchar Rank=1;//光照等级

void Delay2ms();//写入延时
//void UartInit(void);
void Timer0Init(void)    ;
void KeyRead ();
void KEY_Dispose (void);//按键处理
void Set_Show (uchar mode);
void Write_AT24 (uchar add,uchar dat);
uchar Read_AT24 (uchar add);
uchar Read_ADC ();//读取adc
uchar KEY_Resize (uchar sum,uchar Max,uchar Min);//按键调整数值

void main (void)
{
    uchar ADC_Val;
    P0=0x00;P2=0xa0;P2=0x00;
    P0=0xff,P2=0x80,P2=0x00;
    //UartInit();
    Timer0Init();
    Led_Time[0]=Read_AT24(0x01);
    Led_Time[1]=Read_AT24(0x02);
    Led_Time[2]=Read_AT24(0x03);
    Led_Time[3]=Read_AT24(0x04);

    if( Led_Time[0]>=4 && Led_Time[1]>=4 && Led_Time[2]>=4 && Led_Time[3]>=4  &&
                Led_Time[0]<=12 && Led_Time[1]<=12 && Led_Time[2]<=12 && Led_Time[3]<=12
            );//判断读取到的间隔是否在规定范围
    else //不在范围重新赋值
    {
        Led_Time[0]=4;
        Led_Time[1]=4;
        Led_Time[2]=4;
        Led_Time[3]=4;
    }
    while(1)
    {
        if(KEY_Flag)
        {
            KEY_Flag=0;
            KEY_Dispose();//处理按键事件
            if(!SET_Flag && (Trg & KEY_S4 || Cont & KEY_S4))//普通状态显示亮度
            {
                Smg_Buf[6]=11;
                Smg_Buf[7]=Rank;
            }
            else if (!SET_Flag)
            {
                Smg_Buf[6]=10;
                Smg_Buf[7]=10;
            }
        }
        if(ADC_Flag)//亮度判断
        {
            ADC_Flag=0;
            ADC_Val=Read_ADC();
            if(ADC_Val<=69)
                Rank=1;
            else if (ADC_Val>69&&ADC_Val<=131)
                Rank=2;
            else if (ADC_Val>131&&ADC_Val<=193)
                Rank=3;
            else if (ADC_Val>193&&ADC_Val<=255)
                Rank=4;
        }
    }
}

void Timer0Init(void)        //1ms@11.0592MHz
{
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TL0 = 0xCD;
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA  = 1;
}

void TIME0() interrupt 1
{
    static uint LedCount =0;
    static uchar KeyCount =0;
    static uchar SmgCount =0;
    static uchar SmgLen=0;
    static uchar AdcCount=0;
    static uint BlinkCount=0;
    static uchar i =0;
    static uchar PWM;
    static uchar  Run_Mode =1;

    if(++KeyCount>10)
    {
        KeyCount=0;
        KEY_Flag=1;
    }

    if(++AdcCount>100)
    {
        AdcCount=0;
        ADC_Flag=1;
    }

    if(SET_Flag)//设置下闪烁所选位
    {
        if( ++BlinkCount>800)
        {
            BlinkCount=0;
            Blink_Flag=~Blink_Flag;//不可手动清0
        }
    }
    if(++SmgCount>1)
    {
        SmgCount=0;
        P0=~smg_dis[Smg_Buf[SmgLen]];P2=0xe0;P2=0x00;
        P0=1<<SmgLen;P2=0xc0;P2=0x00;
        if(++SmgLen>7)SmgLen=0;
    }
    /*****************/
    PWM++;
    if(PWM <= PWM_Val[Rank])
    {
        switch(Run_Mode)//模式
        {
        case 1 :
            P0=~LED_MODE1[i];
            break;
        case 2 :
            P0=~LED_MODE2[i];
            break;
        case 3 :
            P0=~LED_MODE3[i];
            break;
        case 4 :
            P0=~LED_MODE4[i];
            break;
        }
        P2=0x80;P2=0x00;
    }
    else if(PWM < PWM_MAX&&PWM>PWM_Val[Rank])//pwm周期小于20ms 视觉残留
    {
        P0=~0x00;P2=0x80;P2=0x00;
    }
    else
    {
        PWM=0;
    }
    /*****************/
    if(!LED_Switch)//暂停时 不影响亮度
    {
        ++LedCount;
        if(LedCount > Led_Time[Run_Mode-1]*100)
        {
            LedCount=0;
            ++i;
            if( i > LED_Max[Run_Mode-1])
            {
                i=0;
                Run_Mode++;//模式切换
                if(Run_Mode==5)Run_Mode=1;
            }
        }
    }
}

void KeyRead (void)
{
    uchar ReadData = KEYCOM ^ 0xff;
    Trg = ReadData & (ReadData ^ Cont);
    Cont = ReadData;
}

void KEY_Dispose (void)
{
    static uchar SET_MODE=0,mode1=0;
    KeyRead();
    if( Trg & KEY_S7 )
    {
        LED_Switch=~LED_Switch;
    }
    if( Trg & KEY_S6 )
    {
        if(!SET_Flag)
        {
            SET_MODE=0;
            SET_Flag=1;
            mode1=0;
        }
        else
            SET_MODE++;
        if(SET_MODE>=2)//退出设置 清空显示缓存 便于亮度显示 向EEPROM写入间隔
        {
            SET_MODE=0;
            SET_Flag=0;
            Smg_Buf[0]=10;
            Smg_Buf[1]=10;
            Smg_Buf[2]=10;
            Smg_Buf[3]=10;
            Smg_Buf[4]=10;
            Smg_Buf[5]=10;
            Smg_Buf[6]=10;
            Smg_Buf[7]=10;
            Write_AT24(0x01,Led_Time[0]);//写入间隔
            Delay2ms();//必须延时
            Write_AT24(0x02,Led_Time[1]);
            Delay2ms();
            Write_AT24(0x03,Led_Time[2]);
            Delay2ms();
            Write_AT24(0x04,Led_Time[3]);
            Delay2ms();
        }
    }
    if(SET_Flag)//设置模式
    {
        if(SET_MODE==0)
        {
            mode1=KEY_Resize(mode1,3,0);
            Set_Show(mode1);
            if(Blink_Flag)//闪烁模式位
            {
                Smg_Buf[1]=10;
            }
        }
        else
        {
            Led_Time[mode1]=KEY_Resize(Led_Time[mode1],12,4);
            Set_Show(mode1);
            if(Blink_Flag)//闪烁轮转间隔
            {
                Smg_Buf[4]=10;
                Smg_Buf[5]=10;
                Smg_Buf[6]=10;
                Smg_Buf[7]=10;
            }
        }
    }
}

uchar KEY_Resize (uchar sum,uchar Max,uchar Min)
{
    char Temp;//无字符型uchar <0时会超过取值范围
    Temp=sum;
    if(Trg & KEY_S5)
    {
        if( ++Temp > Max ) Temp=Max;
    }
    if(Trg & KEY_S4)
    {
        if( --Temp < Min ) Temp=Min;
    }
    return Temp;
}

//void UartInit(void)        //9600bps@11.0592MHz
//{
//    SCON = 0x50;
//    AUXR |= 0x01;
//    AUXR |= 0x04;
//    T2L = 0xE0;
//    T2H = 0xFE;
//    AUXR |= 0x10;
//    TI=1;
//}

void Set_Show (uchar mode)
{
    Smg_Buf[0]=11;
    Smg_Buf[1]=mode+1;
    Smg_Buf[2]=11;
    Smg_Buf[3]=10;
    if(Led_Time[mode]<10)//当轮转时间小于1000时 即<10 不显示千位
        Smg_Buf[4]=10;
    else
        Smg_Buf[4]=Led_Time[mode]/10;//1000位
    Smg_Buf[5]=Led_Time[mode]%10;//100位
    Smg_Buf[6]=0;
    Smg_Buf[7]=0;//个位与十位一直为0
}

uchar Read_ADC ()//读取adc
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
	return temp;
}

void Write_AT24 (uchar add,uchar dat)
{
    IIC_Start();
    IIC_SendByte(0xa0);
    IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();
    IIC_SendByte(dat);
    IIC_WaitAck();
    IIC_Stop();
}

uchar Read_AT24 (uchar add)
{
    uchar Temp;
    IIC_Start();
    IIC_SendByte(0xa0);
    IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(0xa1);
    IIC_WaitAck();
    Temp=IIC_RecByte();
    IIC_SendAck(1);
    IIC_Stop();
    return Temp;
}

void Delay2ms()        //@11.0592MHz
{
    unsigned char i, j;

    _nop_();
    _nop_();
    i = 22;
    j = 128;
    do
    {
        while (--j);
    } while (--i);
}