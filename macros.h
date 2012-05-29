/*
 * macros.h
 *
 *  Created on: 18.10.2010
 *      Author: krichevskiy
 */

#ifndef MACROS_H_
#define MACROS_H_

#include <avr\sfr_defs.h>

#define FILL_BITS(x)  ((1 << x) - 1)

#define AR_SIZE(array)    (sizeof(array)/sizeof(array[0]))

#define bit_set(sfr, bit) (sfr |= (1 << bit))
#define bit_clr(sfr, bit) (sfr &= ~(1 << bit))

#endif /* MACROS_H_ */
