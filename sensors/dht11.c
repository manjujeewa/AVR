#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "dht11.h"

// define your data pin right now its given to portc 5
// make sure the clock is at 8MHz
//gethumidity() will return the humidity

unsigned int bits[5];
volatile unsigned int temperature = 0,humidity = 0,i,j,ret,result;

void reset(void){
	//reset port
	DHT11_DDR |= (1<<DHT11_INPUTPIN); //output
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_ms(100);
}

void request(void){
//send request
	DHT11_PORT &= ~(1<<DHT11_INPUTPIN); //low
	_delay_ms(18);
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_us(1);
	DHT11_DDR &= ~(1<<DHT11_INPUTPIN); //input
	_delay_us(39);
}

//get data from DHL11
unsigned int getdata(unsigned int select) {
	//memset(bits, 0, sizeof(bits));
	reset();
	request();
	//check start condition 1
	if((DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);
	//check start condition 2
	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);

	//read the data
	for (j=0; j<5; j++) { //read 5 byte
		result=0;
		for(i=0; i<8; i++) {//read every bit
			while(!(DHT11_PIN & (1<<DHT11_INPUTPIN))); //wait for an high input
			_delay_us(30);
			if(DHT11_PIN & (1<<DHT11_INPUTPIN)) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			while(DHT11_PIN & (1<<DHT11_INPUTPIN)); //wait until input get low
		}
		bits[j] = result;
	}
	reset();

	//check checksum
	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select == 0) { //return temperature
			return(bits[2]);
		} else if(select == 1){ //return humidity
			return(bits[0]);
		}
	}

	return DHT11_ERROR;
}

/*
 * get temperature (0..50C)
 */
unsigned int gettemperature() {
	ret = getdata(0);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}

/*
 * get humidity (20..90%)
 */
unsigned int gethumidity() {
	ret = getdata(1);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}
