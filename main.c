/* Universal Searchlights SLASS Controller
 * searchlight-1.0.1-slass
 * Author: Jon Wilder
 * Project Manager: Kevin Johnson - APR Aviation
 * Created Date: February 24th, 2020
 * Use rights retained by Fresno Police Department and APR Aviation
 * (C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.
 */ 

#include <avr/io.h>			// avr-libc io handler
#include <avr/interrupt.h>	// avr-libc interrupt handler
#include "main.h"			// main header
#include <util/delay.h>		// avr-libc delay handler
#include "pwm.h"			// slass PWM driver
#include "joystick.h"		// slass joystick driver

FUSES = {
	/* Fuse High Byte
	 * Default Fuse Settings
	 * Brown out detection enabled
	 * Brown out detection voltage = 4.0V
	 */
	.high = (HFUSE_DEFAULT & FUSE_BODEN & FUSE_BODLEVEL),
	/* Fuse Low Byte
	 * Default Fuse Settings
	 * Oscillator Type: Ext Clock 6CK Startup
	 */
	.low = (LFUSE_DEFAULT & FUSE_SUT1 & FUSE_CKSEL0),
};

LOCKBITS = (LB_MODE_3);	// further programming and verification disabled

int main(void) {
	/* Device Initialization */
	PORTA = 0;	// PA0-PA7 low
	PORTB = 0;	// PB0-PB7 low
	DDRA = 0;	// tristate PORTA
	DDRB = 0b1101110;	// enable PB1,PB2,PB3,PB5, and PB6 output driver
	MCUCR &= ~_BV(PUD);	// disable all internal pull ups
	setNull();	// find and set joystick null position
	initPwm();	// initialize PWM module - 20Hz PWM frequency
	TIMSK |= 0b1100100;	// enable timer 1 overflow, OC1A match, and OC1B match interrupts
	sei();	// enable interrupts globally
	/* Main function begin */
	while(1) {
		/* Loop forever - sample joystick X and Y positions, then update PWM duty cycle based on joystick position */
		getJoystick();	// get joystick positions
		/* Process X Axis */
		if(position[_X_AXIS] > null[_X_NULL_TOP]) {
			/* On right movement */
			TCCR1A |= 0b100000;		// OC1B output on right pin
			setDutyCycle('X',(uint8_t)position[_X_AXIS] - null[_X_NULL_TOP]);	// set right movement %
		}else if(position[_X_AXIS] < null[_X_NULL_BOT]) {			
			/* On left movement */
			TCCR1A &= ~0b110000;	// disconnect OC1B output from right pin
			setDutyCycle('X',(uint8_t)null[_X_NULL_BOT] - position[_X_AXIS]);	// set left movement %
		}else {
			/* Null Zone */	
			setDutyCycle('X',0);	// X axis right movement duty cycle 0%
			TCCR1A &= ~0b110000;	// OC1B off
		}
		/* Process Y Axis */
		if(position[_Y_AXIS] > null[_Y_NULL_TOP]) {
			/* On up movement */
			TCCR1A &= ~0b11000000;	// disconnect OC1A output from down pin
			setDutyCycle('Y',(uint8_t)position[_Y_AXIS] - null[_Y_NULL_TOP]);	// set up movement %
		}else if(position[_Y_AXIS] < null[_Y_NULL_BOT]) {
			/* On down movement */
			TCCR1A |= 0b10000000;	// OC1A output on down pin
			setDutyCycle('Y',(uint8_t)null[_Y_NULL_BOT] - position[_Y_AXIS]);	// set down movement %
		}else {
			/* Null Zone */
			setDutyCycle('Y',0);	// Y axis right movement duty cycle 0%
			TCCR1A &= ~0b11000000;	// OC1A off
		}
	}
	return 0;
}

/* End of file */