#ifndef __DS1302_H
#define __DS1302_H

void writebyte(unsigned char dat);
void write(unsigned char add,unsigned char dat);
unsigned char read( unsigned char address );
#endif
