#include <stdio.h>
#include <avr/io.h>

#include <util/delay.h>

#include "dcmotorpwmsoft/dcmotorpwmsoft.h"


int main(void) {

	//init
	dcmotorpwmsoft_init();

	uint8_t motorssetdir[DCMOTORPWMSOFT_MOTORNUM];
	uint8_t motorsid[DCMOTORPWMSOFT_MOTORNUM] = {0, 1};
	uint8_t i = 0;

	dcmotorpwmsoft_goforwardslow(1, 1, 100);

	for(;;) {
		PORTB ^= (1<<PB1);


		#if DCMOTORPWMSOFT_SLOWENABLED == 1
		for(i=0; i<DCMOTORPWMSOFT_MOTORNUM; i++) {
			if(motorssetdir[i] == 0) {
				dcmotorpwmsoft_goforwardslow(motorsid[i], 1, 100);
			} else {
				dcmotorpwmsoft_gobackwardslow(motorsid[i], 1, 100);
			}
			motorssetdir[i]++;
			motorssetdir[i]%=2;
		}
		_delay_ms(1000);
		_delay_ms(1000);
		for(i=0; i<DCMOTORPWMSOFT_MOTORNUM; i++) {
			dcmotorpwmsoft_gostopslow(motorsid[i]);
		}
		_delay_ms(1000);
		_delay_ms(1000);
		#else
		for(i=0; i<DCMOTORPWMSOFT_MOTORNUM; i++) {
			if(motorssetdir[i] == 0) {
				dcmotorpwmsoft_goforward(motorsid[i], 70);
			} else {
				dcmotorpwmsoft_gobackward(motorsid[i], 70);
			}
			motorssetdir[i]++;
			motorssetdir[i]%=2;
		}
		_delay_ms(1000);
		for(i=0; i<DCMOTORPWMSOFT_MOTORNUM; i++) {
			dcmotorpwmsoft_gostop(motorsid[i]);
		}
		_delay_ms(1000);
		#endif
	}
}
