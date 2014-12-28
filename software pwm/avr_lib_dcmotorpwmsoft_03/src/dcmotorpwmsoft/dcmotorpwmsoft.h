/*
dcmotorpwmsoft lib 0x03

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#ifndef DCMOTORPWMSOFT_H
#define DCMOTORPWMSOFT_H

//number of motor
#define DCMOTORPWMSOFT_MOTORNUM 2

#define DCMOTORPWMSOFT_MOTORNUMMAX 4 //max number of motor
#if DCMOTORPWMSOFT_MOTORNUM > DCMOTORPWMSOFT_MOTORNUMMAX
	#error "max number of motor"
#endif

//setup motor ports
#define DCMOTORPWMSOFT_MOTORSETUP uint8_t m=0;\
    /*motor 1*/\
	m=0;\
	dcmotorpwmsoft_array[m].ddr = &DDRD;\
	dcmotorpwmsoft_array[m].port = &PORTD;\
	dcmotorpwmsoft_array[m].pinA = PD2;\
	dcmotorpwmsoft_array[m].pinB = PD3;\
	/*motor 2*/\
	m=1;\
	dcmotorpwmsoft_array[m].ddr = &DDRD;\
	dcmotorpwmsoft_array[m].port = &PORTD;\
	dcmotorpwmsoft_array[m].pinA = PD4;\
	dcmotorpwmsoft_array[m].pinB = PD5;

//enable or disable slow start functions
#define DCMOTORPWMSOFT_SLOWENABLED 1

//timer setting
//timerfreq = (FCPU / prescaler) / timerscale
//    timerscale 8-bit = 256
//es. 3906 = (1000000 / 1) / 256
//    1 / 3906 = 0.0002502s = 0.250ms = 250us
#define TIMER0_PRESCALER (1<<CS00) //prescaler 1
#define TIMER0_USDELAY 250 //delay us

#define TIMER_INIT TCCR0 = TIMER0_PRESCALER; \
	TIMSK = 1<<TOIE0;
#define TIMER_INTERRUPT ISR(TIMER0_OVF_vect)

#if DCMOTORPWMSOFT_SLOWENABLED == 1
//steps to count for the slow start/stop counter
//set here how much is the slow start/stop
#define DCMOTORPWMSOFT_SLOWSTEPS 200
#endif

//defint minimum and maximum velocity
#define DCMOTORPWMSOFT_VELMAX 100
#define DCMOTORPWMSOFT_VELMIN 1

//define the max value for step, and the frequency for the motor
//DCMOTORPWMSOFT_MAXVAL = (1/freq)/(TIMER0_USDELAY/(1000*1000))
//es. select 10hz as frequency, 400 = (1/10hz) / (250/(1000*1000))
//DCMOTORPWMSOFT_MAXVAL should be > 100 (which is the velocity max value)
#define DCMOTORPWMSOFT_MAXVAL 400
#define DCMOTORPWMSOFT_MINVAL 200
//motor array
typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *port;
	uint8_t pinA;
	uint8_t pinB;
	uint16_t step;
	uint16_t vel;
	uint8_t velminmax;
	#if DCMOTORPWMSOFT_SLOWENABLED == 1
	uint32_t slowstepscounter;
	uint16_t slowvelto;
	uint8_t slowstop;
	#endif
	volatile uint8_t direction;
} dcmotorpwmsoft_arrayt;

//definitions
#define DCMOTORPWMSOFT_DIRECTIONFORWARD 1
#define DCMOTORPWMSOFT_DIRECTIONBACKWARD 2
#define DCMOTORPWMSOFT_DIRECTIONSTOP 0

//functions
extern void dcmotorpwmsoft_init();
extern uint8_t dcmotorpwmsoft_isstop(uint8_t motorid);
extern void dcmotorpwmsoft_gostop(uint8_t motorid);
extern void dcmotorpwmsoft_goforward(uint8_t motorid, uint8_t velocity);
extern void dcmotorpwmsoft_gobackward(uint8_t motorid, uint8_t velocity);
extern uint16_t dcmotorpwmsoft_getspeed(uint8_t motorid);
extern void dcmotorpwmsoft_setspeed(uint8_t motorid, uint8_t velocity);
#if DCMOTORPWMSOFT_SLOWENABLED == 1
extern void dcmotorpwmsoft_gostopslow(uint8_t motorid);
extern void dcmotorpwmsoft_goforwardslow(uint8_t motorid, uint8_t velocityfrom, uint8_t velocityto);
extern void dcmotorpwmsoft_gobackwardslow(uint8_t motorid, uint8_t velocityfrom, uint8_t velocityto);
#endif

#endif
