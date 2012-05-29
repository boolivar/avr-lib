/*
 * comparator.c
 *
 *  Created on: 18.10.2011
 *      Author: krichevskiy
 */

#include <avr/io.h>

#include "comparator.h"

void comparator_set_negative_input(negative_input_enum negative_input);
void comparator_set_positive_input(positive_input_enum positive_input);
uint8_t comparator_get_output(void);
void comparator_disable(void);
void comparator_int_ena(comparator_interupt_enum interrupt);

void comparator_set_negative_input(negative_input_enum negative_input) {
	if (negative_input == COMPARATOR_AIN1_INPUT) {
		ADCSRB &= ~_BV(ACME);
	}
	else {
		uint8_t tmp = ADMUX;
		tmp &= ~(0x0f);
		tmp |= negative_input;
		ADMUX = tmp;
		ADCSRB |= _BV(ACME);
	}
}

void comparator_set_positive_input(positive_input_enum positive_input) {
	if (positive_input == COMPARATOR_INTERNAL_11V) {
		ACSR |= _BV(ACBG);
	}
	else {
		ACSR &= ~_BV(ACBG);
	}
}

uint8_t comparator_get_output() {
	return (ACSR & _BV(ACO));
}

void comparator_disable() {
	ACSR |= _BV(ACD);
}

void comparator_int_ena(comparator_interupt_enum interrupt) {
	uint8_t tmp = ACSR;
	tmp &= ~(0x2b);
	tmp |= (interrupt | 0x10);
	ACSR = tmp;
}
