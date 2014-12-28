#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

int main(void){

DDRC = 1  << PORTC2;
PORTC = 0b00000100;

	while(1){
	
		PORTC ^= 1  << PORTC2;
		_delay_ms(500);

	}

return 0;
}
