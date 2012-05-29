/*
 * extint2313.c
 *
 *  Created on: 29.07.2011
 *      Author: krichevskiy
 */

#include "extint2313.h"

#include <avr/io.h>

void extint_int_ena(extint_enum extint_mask);
void extint_int_dis(extint_enum extint_mask);
void extint_set_int0_sense_control(input_sense_enum input_sense);
void extint_set_int1_sense_control(input_sense_enum input_sense);

void extint_int_ena(extint_enum extint_mask)
{
	register unsigned char eifr;
	register unsigned char gimsk;

	eifr = 0;
	gimsk = GIMSK;

	if (extint_mask & EXT_INT0)
	{
		eifr |= _BV(INTF0);
		gimsk |= _BV(INT0);
	}

	if (extint_mask & EXT_INT1)
	{
		eifr |= _BV(INTF1);
		gimsk |= _BV(INT1);
	}

	if (extint_mask & EXT_PIN_CHANGE)
	{
		eifr |= _BV(PCIF);
		gimsk |= _BV(PCIE);
	}

	EIFR = eifr;
	GIMSK = gimsk;
}

void extint_int_dis(extint_enum extint_mask)
{
	register unsigned char gimsk;

	gimsk = GIMSK;

	if (extint_mask & EXT_INT0)
	{
		gimsk &= ~_BV(INT0);
	}

	if (extint_mask & EXT_INT1)
	{
		gimsk &= ~_BV(INT1);
	}

	if (extint_mask & EXT_PIN_CHANGE)
	{
		gimsk &= ~_BV(PCIE);
	}

	GIMSK = gimsk;
}

void extint_set_int0_sense_control(input_sense_enum input_sense)
{
	register unsigned char mcucr;

	mcucr = MCUCR;

	mcucr &= ~( _BV(ISC01) | _BV(ISC00) );
	mcucr |= (input_sense << ISC00);

	MCUCR = mcucr;
}

void extint_set_int1_sense_control(input_sense_enum input_sense)
{
	register unsigned char mcucr;

	mcucr = MCUCR;

	mcucr &= ~( _BV(ISC11) | _BV(ISC10) );
	mcucr |= (input_sense << ISC10);

	MCUCR = mcucr;
}
