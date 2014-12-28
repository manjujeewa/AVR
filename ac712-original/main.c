#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define UART_BAUD_RATE 2400
#include "uart/uart.h"

#include "adc/adc.h"

#include "acs712/acs712.h"

uint16_t adc = 0;
uint16_t adchisto = 0;

int main(void)
{
	char printbuff[100];
	long l;
	double d;

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	//init adc
	adc_setchannel(1);
	adc_init();

	//init interrupt
	sei();

	//main loop
	for (;;)
	{
		//read adc and filter it
		adchisto = adc;
		adc = adc_read(1);
		adc = adc_emafilter(adc, adchisto);

		//print out results
		itoa(adc, printbuff, 10);
		uart_puts("ADC     "); uart_puts(printbuff); uart_puts("\r\n");

		l = adc_getresistence(adc, 10000);
		ltoa(l, printbuff, 10);
		uart_puts("RES     "); uart_puts(printbuff); uart_puts("\r\n");

		d = adc_getvoltage(adc, ADC_REFV);
		dtostrf(d, 10, 5, printbuff);
		uart_puts("VOL     "); uart_puts(printbuff); uart_puts("\r\n");
		d = acs712_getcurrent(adc_getvoltage(adc, ADC_REFV), ADC_REFV);
		if(d < 0.0)
			d=-d;
		dtostrf(d, 10, 5, printbuff);
		uart_puts("CURRENT "); uart_puts(printbuff); uart_puts("\r\n");

		uart_puts("\r\n");

		_delay_ms(1500);
	}
	
	return 0;
}
