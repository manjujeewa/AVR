#define F_CPU 16000000UL  // 8 MHz
/************************************************************************
	A program for reading the data from USART and then echo the data back
*************************************************************************/

#include <avr/io.h>

/************************************************************************
	Initialize the USART with the Baudrate that is passes as an argument
	Enable both the transmitter and receiver of USART
*************************************************************************/
void _init_usart( unsigned int Baudrate )
{
  UBRRH=0x00;
  UBRRL = (unsigned char)Baudrate; // Set the baud rate
  UCSRB = 0x18; // enable transmitter and reciever
  UCSRC = 0x86; // set as 8 bit data, no parity bit and 1 stop bit.
}

/************************************************************************
	Receive the byte that arrives on the receiver
*************************************************************************/
unsigned char usart_receive( void )
{
  unsigned char data;
  while ((UCSRA & 0x80) == 0x00); // Wait for till the data arrives data
  data=UDR;	// Read the data from UDR, as soon as the data arrives
  return data;
}

/************************************************************************
	Transmit the data that is passed as argument
*************************************************************************/
void usart_transmit( unsigned char data )
{
  while ((UCSRA & 0x20) == 0x00);  // Wait until the transmit buffer is empty
  UDR = data; // As soon as the buffer is empty, put the data in UDR
}

int main(void)
{
	unsigned char a;
	DDRD=0X0F; 

	_init_usart(6); // For 9600 bps, value of UBBR = 6

	while(1)
	{
     	//a = usart_receive(); // recieve data from the usart receiver
     	usart_transmit(55); // echo the data back
	}
}