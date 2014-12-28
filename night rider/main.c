#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void){
	DDRC= 255;
	DDRA= 1<<3;
	PORTA= 1<<3;
	
	
	
	while(1){ 
		PORTC=0x01;
		int n=0;
		while(n<7){
			
			PORTC = PORTC << 1;
			n++;
			_delay_ms(800);

		}
		
		while(n>0){
			
			PORTC = PORTC >> 1;
			n--;
			_delay_ms(200);

		}
	}
return 0;


}