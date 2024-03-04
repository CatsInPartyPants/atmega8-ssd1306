#define F_CPU 8000000UL

#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	"i2c.h"

int main (void)
{
	unsigned char temperature[] = "Temperature: xx C";
	unsigned char humidity[] = "Humidity: xx %";
	PREPARATION();
	OLED_INIT();
	CLR_SCR();		

	while(1)		
	{
		PUT_TEXT(temperature, 0, 0);
		PUT_TEXT(humidity, 0, 2);
		_delay_ms(2500);
	}	
	

return 0;
}
