#include <avr/io.h>
#include <util/delay.h>
#include "SYSTEM_functions.h"

void port_init(void)
{
  PORTA = 0x00;
  DDRA  = 0x00;
  PORTB = 0x00; //pull-up on for pushbutton
  DDRB  = 0xBF; //MISO line i/p, rest o/p
  PORTC = 0x80; //pull-up for push-button
  DDRC  = 0x20; //PC7- i/p (pushbutton), PC6, PC5 - o/p (two LEDs)
  DDRD  = 1  << PORTD2;
  PORTD = 0  << PORTD2;
  
}

void blinkRedLED(void)
{
	DDRD  = 1  << PORTD2;
	PORTD = 0  << PORTD2;
  while(1)  //blink red LED continuously, if error
  {
    PORTD ^= 1  << PORTD2;
	_delay_ms(500);
  }

}
 
char* numberToChar(int val){


}
