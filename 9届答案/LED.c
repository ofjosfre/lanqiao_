///*******************************************************************************  
//* �ļ����ƣ�LED�����ʵ��
//* ʵ�����ã�J13��������ΪIO��ʽ��J5����ΪBTN��J2����Ϊ1-3��2-4
//* ʵ������ʵ��LED�������Ч��
//* Ӳ��������IAP15F2K61S2��Ƭ������Ƭ���ڲ�����Ƶ�ʣ�12M
//* ע�����Ҫ�õ�_nop_()����Ҫ����intrins.hͷ�ļ�
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
//		case 4://���ֶٹ����� ȡ����
//				HC138_A = 1;
//				HC138_B = 0;
//				HC138_C = 1;
//				break;
//		default:break;
//	}
//}
//void initTimer0(void)
//{
//	TMOD=0X00;//������ʱ������ʱ��������ģʽ0����16λ�Զ���װ�ض�ʱ��
//	//TMOD=0X01;
//	TH0=(65536-200)/256;//��ʱ��0��8λ�Ĵ���
//	TL0=(65536-200)%256;
//	EA=1;
//	ET0=1;//��ʱ��0�жϿ���
//	TR0=1;//��ʱ��0��ʼ����
//}
//void Timer0() interrupt 1
//{

//	 liuzhuandis();
//	//���ȿ���
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
//void LED_Mode1()//����LED����
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
//	if(liangdu_count<MAX)//��������
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
//				//ģʽ��1
//				//���Ʊ߽�
//				if(mode_xianshi<=3)
//				{
//					mode_xianshi++;
//				}
//			}
//			else if(shezhi==2)
//			{
//				//��ת�����100ms
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
//				//ģʽ��1
//				//���Ʊ߽�
//				if(mode_xianshi>=2)
//				{	
//					mode_xianshi--;
//				}
//				
//			}
//			else if(shezhi==2)
//			{
//				//��ת�����100ms
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
//	IIC_Start();//IIC������ʼ�ź�
//	IIC_SendByte(0x90);//����PCF8591��д������ַ
//	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
//	IIC_SendByte(0x03);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��3)��
//	//IIC_SendByte(0x01);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��1)��
//	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
//	IIC_Stop();//IIC������ʼ�ź�
//	
//	IIC_Start();
//	IIC_SendByte(0x91);//����PCF8591�Ķ�������ַ
//	IIC_WaitAck();
//	temp = IIC_RecByte();//��ȡPCF8591ͨ�������� 
//	IIC_SendAck(1);//������Ӧ���ź�
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
//	HC138_Select(3);//λѡ
//	P0 = 0x00;
//	P0 = 0x01<<(pos-1);
//	HC138_Select(2);//��ѡ
//	P0 = 0xff;
//	P0 = SMG_duanma[value];
//	DelaySMG(1);
//	P0 = 0xff;
//}


//����ʱע�⾧��12MHz
#include "stc15.h"
#include "iic.h"
#include <stdio.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int

#define KEYCOM P3                    //����IO
#define KEY_S7 0x01
#define KEY_S6 0x02
#define KEY_S5 0x04
#define KEY_S4 0x08
#define PWM_MAX 19            //pwm���� <20

