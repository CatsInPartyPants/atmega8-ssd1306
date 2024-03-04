#ifndef _I2C_H
#define _I2C_H

#define F_CPU 8000000UL
#include <avr/pgmspace.h>
#include <util/delay.h>


#define		CL_HI		DDRC&=~(1<<0)	//PORTC.0 = SCL = SCK
#define		CL_LOW		DDRC|=(1<<0)	//PORTC.0
#define		DA_HI		DDRC&=~(1<<1)	//PORTC.1 = SDA
#define		DA_LOW		DDRC|=(1<<1)	//PORTC.1


#define		NAME		0b01111000		//ADRESS
#define		DATS		0b01000000		//send multiple bytes of data
#define		DAT			0b11000000		//send one byte of data
#define		COM			0b10000000		//send one byte of command

void PREPARATION(void);
void START(void);
void STOP(void);
void SEND(unsigned char);
void OLED_INIT(void);
void CLR_SCR(void);
void PUT_TEXT(unsigned char*, short, short);
#endif
