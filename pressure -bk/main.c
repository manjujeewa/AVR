#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/pgmspace.h>
#include "twimaster.c"
#include "lcd.h"
#include "bmp085.h"

int main(void) {
	long l;
	double d;
	char printbuff[10];
	lcd_init(LCD_DISP_ON);

	//init interrupt
	sei();

	//init bmp085
	bmp085_init();

	//main loop
	for (;;)
	{
		lcd_clrscr();
		//get temperature
		d = bmp085_gettemperature();
		dtostrf(d, 4,1, printbuff);
		lcd_puts(printbuff);		
		lcd_gotoxy(8,0);

		//get altitude
		d = bmp085_getaltitude();
		dtostrf(d, 4,1, printbuff);	
		lcd_puts(printbuff);
		lcd_puts("\n");


		//get pressure
		l = bmp085_getpressure();
		ltoa(l, printbuff, 10);		
		lcd_puts(printbuff);
		
		_delay_ms(1500);
	}
	
	return 0;
}
