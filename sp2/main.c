#include <stdio.h>
#include <avr/io.h>

#include <util/delay.h>

#include "dcmotorpwmsoft/dcmotorpwmsoft.h"


int main(void) {

	//init
	dcmotorpwmsoft_init();

	uint8_t motor0 = 0;
	uint8_t motor1 = 0;
	for(;;) {
		PORTB ^= (1<<PB1);

		if(motor0 == 0)
			dcmotorpwmsoft_gobackward(0, 0);
		else
			dcmotorpwmsoft_goforward(0, 100);
		motor0++;
		motor0%=2;

		if(motor1 == 0)
			dcmotorpwmsoft_gobackward(1, 100);
		else
			dcmotorpwmsoft_goforward(1, 100);
		motor1++;
		motor1%=2;

		_delay_ms(1000);
	}
}
