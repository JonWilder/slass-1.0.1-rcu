/* SLASS RCU PWM Driver
 * searchlight-1.0.1-slass
 * Author: Jon Wilder
 * Project Manager: Kevin Johnson - APR Aviation
 * Created Date: February 26th, 2020
 * Use rights retained by Fresno Police Department and APR Aviation
 * (C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "joystick.h"
#include "pwm.h"

#define _PWM_DUTY_CYCLE_LIMIT 73	// duty cycle limiting value

uint8_t ledblink = 32;	// LED blink counter

void setDutyCycle(uint8_t module,uint8_t duty) {
	/* Set PWM duty cycle */
	if(duty > _PWM_DUTY_CYCLE_LIMIT) {
		/* Limit duty cycle to 39% */
		duty = _PWM_DUTY_CYCLE_LIMIT;
	}
	switch(module) {
		case 'Y':
			/* update Y axis duty cycle value */
			OCR1A = duty;	
			break;
		case 'X':
			/* update X axis duty cycle value */
			OCR1B = duty;	
			break;
	}
}

void initPwm(void) {
	/* OC1A, OC1B, PB2, and PB6 non-inverting fast PWM */
	OCR1A = 0;		// OC1A 0% duty cycle
	OCR1B = 0;		// OC1B 0% duty cycle
	OCR1C = 191;	// set PWM period 49.2mS - 20.3Hz PWM frequency
	PLLCSR |= _BV(PLLE);	// enable fast PLL clock
	while(bit_is_clear(PLLCSR,PLOCK));	// wait for PLL to stablize
	PLLCSR |= _BV(PCKE);	// fast PLL is timer 1 clock source
	TCCR1A = 0b11;			// OC1A and OC1B non-inverting fast PWM
	TCCR1B = 0b1111;		// 1:16384 timer prescaler
	PORTB &= ~0b1001110;	// PB1, PB2, PB3, and PB6 low
	DDRB |= 0b1001110;		// enable OC1A and OC1B output drivers
}

/* Up and Left Signal Synthesis
 * 
 * See README for more information
 */

ISR(TIMER1_CMPA_vect) {
	/* Set up pin low on timer 1 OC1A compare match */
	PORTB &= ~_BV(UP);
}

ISR(TIMER1_CMPB_vect) {
	/* Set left pin low on OC1B compare match */
	PORTB &= ~_BV(LEFT);
}

ISR(TIMER1_OVF1_vect) {
	/* Set up/left pins high on timer 1 overflow */
	if((position[_Y_AXIS] > null[_Y_NULL_TOP]) && (position[_X_AXIS] < null[_X_NULL_BOT])) {
		/* Up & Left Movement */
		PORTB |= 0b1000100;		// OC1A output on PB6, OB1B output on PB2
	}else {
		if(position[_Y_AXIS] > null[_Y_NULL_TOP]) {
			/* Up Movement */
			PORTB |= _BV(UP);		// OC1A output on PB6, set high
		}
		if(position[_X_AXIS] < null[_X_NULL_BOT]) {
		/* Left Movement */
			PORTB |= _BV(LEFT);		// OC1B output on PB2, set high
		}
	}
	/* On board LED blink */
	--ledblink;	// decrement led blink counter
	PORTB |= _BV(LED);	// LED off
	if(ledblink == 0) {
		/* On blink counter expire */
		PORTB &= ~_BV(LED);	// LED on
		ledblink = 32;	// reset blink counter
	}
}

/* End of file */