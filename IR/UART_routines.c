#include <avr/io.h>
#include <avr/pgmspace.h>

#include "UART_routines.h"
void USART_Init(int baud){
   	// Set baud rate
   	UBRRL = baud;// Load lower 8-bits into the low byte of the UBRR register
   	UBRRH = (baud >> 8); 
	/* Load upper 8-bits into the high byte of the UBRR register Default frame format is 8 data bits, no parity, 1 stop bit to change use UCSRC, see AVR datasheet*/ 
	UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));	// Enable receiver and transmitter and receive complete interrupt 
}


void transmitByte(unsigned char data){
	while((UCSRA &(1<<UDRE)) == 0);  // Wait until last byte has been transmitted
	UDR = data; // Transmit data
}

unsigned char receiveByte(){
	while((UCSRA &(1<<RXC)) == 0);
	return UDR;
}

void transmitString_F(char* string){
  	while (pgm_read_byte(&(*string))) transmitByte(pgm_read_byte(&(*string++)));
}

void transmitString(unsigned char* string){
	while (*string) transmitByte(*string++);
}

void transmitHex( unsigned char dataType, unsigned long data ){
	unsigned char count, i, temp;
	unsigned char dataString[] = "0x        ";

	if (dataType == CHAR) count = 2;
	if (dataType == INT) count = 4;
	if (dataType == LONG) count = 8;

	for(i=count; i>0; i--){
		  temp = data % 16;
		  if((temp>=0) && (temp<10)) dataString [i+1] = temp + 0x30;
		  else dataString [i+1] = (temp - 10) + 0x41;
		  data = data/16;
	}
	transmitString (dataString);
}
