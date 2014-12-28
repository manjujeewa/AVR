
#ifndef _SYSTEM_FUNCTIONS_
#define _SYSTEM_FUNCTIONS_

#define ERROR {blinkRedLED(0x0d);}

#define RED_LED_ON		PORTD = 1  << PORTD2;
#define RED_LED_OFF		PORTD = 0  << PORTD2;

void blinkRedLED(void);
void port_init(void);
char* numberToChar(int);

#endif
	
