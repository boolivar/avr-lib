#include <avr/io.h>

#include "extint.h"

void extint_set_int0_sense_control(extint_sense_enum input_sense);
void extint_set_int1_sense_control(extint_sense_enum input_sense);
void extint_int_ena(extint_enum extint_mask);
void extint_int_dis(extint_enum extint_mask);

void extint_set_int0_sense_control(extint_sense_enum input_sense)
{
	register uint8_t mcucr;

	mcucr = MCUCR;
	mcucr &= ~(_BV(ISC01) | _BV(ISC00));
	MCUCR |= (input_sense << ISC00);

	MCUCR = mcucr;
}

void extint_set_int1_sense_control(extint_sense_enum input_sense)
{
	register uint8_t mcucr;

	mcucr = MCUCR;
	mcucr &= ~(_BV(ISC11) | _BV(ISC10));
	MCUCR |= (input_sense << ISC10);

	MCUCR = mcucr;
}

void extint_int_ena(extint_enum extint_mask)
{
	register uint8_t gifr;
	register uint8_t gicr;

	gifr = 0;
	gicr = GICR;

	if (extint_mask & EXTINT_INT0) {
		gifr |= _BV(INTF0);
		gicr |= _BV(INT0);
	}

	if (extint_mask & EXTINT_INT1) {
		gifr |= _BV(INTF1);
		gicr |= _BV(INT1);	
	}

	GIFR = gifr;
	GICR = gicr;
}

void extint_int_dis(extint_enum extint_mask)
{
#if 0
	register uint8_t gicr;

	gicr = GICR;

	if (extint_mask & EXTINT_INT0) {
		gicr &= ~_BV(INT0);
	}

	if (extint_mask & EXTINT_INT1) {
		gicr &= ~_BV(INT1);	
	}

	GICR = gicr;
#else
	GICR &= ~extint_mask;
#endif
}