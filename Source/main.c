/*
 * main.c
 *
 *  Created on: 13/07/2014
 *      Author: Patrizio
 */

#include <avr/interrupt.h>
#include <blinkLed.h>
#include <buttonLed.h>
#include <task.h>

int main(void) {
	cli();          			// disable global interrupts

	inicializarLed();
	inicializarButtonLed();

	sei();          			// enable global interrupts

	vTaskStartScheduler();

	return 0;
}


