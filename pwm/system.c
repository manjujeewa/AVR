#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "system.h"
#include "ADC.h"
#include "lcd.h"
#include "acs712.h"

double volt_now, cS1_voltage, cS2_voltage, out_current;

double  out_power;

double battery_type = 0.0;


void pwm_init(void){
	// initialize PWM
	
    // initialize timer0 in PWM mode
    TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(0<<CS00)|(1<<CS01)|(0<<CS02);
	// initialize timer2 in PWM mode
	//TCCR2 |= (1<<WGM20)|(1<<COM21)|(1<<WGM21)|(0<<CS20)|(1<<CS21)|(0<<CS22);
	
	OCR0=254;
	//OCR2=0;
    // set OC0 pin (pin PB3) as output pin
    DDRB |= (1<<PB3);
	// set OC2 pin (pin PD7) as output pin
	//DDRD |= (1<<PD7);
}
  
void system_init(void){
	DDRD = 1<<PORTD5 | 1<<PORTD4;
	PORTD = (0<<PORTD5) | (1<<PORTD4);
	DDRA = 1<<PORTA0;
	PORTA = 1<<PORTA0;
	DDRB = 1<<PORTB1;
	PORTB = 1<<PORTB1;
	
	// enableing external interrupts
	MCUCR = 1<<ISC01 | 1<<ISC00 | 1<<ISC11 | 1<<ISC10;
	GICR = 1<<INT0 | 1<<INT1;
}

//led indicators -----------------------------

void green_on(void){
	PORTA = 1<<PORTA0;
}
void green_off(void){
	PORTA = 0<<PORTA0;
}

void red_on(void){
	PORTB = 1<<PORTB1;
}
void red_off(void){
	PORTB = 0<<PORTB1;
}

// end of led indicators ---------------------


void sys_battery_on(void){
	PORTD |= (0<<PORTD5);
}

void sys_battery_off(void){
	PORTD |= (1<<PORTD5);
}


void external_battery_on(void){
	PORTD |= (1<<PORTD5);
}

void external_battery_off(void){
	PORTD |= ~(1<<PORTD5);
}

// increase dusty cycle
/*
void increase_duty(int ch){
	if(ch==0){ 
		if(OCR0>0 && OCR0<255){
			OCR0++;
		}else{
			OCR0=255;
		}
	}
	
	if(ch==2){ 
		if(OCR2>0 && OCR2<255){
			OCR2++;
		}else{
			OCR2=255;
		}
	}
		
}

void decrease_duty(int ch){
	if(ch==0){ 
		if(OCR0>0 && OCR0<255){
			OCR0--;
		}else{
			OCR0=0;
		}
	}
	
	if(ch==2){ 
		if(OCR2>0 && OCR2<255){
			OCR2--;
		}else{
			OCR2=0;
		}
	}
		
}
*/
void parasite_mode_on(void){


}

void regulate_output(double set_volt){

	volt_now = get_batry_voltage();
	OCR0 =255;
	
	while(1){
		if(volt_now>set_volt){
			OCR0++;
			 volt_now = get_batry_voltage();
			//display_Output_voltage();
		}
		if(volt_now<set_volt){
			OCR0--;
			 volt_now = get_batry_voltage();
			//display_Output_voltage();
		}
		
		//if (abs(set_volt-volt_now) < 0.02) break;
		
		_delay_ms(100);
		
	}
}

void MPPT_out(void){
	double tmp_power;
	OCR0 =250;
	out_power = 0;
	tmp_power = get_output_power();
	
		while(1){
			while((tmp_power-out_power)>0){
				out_power = get_output_power();
				OCR0--;
				tmp_power = get_output_power();
			}
			while((tmp_power-out_power)<0){
				out_power = get_output_power();
				OCR0++;
				tmp_power = get_output_power();
			}
			
			/*if (get_batry_voltage() < battery_type){
				lcd_clrscr();
				lcd_puts("ERROR: Low Power");
				_delay_ms(2000);
				external_battery_off();
				break;
			}*/
			
			if (get_output_current() < 0.5){
				lcd_clrscr();
				lcd_puts("charge complete");
				_delay_ms(2000);
				external_battery_off();
				break;
			}
		}

}

double get_output_current(void){

	cS1_voltage = get_cs1_voltage();
	out_current = acs712_getcurrent(cS1_voltage, 5.0);
	
	if (out_current<0) out_current = -out_current;
	return (double)(out_current);
}

void detect_battery(void){
	external_battery_off();
	volt_now = get_batry_voltage();
	
	if(volt_now>10.5 && volt_now<12.5) {
		battery_type = 12.0;
		lcd_clrscr();
		lcd_puts("12V batery found");
		lcd_gotoxy(0,1);
		lcd_puts("Charging Started");
	}
	
	if(volt_now>8.0 && volt_now<9.5){ 
		battery_type = 9.0;
		lcd_clrscr();
		lcd_puts("9V batery found");
		lcd_gotoxy(0,1);
		lcd_puts("Charging Started");
	}
	
	if(volt_now>4.5 && volt_now<6.5){ 
		battery_type = 6.0;
		lcd_clrscr();
		lcd_puts("6V batery found");
		lcd_gotoxy(0,1);
		lcd_puts("Charging Started");
	}
	external_battery_on();
	_delay_ms(2000);
}


double get_output_power(void){    //**************************************************************

	volt_now = get_batry_voltage();
	cS1_voltage = get_cs1_voltage();
	out_current = acs712_getcurrent(cS1_voltage, 5.0);
	if (out_current<0) out_current = -out_current;
	
	return (double)(volt_now*out_current);
}


double get_input_power(void){

	double volt_in = get_supply_voltage();
	cS2_voltage = get_cs2_voltage();
	double current_in = acs712_getcurrent(cS2_voltage, 5.0);
	if (current_in < 0)  current_in = -current_in;
		
	return (double)(volt_in*current_in);

}

/*
void regulate_internal_voltage(double set_volt){

	volt_now = get_batry_voltage();
	
	while(1){
		if(volt_now>set_volt){
			OCR0++;
			 volt_now = get_batry_voltage();
			display_Output_voltage();
		}
		if(volt_now<set_volt){
			OCR0--;
			 volt_now = get_batry_voltage();
			display_Output_voltage();
		}
		
		if (abs(set_volt-volt_now) < 0.02) break;
		
		_delay_ms(100);
		
	}
}*/

void display_Output_voltage(void){
	lcd_clrscr();
	char printbuff[10];
	double c1 = get_batry_voltage();
	dtostrf(c1, 4, 2, printbuff);
	lcd_puts(printbuff);
}

void display_double(double d){
	char printbuff[10];
	dtostrf(d, 4, 2, printbuff);
	lcd_puts(printbuff);
}
void display_int(double d){
	char printbuff[10];
	itoa(d, printbuff, 10);
	lcd_puts(printbuff);
}