uchar code smg_dis[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};
uchar code LED_Max[]= {7,7,3,3}; //ģʽ��תʱ ��������±�
uchar code LED_MODE1[]= {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar code LED_MODE2[]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
uchar code LED_MODE3[]= {0x81,0x42,0x24,0x18};
uchar code LED_MODE4[]= {0x18,0x24,0x42,0x81};
uchar code PWM_Val[]= {0,5,10,15,20}; //���ȵ���PWMֵ

uchar Smg_Buf[8]= {10,10,10,10,10,10,10,10};
uchar Led_Time[4]= {4,4,4,4}; //��תʱ��

bit LED_Switch=0;//led��ת����
bit KEY_Flag=0;
bit ADC_Flag=0;
bit SET_Flag=0;
bit Blink_Flag=0;//0.8s��˸

uchar Trg, Cont; //��ֵ
uchar Rank=1;//���յȼ�

void Delay2ms();//д����ʱ
//void UartInit(void);
void Timer0Init(void)    ;
void KeyRead ();
void KEY_Dispose (void);//��������
void Set_Show (uchar mode);
void Write_AT24 (uchar add,uchar dat);
uchar Read_AT24 (uchar add);
uchar Read_ADC ();//��ȡadc
uchar KEY_Resize (uchar sum,uchar Max,uchar Min);//����������ֵ

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
            );//�ж϶�ȡ���ļ���Ƿ��ڹ涨��Χ
    else //���ڷ�Χ���¸�ֵ
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
            KEY_Dispose();//�������¼�
            if(!SET_Flag && (Trg & KEY_S4 || Cont & KEY_S4))//��ͨ״̬��ʾ����
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
        if(ADC_Flag)//�����ж�
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

    if(SET_Flag)//��������˸��ѡλ
    {
        if( ++BlinkCount>800)
        {
            BlinkCount=0;
            Blink_Flag=~Blink_Flag;//�����ֶ���0
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
        switch(Run_Mode)//ģʽ
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
    else if(PWM < PWM_MAX&&PWM>PWM_Val[Rank])//pwm����С��20ms �Ӿ�����
    {
        P0=~0x00;P2=0x80;P2=0x00;
    }
    else
    {
        PWM=0;
    }
    /*****************/
    if(!LED_Switch)//��ͣʱ ��Ӱ������
    {
        ++LedCount;
        if(LedCount > Led_Time[Run_Mode-1]*100)
        {
            LedCount=0;
            ++i;
            if( i > LED_Max[Run_Mode-1])
            {
                i=0;
                Run_Mode++;//ģʽ�л�
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
        if(SET_MODE>=2)//�˳����� �����ʾ���� ����������ʾ ��EEPROMд����
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
            Write_AT24(0x01,Led_Time[0]);//д����
            Delay2ms();//������ʱ
            Write_AT24(0x02,Led_Time[1]);
            Delay2ms();
            Write_AT24(0x03,Led_Time[2]);
            Delay2ms();
            Write_AT24(0x04,Led_Time[3]);
            Delay2ms();
        }
    }
    if(SET_Flag)//����ģʽ
    {
        if(SET_MODE==0)
        {
            mode1=KEY_Resize(mode1,3,0);
            Set_Show(mode1);
            if(Blink_Flag)//��˸ģʽλ
            {
                Smg_Buf[1]=10;
            }
        }
        else
        {
            Led_Time[mode1]=KEY_Resize(Led_Time[mode1],12,4);
            Set_Show(mode1);
            if(Blink_Flag)//��˸��ת���
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
    char Temp;//���ַ���uchar <0ʱ�ᳬ��ȡֵ��Χ
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
    if(Led_Time[mode]<10)//����תʱ��С��1000ʱ ��<10 ����ʾǧλ
        Smg_Buf[4]=10;
    else
        Smg_Buf[4]=Led_Time[mode]/10;//1000λ
    Smg_Buf[5]=Led_Time[mode]%10;//100λ
    Smg_Buf[6]=0;
    Smg_Buf[7]=0;//��λ��ʮλһֱΪ0
}

uchar Read_ADC ()//��ȡadc
{
  unsigned char temp = 0;
	IIC_Start();//IIC������ʼ�ź�
	IIC_SendByte(0x90);//����PCF8591��д������ַ
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_SendByte(0x03);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��3)��
	//IIC_SendByte(0x01);//���Ϳ����ֽڣ�ѡ��ģ��������ģʽ(4·��������)��ͨ��(ͨ��1)��
	IIC_WaitAck();//�ȴ��ӻ�Ӧ��
	IIC_Stop();//IIC������ʼ�ź�
	
	IIC_Start();
	IIC_SendByte(0x91);//����PCF8591�Ķ�������ַ
	IIC_WaitAck();
	temp = IIC_RecByte();//��ȡPCF8591ͨ�������� 
	IIC_SendAck(1);//������Ӧ���ź�
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