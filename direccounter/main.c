#define F_CPU 8000000UL


#include <avr/io.h>
#include <util/delay.h>

void getdigi(unsigned int);
void displaySSD(unsigned int n);
void init(void);

unsigned char SSD[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char dig[] = {1,2,3,4};

int main(void){
		
	init();
	unsigned int count =0;
	getDigi(count);
	char state;

		
	while(1){
	if((++count)==10000) count =0;
	/*
	
		if (state){
			if((++count)==10000) count =0;
			
			
		}else{
		
			if((count--)==1) count =0;
			
			
		}
		
		if(PIND & (1<<2)){
				state = true;
		}*/
			
		getDigi(count);		
		displaySSD(100);
		
		
		
	}
		
	return 0;


}






void init(void){
	DDRC = 0xff;
	DDRA = 1<<3 | 1<<4| 1<<5| 1<<6 | 1<<7;	
	DDRD = 0;

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
		
			PORTC=SSD[dig[i]];
			PORTA = 1<<i+4;
			_delay_ms(1);
			PORTA=0;
		}
	}
}