/*
 * debounce.h
 *
 *  Created on: 09.12.2010
 *      Author: krichevskiy
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <stdint.h>
#include <stdbool.h>

/*==============================
 * Определи размер здесь.
 *=============================*/
typedef uint8_t deb_value_t;		// Разрядность значений для антидребезга.
typedef uint8_t deb_counter_t;		// Разрядность счетчика антидребезга.

typedef struct{
	deb_counter_t deb_counter;
	deb_counter_t deb_level;
	deb_value_t prev_value;
} Debounce;

typedef struct{
	deb_counter_t deb_counter;
	deb_counter_t deb_level;
	deb_value_t level_top;
	deb_value_t level_bottom;
} DebounceGate;

extern void debounce_create(Debounce *d, deb_value_t debounce_level);
extern bool debounce_level_steady(Debounce *d, deb_value_t value);
extern bool debounce_binary_reversible(Debounce *d, deb_value_t value);

extern void debounce_gate_create(DebounceGate *d, deb_value_t debounce_level, deb_value_t min, deb_value_t max);
extern bool debounce_gate_steady(DebounceGate *d, deb_value_t value);

#endif /* DEBOUNCE_H_ */
