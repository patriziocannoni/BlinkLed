/*
 * blinkLed.c
 *
 *  Created on: 14/03/2015
 *      Author: Patrizio
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "blinkLed.h"

static unsigned char estado_;

enum {
	LED_ON,
	LED_OFF
};

static void executarTarefaBlinkLed(void *arg);

void inicializarLed(void) {
	DDRD |= _BV(DDD6);					// set pin 6 of PORTD for output
	estado_ = LED_OFF;
	xTaskCreate(executarTarefaBlinkLed, (signed portCHAR *) "BLINK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

static void executarTarefaBlinkLed(void *arg) {
	for (;;) {
		switch (estado_) {
			case LED_ON:
				PORTD &= ~_BV(PORTD6);		// Set pin 6 low to turn led ON.
				vTaskDelay(850);
				estado_ = LED_OFF;
				break;

			case LED_OFF:
				PORTD |= _BV(PORTD6);		// Set pin 6 high to turn led OFF.
				vTaskDelay(150);
				estado_ = LED_ON;
				break;
		}
	}
}
