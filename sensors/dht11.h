/*
DHT11 Library
Author:M.P Sameera Vitaj
University of Colombo
*/

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef _DHT11_H_INCLUDED
#define _DHT11_H_INCLUDED

#define DHT11_DDR DDRD
#define DHT11_PORT PORTD
#define DHT11_PIN PIND
#define DHT11_INPUTPIN PD5
#define DHT11_ERROR 255

void reset(void);
void request(void);
unsigned int getdata(unsigned int select);
unsigned int gettemperature(void);
unsigned int gethumidity(void);








#endif // _DHT11_H_
