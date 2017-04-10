/*
 * buttonLed.c
 *
 *  Created on: 7 de abr de 2017
 *      Author: patrizio
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include "buttonLed.h"

#define BUTTON_D2_PRESSED() !(PIND & _BV(PORTD2))

static TIMER timer_;
static unsigned char estado_;
static uint8_t button_ = 0;

enum {
	BUTTON_IRQ_TRIGGER,
	BUTTON_READ
};

void inicializarButtonLed(void) {
	DDRD |= _BV(DDD7);			// set pin 7 of PORTD for output

	DDRD &= ~(_BV(DDD2));     	// Clear the PD2 pin
	// PD2 (PCINT0 pin) is now an input

	PORTD |= _BV(PORTD2);  	// turn On the Pull-up
	// PD2 is now an input with pull-up enabled

	EICRA |= (1<<ISC01)|(0<<ISC00)|(1<<ISC11)|(0<<ISC10);
	EIMSK |= _BV(INT0);     	// Turns on INT0

	timer_start(&timer_);
	estado_ = BUTTON_READ;
}

void executarTarefaButtonLed(void) {
	switch (estado_) {
		case BUTTON_IRQ_TRIGGER:
			if (timer_check(timer_, 15)) {
				if (BUTTON_D2_PRESSED()) {
					if (!button_) {
						PORTD |= _BV(PORTD7);
						button_ = 1;
					} else {
						PORTD &= ~_BV(PORTD7);
						button_ = 0;
					}
					estado_ = BUTTON_READ;
				}
				EIMSK |= _BV(INT0);     	// Turns on INT0
			}
			break;

		case BUTTON_READ:
			break;
	}
}

ISR(INT0_vect) {
	estado_ = BUTTON_IRQ_TRIGGER;
	EIMSK |= 0;     	// Turns off INT0
	timer_start(&timer_);
}
