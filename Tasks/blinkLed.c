/*
 * blinkLed.c
 *
 *  Created on: 14/03/2015
 *      Author: Patrizio
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <timer.h>
#include "blinkLed.h"

static TIMER timer_;
static unsigned char estado_;

enum {
	LED_ON,
	LED_OFF
};

void inicializarLed(void) {
	DDRD |= _BV(DDD6);					// set pin 6 of PORTD for output
	timer_start(&timer_);
	estado_ = LED_OFF;
}

void executarTarefaBlinkLed(void) {
	switch (estado_) {
		case LED_ON:
			PORTD &= ~_BV(PORTD6);		// Set pin 6 low to turn led ON.

			if (timer_check(timer_, 850)) {
				estado_ = LED_OFF;
				timer_start(&timer_);
			}
			break;

		case LED_OFF:
			PORTD |= _BV(PORTD6);		// Set pin 6 high to turn led OFF.

			if (timer_check(timer_, 150)) {
				estado_ = LED_ON;
				timer_start(&timer_);
			}
			break;
	}
}
