/*
 * comparator.h
 *
 *  Created on: 18.10.2011
 *      Author: krichevskiy
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_

typedef enum {
	COMPARATOR_AIN0_INPUT = 0,
	COMPARATOR_INTERNAL_11V = 1,
} positive_input_enum;

typedef enum {
	COMPARATOR_ADC0 = 0,
	COMPARATOR_ADC1 = 1,
	COMPARATOR_ADC2 = 2,
	COMPARATOR_ADC3 = 3,

	COMPARATOR_ADC4 = 4,
	COMPARATOR_ADC5 = 5,
	COMPARATOR_ADC6 = 6,
	COMPARATOR_ADC7 = 7,

	COMPARATOR_AIN1_INPUT,
} negative_input_enum;

typedef enum {
	COMPARATOR_INT_OFF = 0,
	COMPARATOR_INT_TOGGLE = 0x08,
	COMPARATOR_INT_FALLING_EDGE = 0x0a,
	COMPARATOR_INT_RISING_EDGE = 0x0b
} comparator_interupt_enum;

extern void comparator_set_negative_input(negative_input_enum negative_input);
extern void comparator_set_positive_input(positive_input_enum positive_input);
extern uint8_t comparator_get_output(void);
extern void comparator_disable(void);
extern void comparator_int_ena(comparator_interupt_enum interrupt);

#endif /* COMPARATOR_H_ */
