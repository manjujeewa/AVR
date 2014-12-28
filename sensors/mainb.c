#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600   // Define baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "UART_routines.h"


double rounds;
double point;
double r = 0.03; // radius of the circle in meter  //define times for start and end of signal
double falling; //define overall counts
double counts;



void wait_until_key_pressed(void);
void getPasses();
void init(void);
ISR(TIMER1_OVF_vect);
ISR(TIMER1_COMPA_vect);

void init(void){
	TCCR1B = (1<<CS11|1<<WGM12|1<<ICES1);
	TIMSK = (1<<TOIE1|1<<TICIE1);
	TCNT1=0;
}

void calculate(){
	double cir = r*2*22/7;
	point = 3.6*(cir/(counts*0.000008)); // speed in Km/h
}


ISR(TIMER1_OVF_vect){		
	TCNT1=0;
	rounds++;
}

ISR(TIMER1_CAPT_vect){
	if (PIND & 0x40) { //if high level
		    falling=ICR1;
		    TCCR1B=(1<<CS11|1<<WGM12|0<<ICES1);
		    counts=falling+(rounds*65535);
	}else {
		rounds=0;
		TCNT1=0;
		TCCR1B=(1<<CS11|1<<WGM12|1<<ICES1);
	}

}

int main(void)
{	
	char buffer[12];
	char flot[8];
	USART_Init(BAUD_PRESCALE);  // Initialise USART
	init();
    	sei();

    while(1) {  

	calculate();
	dtostrf( point ,6,3, flot);
	transmitString("S");
	_delay_ms(500);
              
    }
}



























