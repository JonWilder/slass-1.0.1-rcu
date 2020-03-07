/* SLASS RCU Joystick Driver
 * searchlight-1.0.1-slass
 * Author: Jon Wilder
 * Project Manager: Kevin Johnson - APR Aviation
 * Created Date: March 3rd, 2020
 * Use rights retained by Fresno Police Department and APR Aviation
 * (C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.
 */ 

#include <avr/io.h>
#include "main.h"
#include <util/delay.h>
#include "adc.h"
#include "joystick.h"

uint16_t position[2];	// joystick position data array
uint16_t null[4] = {0};	// joystick null position data array

void getJoystick(void) {
	/* Get joystick position */
	uint16_t positionbuffer[2] = {0};
	for(uint8_t i = 10; i != 0; --i) {
		/* Sample joystick position 10 times, rate 1 sample per mS, then calculate average */
		positionbuffer[_X_AXIS] += getAdc(_X_AXIS);	// get X axis position, then add to previous samples
		positionbuffer[_Y_AXIS] += getAdc(_Y_AXIS);	// get Y axis position, then add to previous samples
		_delay_ms(1);	// 1mS wait
	}
	position[_X_AXIS] = positionbuffer[_X_AXIS] / 10;	// store X axis position data
	position[_Y_AXIS] = positionbuffer[_Y_AXIS] / 10;	// store Y axis position data
}

void setNull(void) {
	/* Get joystick null position */
	initAdc();	// initialize analog to digital converter
	_delay_ms(50);	// wait 50mS
	getJoystick();	// get joystick position
	null[_X_NULL_TOP] = 10 + position[_X_AXIS];		// store top of X axis null range
	null[_X_NULL_BOT] = null[_X_NULL_TOP] - 20;		// store bottom of X axis null range
	null[_Y_NULL_TOP] = 10 + position[_Y_AXIS];		// store top of Y axis null range
	null[_Y_NULL_BOT] = null[_Y_NULL_TOP] - 20;		// store bottom of Y axis null range
}

/* End of file */