
#define F_CPU 8000000UL


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define F_PWM 40
#define PWM_RES 256

#define OCR1A_val (F_CPU / (F_PWM * PWM_RES))


volatile uint8_t PWM_setting[8];

int main(void){
	
	TCCR1B = 1; // CTC mode prescaler 1
	
	TIMSK = (1<<OCIE1A); // enable timer 1 compare match interrupt
	
	DDRA = 0x00;
	


	
	sei(); // globally enable interrupts
	
	

	while(1){
		
		
		for(uint8_t i = 0; i < 8; i++){
			PWM_setting[i] = (F_CPU / (F_PWM * PWM_RES)) / (rand() % 25 + 1); // 50% duty cycle
			
		}
		
		if(PINA & (1<<PA0)){
			DDRA = 1<<PA0;
			PORTA = 1<<PA0;
		}
		
			
	}
	
	return 0;
}

ISR(TIMER1_COMPA_vect){

	static uint8_t overflows;
	
	uint8_t tmp = 0; // tmp variable to save pin status
	
	OCR1A += OCR1A_val;
	
		
	if(PWM_setting[0] > overflows) tmp |= (1<<0);	
	if(PWM_setting[1] > overflows) tmp |= (1<<1);
	if(PWM_setting[2] > overflows) tmp |= (1<<2);
	if(PWM_setting[3] > overflows) tmp |= (1<<3);
	if(PWM_setting[4] > overflows) tmp |= (1<<4);
	if(PWM_setting[5] > overflows) tmp |= (1<<5);
	if(PWM_setting[6] > overflows) tmp |= (1<<6);
	if(PWM_setting[7] > overflows) tmp |= (1<<7);
	
	
	
		
	//PORTA = tmp; // only one call of PORTD reduces execution time
	//PORTC = tmp;
	
	// check if the timer overflows have exceeded the needed value
	if(overflows > (PWM_RES-1)){
		overflows = 0;
	}
	else{
		overflows++;
	}
}