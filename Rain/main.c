#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lcd.h"


static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};
  
// initialize adc
void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
  
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
}
  
// read adc value
uint16_t adc_read()
{
    ADMUX = (ADMUX)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
  
    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);
  
    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));
  
    return (ADC);
}
  
int main()
{
    uint16_t adc_result;
  
    // initialize adc and lcd
    adc_init();
	char buffer[12];
	char flot[8];

    lcd_init(LCD_DISP_ON);
	
    while(1)
    {
        adc_result = adc_read();
  
		
		lcd_clrscr();
		
		dtostrf( adc_result ,6,1, flot);
		
		lcd_puts(flot);

		_delay_us(700);
        
    }
}