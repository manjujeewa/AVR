#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600   // Define baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "twimaster.c"
#include "UART_routines.h"
#include "bmp085.h"

int main(void) {
	long l;
	double d;
	char printbuff[10];
	

	//init interrupt
	sei();
	USART_Init(BAUD_PRESCALE);
	//init bmp085
	bmp085_init();

	//main loop
	for (;;)
	{
		
		l = bmp085_getpressure();
		ltoa(l, printbuff, 10);		
		transmitString(printbuff);
		TX_NEWLINE;
		
		_delay_ms(500);
	}
	
	return 0;
}
