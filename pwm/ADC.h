#ifndef _ADC_ROUTINES_H_
#define _ADC_ROUTINES_H_

uint16_t adc_read(uint8_t);
void adc_init(void);
double get_cs1_voltage(void);
double get_cs2_voltage(void);
double get_supply_voltage(void);
double get_batry_voltage(void);
double get_internal_voltage(void);


#endif