#include "../template.h"
#include "uart.h"

#define UART_BAUD_RATE      9600

int main(void){
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	int c;
	while(1){
        c = uart_getc();
        if (		!(c & UART_NO_DATA)
			&&	!(c & UART_FRAME_ERROR)
			&&	!(c & UART_OVERRUN_ERROR)
			&&	!(c & UART_BUFFER_OVERFLOW)
		   )
		uart_putc( (unsigned char) c);	// c is type casted to get the CHAR
		// it can be used like:
		// unsigned char z = (unsigned char) c;
	};

return(0);
}
