#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600   // Define baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>

#include "UART_routines.h"
#include "twimaster.c"

#define HMC5883L_WRITE 0x3C // write address
#define HMC5883L_READ 0x3D // read address

int16_t raw_x = 0;
int16_t raw_y = 0;
int16_t raw_z = 0;

void init_HMC5883L(void){
	i2c_start(HMC5883L_WRITE);
	i2c_write(0x00); // set pointer to CRA
	i2c_write(0x70); // write 0x70 to CRA
	i2c_stop();
	i2c_start(HMC5883L_WRITE);
	i2c_write(0x01); // set pointer to CRB
	i2c_write(0xA0);
	i2c_stop();
	i2c_start(HMC5883L_WRITE);
	i2c_write(0x02); // set pointer to measurement mode
	i2c_write(0x00); // continous measurement
	i2c_stop();
}

void getHeading(void){
	i2c_start_wait(HMC5883L_WRITE);
	i2c_write(0x03); //set pointer to X-axis MSB
	i2c_stop();
	i2c_rep_start(HMC5883L_READ);
	raw_x = ((uint8_t)i2c_readAck())<<8;
	raw_x |= i2c_readAck();
	raw_z = ((uint8_t)i2c_readAck())<<8;
	raw_z |= i2c_readAck();
	raw_y = ((uint8_t)i2c_readAck())<<8;
	raw_y |= i2c_readNak();
	i2c_stop();
}

int main(void)
{	
	char buffer[8];

	i2c_init();
	init_HMC5883L();

	USART_Init(BAUD_PRESCALE);  // Initialise USART
	
	

    while(1) {  
		getHeading();
		//float headingDegrees = atan2((double)raw_y,(double)raw_z)* 180 / 3.14159265 + 180;
		
		//dtostrf( headingDegrees ,4,1, buffer);
		

		
		transmitString("A");
		transmitString("\r\n");
		
	
	_delay_ms(500);              
    }
}



























