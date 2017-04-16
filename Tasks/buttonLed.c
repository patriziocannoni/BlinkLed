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
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include "buttonLed.h"

#define BUTTON_D2_PRESSED() !(PIND & _BV(PORTD2))

//static unsigned char estado_;
static uint8_t button_ = 0;
static xSemaphoreHandle xSemaphore;

static void executarTarefaButtonLed(void *arg);

//enum {
//	BUTTON_IRQ_TRIGGER,
//	BUTTON_READ
//};

void inicializarButtonLed(void) {
	DDRD |= _BV(DDD7);			// set pin 7 of PORTD for output

	DDRD &= ~(_BV(DDD2));     	// Clear the PD2 pin
	// PD2 (PCINT0 pin) is now an input

	PORTD |= _BV(PORTD2);  	// turn On the Pull-up
	// PD2 is now an input with pull-up enabled

	EICRA |= (1<<ISC01)|(0<<ISC00)|(1<<ISC11)|(0<<ISC10);
	EIMSK |= _BV(INT0);     	// Turns on INT0

	vSemaphoreCreateBinary(xSemaphore);

	if (xSemaphore != NULL) {
		xTaskCreate(executarTarefaButtonLed, (signed portCHAR *) "Handler", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	}

//	estado_ = BUTTON_READ;
}

static void executarTarefaButtonLed(void *arg) {

	for (;;) {
		xSemaphoreTake(xSemaphore, portMAX_DELAY);

		vTaskDelay(15);

		if (BUTTON_D2_PRESSED()) {
			if (!button_) {
				PORTD |= _BV(PORTD7);
				button_ = 1;
			} else {
				PORTD &= ~_BV(PORTD7);
				button_ = 0;
			}
		}

		EIMSK |= _BV(INT0);     	// Turns on INT0
	}


//	switch (estado_) {
//		case BUTTON_IRQ_TRIGGER:
//			if (timer_check(timer_, 15)) {
//				if (BUTTON_D2_PRESSED()) {
//					if (!button_) {
//						PORTD |= _BV(PORTD7);
//						button_ = 1;
//					} else {
//						PORTD &= ~_BV(PORTD7);
//						button_ = 0;
//					}
//					estado_ = BUTTON_READ;
//				}
//				EIMSK |= _BV(INT0);     	// Turns on INT0
//			}
//			break;
//
//		case BUTTON_READ:
//			break;
//	}
}

ISR(INT0_vect) {
	static portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);

	if (xHigherPriorityTaskWoken == pdTRUE) {
		EIMSK |= 0;     	// Turns off INT0
	}

}
