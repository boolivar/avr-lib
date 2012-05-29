/*
 * debounce.c
 *
 *  Created on: 09.12.2010
 *      Author: krichevskiy
 */

#include "debounce.h"

void debounce_create(Debounce *d, deb_value_t debounce_level);
bool debounce_binary_reversible(Debounce *d, deb_value_t value);
bool debounce_level_steady(Debounce *d, deb_value_t value);

void debounce_gate_create(DebounceGate *d, deb_value_t debounce_level, deb_value_t min, deb_value_t max);
bool debounce_gate_steady(DebounceGate *d, deb_value_t value);

/*!
 * »нициализирует структуру антидребезга d.
 *  [in] debounce_level - порог антидребезга.
 */
void debounce_create(Debounce *d, deb_value_t debounce_level)
{
	d->deb_counter = 0;
	d->deb_level = debounce_level;
	d->prev_value = 0;
}

/*!
 * јнтидребезг установившегос€ состо€ни€
 * возвращает true если значение value
 * не мен€лось в течении d.deb_level последовательных
 * вызовов функции.
 */
bool debounce_level_steady(Debounce *d, deb_value_t value)
{
	if (value != d->prev_value)
	{
		d->prev_value = value;
		d->deb_counter = 0;

		return false;
	}

	if (d->deb_counter < d->deb_level)
	{
		++d->deb_counter;

		return (d->deb_counter == d->deb_level);
	}

	return true;
}

/*!
 * 	јнтидребезг реверсивным счетчиком,
 * устойчив к помехам, но примен€етс€ только
 * к бинарным вариантам значений (0/не 0).
 *
 *	“екущее значение, требующее проведени€
 * процедуры антидребезга записываетс€ в
 * value, значением можно пользоватьс€ в
 * случае успешного прохождени€ процедуры
 * антидребезга.
 *
 *	¬озвращает true если пройден антидребезг.
 */
bool debounce_binary_reversible(Debounce *d, deb_value_t value)
{
	bool result;

	if (value) {
		if (d->deb_counter < d->deb_level) {
			++d->deb_counter;
			return false;
		}
	} else {
		if (d->deb_counter) {
			--d->deb_counter;
			return false;
		}
	}

	result = (value != d->prev_value);
	d->prev_value = value;
	return result;
}

void debounce_gate_create(DebounceGate *d, deb_value_t debounce_level, deb_value_t min, deb_value_t max)
{
	d->deb_counter = 0;
	d->deb_level = debounce_level;
	d->level_bottom = min;
	d->level_top = max;
}

bool debounce_gate_steady(DebounceGate *d, deb_value_t value)
{
	if (value <= d->level_top
		&& value >= d->level_bottom)
	{
		if (d->deb_counter >= d->deb_level)
		{
			return true;
		}

		++d->deb_counter;
	}

	return false;
}
