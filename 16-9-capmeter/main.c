#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#define R 10000
#define STOP_V 511.0
#define FULLP_V 948.0

void getDigi(unsigned int);
void displaySSD(unsigned int n);
void init(void);

unsigned char SSD[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x67,0x54,0x1c};
unsigned char dig[] = {1,2,3,4};

volatile unsigned int count=0000;
volatile unsigned char countU =4, valMS=0, valS=0;
volatile unsigned int countMS =0;
volatile unsigned char on = 0;
volatile unsigned char c_scale = 'u';

int main(void){

	init();

	sei();// Global interrupt enable

	while(1){
		

		ADCSRA |= (1<<ADSC);

		while(ADCSRA & 1<<ADSC);
		
		
		if((ADCL | ADCH << 8)>500){
			on=0;
			double a = -R*log((FULLP_V + 24 - STOP_V)/FULLP_V);
			double c = (count)/(a*1000);
			
			if (c< 1e-9) {
				c*=1e+13;
				c_scale = 'p';
			}
			if (c< 1e-6) {
				c*=1e+10;
				c_scale = 'n';
			}
			if (c< 1e-3) {
				c*=1e+7;
				c_scale = 'u';
			}
			
			getDigi((int)c);
			while(1){
				displaySSD(1);
			}; 
		}
			
		

	}

	return 0;
}


void init(void){

	DDRC = 0xff;
	DDRA =  1<<3| 1<<4| 1<<5| 1<<6 | 1<<7 ;
	DDRD = DDRD | ~(1<<2) | 1<<5;

	PORTD = 0<<5;

	//ADMUX = 1 << MUX1 | 1 << REFS0 | 1 << REFS1;
	//ADCSRA = 1 << ADEN | 1 << ADPS1 | 1 << ADPS2 ;
	ADCSRA = 0b10000110;
	ADMUX = 0b11000000;

    	TCCR0 = 1<<FOC0 | 1<<WGM01 | 0<<WGM00 | 0<<CS02 | 1<<CS01 | 0<<CS00 ;  //prescaler 8

	OCR0=249; // compare value 250 * 8 * 1/4 MHz = 250us

	TIMSK |= 1<<OCIE0; //enable interrupt on compare

	// enableing external interrupts
	MCUCR = 1<<ISC01 | 1<<ISC00 | 1<<ISC11 | 1<<ISC10;
	GICR = 1<<INT0 | 1<<INT1;
}

ISR(TIMER0_COMP_vect){ //compare match timer0 at each 250us

	if(on){
		if (--countU==0){
			countU=4;


				if (++countMS==1000) {
					countMS=0;

					if(++valS==10){
						valS=0;
					}
				}

		}
	}

		count=valS*1000 + countMS;

}

ISR(INT0_vect){

	on = 1;
	PORTD = 1<<5;

}


ISR(INT1_vect){

	on=0;

}


void getDigi(unsigned int val){

	switch (c_scale){
		case 'p':
			dig[0] = 10;
			break;
		case 'n':
			dig[0] = 11;
			break;
		case 'u':
			dig[0] = 12;
			break;
	
	}
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
				PORTC=SSD[dig[i]] ; 
			}else{
				PORTC=SSD[dig[i]];
			}
			PORTA = (1<<i+4) ;
			_delay_ms(1);
			PORTA=0;
		}
	}
}
