#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void getDigi(unsigned int);
void displaySSD(unsigned int n);
void init(void);

unsigned char SSD[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char dig[] = {1,2,3,4};


int main(void){

	init();

	sei();// Global interrupt enable

	while(1){
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & 1<<ADSC);
		double c = ((ADCL | ADCH << 8)*256)/(1023);
		getDigi((int)c);
		displaySSD(1);
	}

	return 0;
}


void init(void){

	DDRC = 0xff;
	DDRA =  1<<3| 1<<4| 1<<5| 1<<6 | 1<<7 ;
	DDRD = DDRD | ~(1<<2) | 1<<4 | 1<<5;	
	//ADMUX = 1 << MUX1 | 1 << REFS0 | 1 << REFS1;
	//ADCSRA = 1 << ADEN | 1 << ADPS1 | 1 << ADPS2 ;
	ADCSRA = 0b10000110;
	ADMUX = 0b11000000;
}



void getDigi(unsigned int val){

	dig[0] = val%10;
	dig[1] = (val%100)/10;
	dig[2] = (val/100)%10;
	dig[3] = val/1000;
}


void displaySSD(unsigned int n){

	unsigned int j;

	for(j=0;j<n;j++){
		unsigned char i ;

		for(i=0;i != 4;i++){


			if(i==3) {
				PORTC=SSD[dig[i]] ; //Displaying Dot
			}else{
				PORTC=SSD[dig[i]];
			}
			PORTA = (1<<i+4) ;
			_delay_ms(1);
			PORTA=0;
		}
	}
}
