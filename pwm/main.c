#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600   // Define baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
  
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "ADC.h"
#include "acs712.h"
#include "system.h"

	char int_buffer[10];
    char printbuff[10];
	char brightness;
	char charging = 1;

void main(){
    
    
	system_init();
	

    pwm_init();
	
    
	lcd_init(LCD_DISP_ON);
	adc_init();
	

	//init interrupt
	sei();
	lcd_puts("Charging Started");
	
    while(1){
		green_on();
		red_on();
		
		MPPT_out();
		charging = 0;
		lcd_puts("Charging Complete");
		while (1){
			green_on();
			red_on();
			_delay_ms(500);
			green_off();
			red_off();
			_delay_ms(500);
			
			if (charging == 1) break; 
		}
	
	}
}

ISR(INT0_vect){

	
	PORTA ^= 1 <<PORTA0;
	//while((PINA & (1<<PORTA0)));
	_delay_ms(1000);
	charging = 1;

}


ISR(INT1_vect){

	PORTB ^= 1 <<PORTB1;
	//while((PINB & (1<<PORTB1)));
	_delay_ms(1000);
	

}