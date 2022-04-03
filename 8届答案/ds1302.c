/*
  ����˵��: DS1302��������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "stc15.h"
#include "ds1302.h"
//#include <intrins.h>

sbit SCK=P1^7;		
sbit IO=P2^3;		
sbit RST = P1^3;   // DS1302��λ												
 
unsigned char read( unsigned char address )
{
 	unsigned char i,dat1,dat2,temp;
 	RST=0;
 	SCK=0;
 	RST=1;
 	writebyte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(IO)
		{
 		temp|=0x80;	
		}
 		SCK=1;
	} 
	IO=0;
	dat1 = temp/16;
	dat2 = temp%16;
	temp = dat1*10 + dat2;
	return (temp);			
}

void writebyte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SCK=0;
		IO=dat&0x01;
		dat>>=1;
		SCK=1;
	}
} 
void write(unsigned char add,unsigned char dat)
{
	unsigned char num;
	
	RST=0;
	SCK=0;
	RST=1;
	writebyte(add);
	num=(dat/10<<4)+(dat%10);// 55
	writebyte(num);
	RST=0;
}



