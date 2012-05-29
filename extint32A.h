/*
 * extint2313.h
 *
 *  Created on: 29.07.2011
 *      Author: Полугрудов
 */

#ifndef EXTINT32A_H_
#define EXTINT32A_H_

#include <avr/io.h>

typedef enum
{
	INT2_ENA = 1,
	INT0_ENA = 2,
	INT1_ENA = 4
} extint_enum;

typedef enum
{
	IS_LOW = 0,
	IS_CHANGE = 1,
	IS_FALLING_EDGE = 2,
	IS_RISING_EDGE = 3
} input_sense_enum;

extern void extint_int_ena(extint_enum extint_mask);
extern void extint_int_dis(extint_enum extint_mask);
extern void extint_set_int0_sense_control(input_sense_enum input_sense);
extern void extint_set_int1_sense_control(input_sense_enum input_sense);

#endif /* EXTINT32A_H_ */
