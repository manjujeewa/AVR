/*
dcmotorpwmsoft lib 0x03

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dcmotorpwmsoft.h"

volatile dcmotorpwmsoft_arrayt dcmotorpwmsoft_array[DCMOTORPWMSOFT_MOTORNUM];

/*
 * init a motor
 */
void dcmotorpwmsoft_init() {
	//setup motors
	DCMOTORPWMSOFT_MOTORSETUP

	//set ports
	uint8_t i = 0;
	for(i=0; i<DCMOTORPWMSOFT_MOTORNUM; i++) {
		*dcmotorpwmsoft_array[i].ddr |= (1 << dcmotorpwmsoft_array[i].pinA); //output
		*dcmotorpwmsoft_array[i].ddr |= (1 << dcmotorpwmsoft_array[i].pinB); //output
		*dcmotorpwmsoft_array[i].port &= ~(1 << dcmotorpwmsoft_array[i].pinA); //off
		*dcmotorpwmsoft_array[i].port &= ~(1 << dcmotorpwmsoft_array[i].pinB); //off
	}

	//init timer
	TIMER_INIT

	sei();
}

//wehave to set this as a definition because a for within the timer overflow interrupt have problems
#if DCMOTORPWMSOFT_SLOWENABLED == 1
#define dcmotorpwmsoft_checkmotorslow(motorid) \
	if(dcmotorpwmsoft_array[motorid].slowvelto != 0 || dcmotorpwmsoft_array[motorid].slowstop == 1) { \
		dcmotorpwmsoft_array[motorid].slowstepscounter++; \
		if(dcmotorpwmsoft_array[motorid].slowstepscounter == DCMOTORPWMSOFT_SLOWSTEPS) { \
			dcmotorpwmsoft_array[motorid].slowstepscounter = 0; \
			if(dcmotorpwmsoft_array[motorid].slowstop == 1) { \
				if(dcmotorpwmsoft_array[motorid].vel > DCMOTORPWMSOFT_MINVAL) \
					dcmotorpwmsoft_array[motorid].vel--; \
				else { \
					dcmotorpwmsoft_array[motorid].direction = DCMOTORPWMSOFT_DIRECTIONSTOP; \
					*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinA); \
							*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinB); \
					dcmotorpwmsoft_array[motorid].slowstop = 0; \
				} \
			} else { \
				if(dcmotorpwmsoft_array[motorid].vel+1 < DCMOTORPWMSOFT_MAXVAL) \
					dcmotorpwmsoft_array[motorid].vel++; \
				if(dcmotorpwmsoft_array[motorid].vel == dcmotorpwmsoft_array[motorid].slowvelto) \
					dcmotorpwmsoft_array[motorid].slowvelto = 0; \
			} \
		} \
	}
#else
#define dcmotorpwmsoft_checkmotorslow(motorid) \
	;
#endif

#define dcmotorpwmsoft_checkmotor(motorid) \
		dcmotorpwmsoft_checkmotorslow(motorid); \
		if(dcmotorpwmsoft_array[motorid].step == dcmotorpwmsoft_array[motorid].vel) { \
			*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinA); \
			*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinB); \
		} else if(dcmotorpwmsoft_array[motorid].step == DCMOTORPWMSOFT_MAXVAL) { \
			dcmotorpwmsoft_array[motorid].step = 0; \
			if(dcmotorpwmsoft_array[motorid].direction == DCMOTORPWMSOFT_DIRECTIONFORWARD) { \
				*dcmotorpwmsoft_array[motorid].port |= (1 << dcmotorpwmsoft_array[motorid].pinA); \
				*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinB); \
			} else if(dcmotorpwmsoft_array[motorid].direction == DCMOTORPWMSOFT_DIRECTIONBACKWARD) { \
				*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinA); \
				*dcmotorpwmsoft_array[motorid].port |= (1 << dcmotorpwmsoft_array[motorid].pinB); \
			} \
		} \
		dcmotorpwmsoft_array[motorid].step++;

/*
 * timer interrupt
 */
