/*
 * moving_average.c
 *
 *  Created on: 31.05.2013
 *      Author: krichevskiy
 */

#include <string.h>

#include "moving_average.h"

void moving_average_filter_init(moving_average_t* filter, filter_value_t* data, window_size_t size);
filter_value_t moving_average_filter_out(moving_average_t* filter, filter_value_t in);

void moving_average_filter_init(moving_average_t* filter, filter_value_t* data, window_size_t size) {
	memset(data, 0, size);
	filter->window = data;
	filter->size = size;
	filter->pos = 0;
	filter->acc = 0;
}

filter_value_t moving_average_filter_out(moving_average_t* filter, filter_value_t in) {
	filter->acc -= filter->window[filter->pos];
	filter->acc += in;

	filter->window[filter->pos] = in;
	if (++filter->pos == filter->size) {
		filter->pos = 0;
	}

	return (filter->acc + filter->size/2)/filter->size;
}
