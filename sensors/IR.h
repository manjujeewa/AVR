/*
DHT11 Library
Author:M.P Sameera Vitaj
University of Colombo
*/

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef _IR_H_
#define _IR_H_


double calculate(void);
void init_IR(void);
ISR(TIMER1_OVF_vect);
ISR(TIMER1_COMPA_vect);








#endif // _IR_H_
