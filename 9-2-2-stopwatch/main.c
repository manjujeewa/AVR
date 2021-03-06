#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void getdigi(unsigned int);
void displaySSD(unsigned int n);
void init(void);

unsigned char SSD[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char dig[] = {1,2,3,4};

volatile unsigned int count=2088;
volatile unsigned char countU =4, valMS=0, valS=0;
volatile unsigned int countMS =0;
volatile unsigned char on = 0;

int main(void){
		
	init();
	
	sei();// Global interrupt enable
	
	while(1){
				
		getDigi(count);
		displaySSD(1);
				
	}
		
	return 0;
}






void init(void){
	DDRC = 0xff;
	DDRA = 1<<3 | 1<<4| 1<<5| 1<<6 | 1<<7;
	
	DDRD = DDRD | ~(1<<2);
	PORTA = 1<<3;
	
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
	
}


ISR(INT1_vect){

	on=0;

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
				PORTC=SSD[dig[i]] + 128; //Displaying Dot
			}else{
				PORTC=SSD[dig[i]];
			}
			PORTA = 1<<i+4;
			_delay_ms(1);
			PORTA=0;
		}
	}
}
