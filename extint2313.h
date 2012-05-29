/*
 * extint2313.h
 *
 *  Created on: 29.07.2011
 *      Author: krichevskiy
 */

#ifndef EXTINT2313_H_
#define EXTINT2313_H_

#include <avr/io.h>

typedef enum
{
	EXT_INT0 = _BV(0),
	EXT_INT1 = _BV(1),
	EXT_PIN_CHANGE = _BV(2)
} extint_enum;

typedef enum
{
	IS_LOW = 0,
	IS_CHANGE = 1,
	IS_FALLING_EDGE = 2,
	IS_RISING_EDGE = 3
} input_sense_enum;

typedef enum
{
	EXT_PCINT0 = _BV(0),
	EXT_PCINT1 = _BV(1),
	EXT_PCINT2 = _BV(2),
	EXT_PCINT3 = _BV(3),
	EXT_PCINT4 = _BV(4),
	EXT_PCINT5 = _BV(5),
	EXT_PCINT6 = _BV(6),
	EXT_PCINT7 = _BV(7)
} pin_change_enum;

extern void extint_int_ena(extint_enum extint_mask);
extern void extint_int_dis(extint_enum extint_mask);
extern void extint_set_int0_sense_control(input_sense_enum input_sense);
extern void extint_set_int1_sense_control(input_sense_enum input_sense);
inline void extint_set_pin_change_mask(pin_change_enum pin_change_mask)
{
	PCMSK = pin_change_mask;
}

#endif /* EXTINT2313_H_ */
