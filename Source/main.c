/*
 * main.c
 *
 *  Created on: 13/07/2014
 *      Author: Patrizio
 */

#include <avr/interrupt.h>
#include <timer.h>
#include <blinkLed.h>
#include <buttonLed.h>

int main(void) {
	cli();          			// disable global interrupts

	inicializarTimer();
	inicializarLed();
	inicializarButtonLed();

	sei();          			// enable global interrupts

	for (;;) {
		executarTarefaBlinkLed();
		executarTarefaButtonLed();
	}

	return 0;
}


