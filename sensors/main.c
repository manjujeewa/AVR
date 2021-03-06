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

#include "twimaster.c"
#include "UART_routines.h"
#include "bmp085.h"
#include "dht11.h"

#define HMC5883L_WRITE 0x3C // write address
#define HMC5883L_READ 0x3D // read address

void init_HMC5883L(void);
void getHeading(void);


double rounds;
double point;
double r = 0.02;

double falling;
double counts;

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

void init_IR(void){
	TCCR1B = (1<<CS11|1<<WGM12|1<<ICES1);
	TIMSK = (1<<TOIE1|1<<TICIE1);
	TCNT1=0;
}

void calculate(void){
	double cir = r*2*22/7;
	point = 3.6*(cir/(counts*0.000008)); // speed in Km/h
}


ISR(TIMER1_OVF_vect){		
	TCNT1=0;
	rounds++;
}

ISR(TIMER1_CAPT_vect){
	if (PIND & 0x40) {
			falling=ICR1;
			TCCR1B=(1<<CS11|1<<WGM12|0<<ICES1);
			counts=falling+(rounds*65535);
	}else{
		rounds=0;
		TCNT1=0;
		TCCR1B=(1<<CS11|1<<WGM12|1<<ICES1);
	}
}

int main(void){	
	//****************************************
		sei();	// interrupt 
		init_IR();
	//****************************************
		long l;
		double d;
		char preasure[10];
		char p_tmp[10];
		
		
		
		char *humidity;
		char *H_temperature;
		char *temperature;
		char buffer[12];
		char flot[8];
		
		
		char direction[8];
		char *lux;
	//****************************************
	
	//****************************************
		USART_Init(BAUD_PRESCALE);	// UART Init
	//****************************************
	
	
	while(1){
	
		//****************************************
			bmp085_init();     //init bmp085 preasure sensor
			
			//d = bmp085_gettemperature();
			//dtostrf(d, 4,1, p_tmp);
			
			l = bmp085_getpressure();
			ltoa(l, preasure, 10);		
			
			
		//****************************************
			humidity = itoa(gethumidity(),10,humidity);
			
			H_temperature = itoa(gettemperature(),10,H_temperature);
			
		//****************************************
			/*calculate();
			dtostrf( rounds ,3,0, buffer);	
			dtostrf( counts ,10,0, buffer);
			dtostrf( point ,6,3, flot);
			
			transmitString(flot); // wind speed
			transmitString(", ");*/
		//****************************************
			i2c_init(); 
			init_HMC5883L();
			
			getHeading();
			float headingDegrees = atan2((double)raw_y,(double)raw_z)* 180 / 3.14159265 + 180;
			
			dtostrf( headingDegrees ,4,1, direction);
			
		//****************************************
		
		transmitString(preasure);
		transmitString(", ");
		transmitString(humidity);
		transmitString(", ");
		transmitString(H_temperature);
		transmitString(", ");
		transmitString(direction);
		transmitString(", ");
		
		TX_NEWLINE;
		
	}

}

