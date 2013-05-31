/*
 * moving_average.h
 *
 *  Created on: 31.05.2013
 *      Author: krichevskiy
 */

#ifndef MOVING_AVERAGE_H_
#define MOVING_AVERAGE_H_

#include <stdint.h>

typedef uint8_t window_size_t;
typedef uint16_t filter_value_t;
typedef uint16_t accumulator_t;

typedef struct moving_average_struct{
	filter_value_t* window;
	window_size_t size;
	window_size_t pos;
	accumulator_t acc;
} moving_average_t;

extern void moving_average_filter_init(moving_average_t* filter, filter_value_t* data, window_size_t size);
extern filter_value_t moving_average_filter_out(moving_average_t* filter, filter_value_t in);

#endif /* MOVING_AVERAGE_H_ */
