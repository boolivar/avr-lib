#ifndef _EXTINT8_HEADER_INCLUDED_
#define _EXTINT8_HEADER_INCLUDED_

#include <avr/io.h>

typedef enum
{
	EXTINT_INT0 = _BV(INT0),
	EXTINT_INT1 = _BV(INT1)
} extint_enum;

typedef enum
{
	EXTINT_IS_LOW = 0,
	EXTINT_IS_CHANGE = 1,
	EXTINT_IS_FALLING_EDGE = 2,
	EXTINT_IS_RISING_EDGE = 3
} extint_sense_enum;

extern void extint_set_int0_sense_control(extint_sense_enum input_sense);
extern void extint_set_int1_sense_control(extint_sense_enum input_sense);
extern void extint_int_ena(extint_enum extint_mask);
extern void extint_int_dis(extint_enum extint_mask);

#endif