TIMER_INTERRUPT {
	uint8_t motorid = 0;

	//motor 1
	#if DCMOTORPWMSOFT_MOTORNUM > 0
	dcmotorpwmsoft_checkmotor(motorid);
	#endif

	//motor 2
	motorid++;
	#if DCMOTORPWMSOFT_MOTORNUM > 1
	dcmotorpwmsoft_checkmotor(motorid);
	#endif

	//motor 3
	motorid++;
	#if DCMOTORPWMSOFT_MOTORNUM > 2
	dcmotorpwmsoft_checkmotor(motorid);
	#endif

	//motor 4
	motorid++;
	#if DCMOTORPWMSOFT_MOTORNUM > 3
	dcmotorpwmsoft_checkmotor(motorid);
	#endif
}

/*
 * is a motor running
 */
uint8_t dcmotorpwmsoft_isstop(uint8_t motorid) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		if(dcmotorpwmsoft_array[motorid].direction == DCMOTORPWMSOFT_DIRECTIONSTOP)
			return 1;
		else
			return 0;
	}
	return 0;
}

/*
 * stop a motor
 */
void dcmotorpwmsoft_gostop(uint8_t motorid) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		dcmotorpwmsoft_array[motorid].direction = DCMOTORPWMSOFT_DIRECTIONSTOP;
		dcmotorpwmsoft_array[motorid].step = 0;
		dcmotorpwmsoft_array[motorid].vel = 0;
		#if DCMOTORPWMSOFT_SLOWENABLED == 1
		dcmotorpwmsoft_array[motorid].slowstepscounter = 0;
		dcmotorpwmsoft_array[motorid].slowvelto = 0;
		dcmotorpwmsoft_array[motorid].slowstop = 0;
		#endif
		*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinA);
		*dcmotorpwmsoft_array[motorid].port &= ~(1 << dcmotorpwmsoft_array[motorid].pinB);
	}
}

/*
 * get a motor velocity
 */
uint16_t dcmotorpwmsoft_getspeed(uint8_t motorid) {
	uint16_t vel = 0;
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		//transform ocr velocity to velocity value
		vel = (long)(DCMOTORPWMSOFT_VELMAX-DCMOTORPWMSOFT_VELMIN) * (dcmotorpwmsoft_array[motorid].vel-DCMOTORPWMSOFT_MINVAL) / (DCMOTORPWMSOFT_MAXVAL-DCMOTORPWMSOFT_MINVAL) + DCMOTORPWMSOFT_VELMIN;
		if(vel > dcmotorpwmsoft_array[motorid].velminmax || vel == (dcmotorpwmsoft_array[motorid].velminmax-1)) //check max and rude ceil
		 	vel = dcmotorpwmsoft_array[motorid].velminmax;
	}
	return vel;
}

/*
 * set a motor velocity
 */
void dcmotorpwmsoft_setspeed(uint8_t motorid, uint8_t velocity) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		uint16_t vel =  0;
		//limit velocity
		if(velocity > DCMOTORPWMSOFT_VELMAX)
			velocity = DCMOTORPWMSOFT_VELMIN;
		else if (velocity < DCMOTORPWMSOFT_VELMIN)
			velocity = DCMOTORPWMSOFT_VELMIN;
		//transform velocity to orc value
		vel = (long)(DCMOTORPWMSOFT_MAXVAL-DCMOTORPWMSOFT_MINVAL) * (velocity-DCMOTORPWMSOFT_VELMIN) / (DCMOTORPWMSOFT_VELMAX-DCMOTORPWMSOFT_VELMIN) + DCMOTORPWMSOFT_MINVAL;
		if(vel > DCMOTORPWMSOFT_MAXVAL || vel == (DCMOTORPWMSOFT_MAXVAL-1)) //check max and rude ceil
			vel = DCMOTORPWMSOFT_MAXVAL;
		dcmotorpwmsoft_array[motorid].vel = vel;
	}
}

/*
 * set the motor go
 */
