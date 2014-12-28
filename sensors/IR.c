#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "IR.h"

// define your data pin right now its given to portc 5
// make sure the clock is at 8MHz
//gethumidity() will return the humidity

double rounds;
double point;
double r = 0.03;

double falling;

double counts;


void init_IR(void)
{

	TCCR1B = (1<<CS11|1<<WGM12|1<<ICES1);
	TIMSK = (1<<TOIE1|1<<TICIE1);
	TCNT1=0;
	
}

double calculate(void){
	double cir = r*2*22/7;

	return (double)1.0; //(3.6*(cir/(counts*0.000008))); // speed in Km/h
}


ISR(TIMER1_OVF_vect){		
	TCNT1=0;
	rounds++;
}

ISR(TIMER1_CAPT_vect){

	if (PIND & 0x40) {
			falling=ICR1;
			TCCR1B=(1<<CS11|1<<WGM12|0<<ICES1);
			counts=falling+(rounds*65535);
	}else{
		rounds=0;		
		TCNT1=0;
		TCCR1B=(1<<CS11|1<<WGM12|1<<ICES1);
	}

}



