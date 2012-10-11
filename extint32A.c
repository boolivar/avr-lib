/*
 * extint2313.c
 *
 *  Created on: 29.07.2011
 *      Author: Полугрудов
 */
#include <avr/io.h>

#include "macros.h"
#include "extint32A.h"

void extint_int_ena(extint_enum extint_mask);
void extint_int_dis(extint_enum extint_mask);
void extint_set_int0_sense_control(input_sense_enum input_sense);
void extint_set_int1_sense_control(input_sense_enum input_sense);

void extint_int_ena(extint_enum extint_mask)
{
	register uint8_t gifr = 0;
	register uint8_t gicr = GICR;

#if 0
	gifr |= (extint_mask << INTF2);
	gicr |= (extint_mask << INT2);
#else
	if (extint_mask & INT0_ENA) {
		bit_set(gifr, INTF0);
		bit_set(gicr, INT0);
	}
	
	if (extint_mask & INT1_ENA) {
		bit_set(gifr, INTF1);
		bit_set(gicr, INT1);
	}
	
	if (extint_mask & INT2_ENA) {
		bit_set(gifr, INTF2);
		bit_set(gicr, INT2);
	}
#endif

	GIFR = gifr;
	GICR = gicr;
}

void extint_int_dis(extint_enum extint_mask)
{
	register uint8_t gicr = GICR;
	
#if 0
	GICR &=~ (extint_mask << INT2);
#else
	if (extint_mask & INT0_ENA) {
		bit_clr(gicr, INT0);
	}
		
	if (extint_mask & INT1_ENA) {
		bit_clr(gicr, INT1);
	}
		
	if (extint_mask & INT2_ENA) {
		bit_clr(gicr, INT2);
	}
#endif

	GICR = gicr;
}

void extint_set_int0_sense_control(input_sense_enum input_sense)
{
	register uint8_t mcucr = MCUCSR;
	
	switch(input_sense)
	{
		case IS_LOW:
			bit_clr(mcucr, ISC00);
			bit_clr(mcucr, ISC01);
		break;
		
		case IS_CHANGE:
			bit_set(mcucr, ISC00);
			bit_clr(mcucr, ISC01);
		break;
		
		case IS_FALLING_EDGE:
			bit_clr(mcucr, ISC00);
			bit_set(mcucr, ISC01);
		break;
		
		case IS_RISING_EDGE:
			bit_set(mcucr, ISC00);
			bit_set(mcucr, ISC01);
		break;
		
		default:		
		break;
	}
	
	MCUCR = mcucr;
}

void extint_set_int1_sense_control(input_sense_enum input_sense)
{
	register uint8_t mcucr = MCUCSR;
	
	switch(input_sense)
	{
		case IS_LOW:
			bit_clr(mcucr, ISC10);
			bit_clr(mcucr, ISC11);
		break;
		
		case IS_CHANGE:
			bit_set(mcucr, ISC10);
			bit_clr(mcucr, ISC11);
		break;
		
		case IS_FALLING_EDGE:
			bit_clr(mcucr, ISC10);
			bit_set(mcucr, ISC11);
		break;
		
		case IS_RISING_EDGE:
			bit_set(mcucr, ISC10);
			bit_set(mcucr, ISC11);
		break;
		
		default:
		break;
	}
	
	MCUCR = mcucr;
}
