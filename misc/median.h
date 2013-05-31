/*
 * median.h
 *
 *  Created on: 31.05.2013
 *      Author: krichevskiy
 */

#ifndef MEDIAN_H_
#define MEDIAN_H_

#include <stdint.h>

#include "list.h"

typedef uint8_t median_filter_window_size_t;
typedef uint16_t median_filter_value_t;

typedef struct {
	list_t list;
	median_filter_window_size_t size;
} median_filter_t;

typedef struct {
	uint16_t value;
	uint8_t ttl;
} median_filter_elem_t;

extern void median_filter_init(median_filter_t* filter, list_elem_t* list_items,
		median_filter_elem_t* filter_items, median_filter_window_size_t size);
extern uint16_t median_filter_out(median_filter_t* filter, median_filter_value_t val);

#endif /* MEDIAN_H_ */