void dcmotorpwmsoft_goset(uint8_t motorid, uint8_t direction, uint8_t velocity) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		dcmotorpwmsoft_array[motorid].direction = direction;
		dcmotorpwmsoft_array[motorid].step = 0;
		#if DCMOTORPWMSOFT_SLOWENABLED == 1
		dcmotorpwmsoft_array[motorid].slowstepscounter = 0;
		dcmotorpwmsoft_array[motorid].slowvelto = 0;
		dcmotorpwmsoft_array[motorid].slowstop = 0;
		#endif
		dcmotorpwmsoft_array[motorid].vel = velocity;
		dcmotorpwmsoft_setspeed(motorid, velocity);
	}
}

/*
 * let a motor go forward
 */
void dcmotorpwmsoft_goforward(uint8_t motorid, uint8_t velocity) {
	dcmotorpwmsoft_goset(motorid, DCMOTORPWMSOFT_DIRECTIONFORWARD, velocity);
}

/*
 * let a motor go backward
 */
void dcmotorpwmsoft_gobackward(uint8_t motorid, uint8_t velocity) {
	dcmotorpwmsoft_goset(motorid, DCMOTORPWMSOFT_DIRECTIONBACKWARD, velocity);
}


#if DCMOTORPWMSOFT_SLOWENABLED == 1
/*
 * setup the slow startup, for startup speed setup the step multiplier from 1 to 5
 */
void dcmotorpwmsoft_gosetslow(uint8_t motorid, uint8_t direction, uint8_t velocityfrom, uint8_t velocityto) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM && velocityfrom < velocityto) {
		dcmotorpwmsoft_array[motorid].direction = direction;
		dcmotorpwmsoft_array[motorid].step = 0;
		dcmotorpwmsoft_array[motorid].slowstepscounter = 0;
		if(velocityto > DCMOTORPWMSOFT_VELMAX)
			velocityto = DCMOTORPWMSOFT_VELMIN;
		else if (velocityto < DCMOTORPWMSOFT_VELMIN)
			velocityto = DCMOTORPWMSOFT_VELMIN;
		uint16_t velto = (long)(DCMOTORPWMSOFT_MAXVAL-DCMOTORPWMSOFT_MINVAL) * (velocityto-DCMOTORPWMSOFT_VELMIN) / (DCMOTORPWMSOFT_VELMAX-DCMOTORPWMSOFT_VELMIN) + DCMOTORPWMSOFT_MINVAL;
		if(velto > DCMOTORPWMSOFT_MAXVAL || velto == (DCMOTORPWMSOFT_MAXVAL-1)) //check max and rude ceil
			velto = DCMOTORPWMSOFT_MAXVAL;
		dcmotorpwmsoft_array[motorid].slowvelto = velto;
		dcmotorpwmsoft_array[motorid].slowstop = 0;
		dcmotorpwmsoft_array[motorid].velminmax = velocityto;
		dcmotorpwmsoft_setspeed(motorid, velocityfrom);
	}
}

/*
 * stop a motor, slowly
 */
void dcmotorpwmsoft_gostopslow(uint8_t motorid) {
	if(motorid >= 0 && motorid < DCMOTORPWMSOFT_MOTORNUM) {
		dcmotorpwmsoft_array[motorid].slowstepscounter = 0;
		dcmotorpwmsoft_array[motorid].slowstop = 1;
	}
}

/*
 * let a motor go forward with slow startup
 */
void dcmotorpwmsoft_goforwardslow(uint8_t motorid, uint8_t velocityfrom, uint8_t velocityto) {
	dcmotorpwmsoft_gosetslow(motorid, DCMOTORPWMSOFT_DIRECTIONFORWARD, velocityfrom, velocityto);
}

/*
 * let a motor go forward with slow startup
 */
void dcmotorpwmsoft_gobackwardslow(uint8_t motorid, uint8_t velocityfrom, uint8_t velocityto) {
	dcmotorpwmsoft_gosetslow(motorid, DCMOTORPWMSOFT_DIRECTIONBACKWARD, velocityfrom, velocityto);
}
#endif
