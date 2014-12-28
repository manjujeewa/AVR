
#define F_CPU 8000000UL


#include <avr/io.h>
#include <util/delay.h>


int main(void){

	
	DDRA = 0;
	DDRC = 0;
	
    while(1)
    {
		/*
        if(!(bit_is_clear(PINA,0))){
			DDRA = (1 << PINA0);
			PORTA = 1 << PINA0;
		}*/
		
				
		for(unsigned int i=0; i<8; i++){
		
			if(!(bit_is_clear(PINA,i))){
				DDRA |= (1 << i);
				PORTA |= 1 << i;
			}
		} 
		
		/*
		if( (PINA & (1 << PINA0)) && !(DDRA & (1 << PINA0))){
			DDRA |= (1 << PINA0);
			PORTA |= 1 << PINA0;
		}
		
		
		if(!(bit_is_clear(PINA,1)) && !(DDRA & (1 << PINA1))){
			DDRA |= (1 << PINA1);
			PORTA |= 1 << PINA1;
		}
		*/
    }
	
	return 0;
}
