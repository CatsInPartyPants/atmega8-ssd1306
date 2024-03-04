// https://radiokot.ru/articles/77/
#include "i2c.h"
#include "font.c"

//МАССИВ ДЛЯ ИНИЦИАЛИЗАЦИИ
//A0 - начало в правом углу; A1 - начало в левом углу
//C0 - начало снизу экрана; C8 - начало сверху экрана
//12 - прямая линия, 02, 22 - линия через строчку
//7F - яркость (максимум 0xFF)
unsigned char const PROGMEM init[18]=
{
	0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,0x12,
	0x81,0x7F,0xA4,0xA6,0xD5,0x80,0x8D,0x14,0xAF,
};

void PREPARATION(void)
{
	PORTC=0x00;	//РC0 SCK, РC1 SDA
	DDRC =0x00;

	PORTB=0x00;		//на всякий случай
	DDRB =0x00;		//на всякий случай

	ACSR=0x80;		//отключение компаратора для уменьшения потребления тока

	_delay_ms(500);	
}

void START (void)
{
	DA_HI;
	CL_HI;
	DA_LOW;
	CL_LOW;
}

void STOP (void)
{
	DA_LOW;
	CL_HI;
	DA_HI;
}

void SEND (unsigned char data)
{
	unsigned char k;

	for(k=8;k>0;k--)
	{
		if(data&(1<<(k-1)))
		{
			CL_LOW;
			DA_HI;
			CL_HI;
			CL_LOW;
		}
		else
		{
			CL_LOW;
			DA_LOW;
			CL_HI;
			CL_LOW;
		}
	}

	//ACK
	DA_HI;
	CL_HI;
	if(!(PINC&(1<<1)))
	CL_LOW;
}

void OLED_INIT (void)
{
	unsigned char k;

	START();
	SEND(NAME);

	for(k=0;k<18;k++)
	{
		SEND(COM);
		SEND(pgm_read_byte(init+k));
	}

	STOP();
}

void CLR_SCR (void)
{
	unsigned char k, kk;

	START();
	SEND(NAME);

	//выбираем Horizontal Addressing Mode (заполнение экрана по типу того как читаем книгу)
	SEND(COM);
	SEND(0x20);
	SEND(COM);
	SEND(0x00);

	//заполняем строку слева направо
	SEND(DATS);
	for(kk=0;kk<8;kk++) 
	{
		for(k=0;k<128;k++) SEND(0x00);	//LSB вверху, MSB снизу
	}

	STOP();
}


void PUT_TEXT(unsigned char* data, short column, short page)
{
	unsigned int k;
	unsigned int length = 0;
	unsigned char letter;
	while(letter != '\0')
	{
		length+=1;
		letter = data[length];
	}
	START();
	SEND(NAME);
	
	SEND(COM);
	SEND(0x21);		//Set Column Address (21h)
	SEND(COM);
	SEND(column);
	SEND(COM);
	SEND(126);
			
	SEND(COM);
	SEND(0x22);		//Set Page Address (22h)
	SEND(COM);
	SEND(page);		
	SEND(COM);
	SEND(7);
	STOP();
			
	for(int i = 0; i < length; i++)
	{
		START();
		SEND(NAME);	
		SEND(DATS);
		int g = data[i]-32;
		for(k=0;k<3;k++) SEND(pgm_read_byte(SmallFont[g]+k));		//рисуем верхнюю часть цифры
		for(k=3;k<6;k++) SEND(pgm_read_byte(SmallFont[g]+k));	//рисуем нижнюю часть цифры
		STOP();
	} 
}
