#ifndef MPPT_SYSTEM_H_
#define MPPT_SYSTEM_H_

#define BATTERY_ON {sys_battery_on();}
#define BATTERY_OFF {sys_battery_off();}


void pwm_init(void);
void system_init(void);
void sys_battery_on(void);
void sys_battery_off(void);
void external_battery_on(void);
void external_battery_off(void);

void increase_duty(int);
void decrease_duty(int);
void display_Output_voltage(void);

double get_output_power(void);
double get_input_power(void);
double get_output_current(void);

void MPPT_out(void);

void regulate_output(double);

void display_int(double);
void display_double(double);

#endif