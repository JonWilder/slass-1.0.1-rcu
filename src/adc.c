/* ATtiny26 Analog to Digital Converter Driver
 * Author: Jon Wilder
 * Created Date: February 25th, 2020
 * (C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.
 */ 

#include <avr/io.h>		// avr-libc AVR header file
#include "main.h"		// main header file
#include <util/delay.h>	// avr-libc delay handler
#include "adc.h"		// adc driver header file

void initAdc(void) {
	ADMUX = 0b0;			// Aref = AVCC, right justified result
	ADCSR = 0b10000111;		// enable ADC
}

uint16_t getAdc(uint8_t channel) {
	ADMUX &= ~0b11111;	// reset channel selection
	ADMUX |= channel;	// set channel
	ADCSR |= _BV(ADSC);	// start conversion
	while(bit_is_set(ADCSR,ADSC));	// wait for conversion to complete
	ADCSR |= _BV(ADIF);	// clear ADC interrupt flag
	return ADC;			// return ADC result
}

/* End of file */