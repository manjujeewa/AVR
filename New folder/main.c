#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void){
	DDRC = 255;
	DDRA = 1<<3;
	PORTA = 1<<3;
	
	while(1){
	
		PORTC=0xff;	
		_delay_ms(100);
		PORTC=0;
		_delay_ms(300);
		
	}
	
	return 0;

}