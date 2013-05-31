/*
 * median.c
 *
 *  Created on: 31.05.2013
 *      Author: krichevskiy
 */

#include "median.h"
#include "list.h"

void median_filter_init(median_filter_t* filter, list_elem_t* list_items,
		median_filter_elem_t* filter_items, median_filter_window_size_t size);
uint16_t median_filter_out(median_filter_t* filter, median_filter_value_t val);

static void filter_insert_ordered(list_t* list, list_elem_t* new_elem);
static list_elem_t* filter_pop_oldest(list_t* list);

void median_filter_init(median_filter_t* filter, list_elem_t* list_items,
		median_filter_elem_t* filter_items, median_filter_window_size_t size) {
	uint8_t i;

	filter->size = size;
	list_init(&filter->list);

	for (i = 0; i < size; ++i) {
		filter_items[i].ttl = i+1;
		filter_items[i].value = 0;
		list_items[i].data = &filter_items[i];
		list_insert(&filter->list, &list_items[i], 0);
	}
}

uint16_t median_filter_out(median_filter_t* filter, median_filter_value_t val) {
	list_elem_t* elem;
	median_filter_elem_t* filter_elem;

	elem = filter_pop_oldest(&filter->list);

	filter_elem = elem->data;
	filter_elem->ttl = filter->size;
	filter_elem->value = val;
	filter_insert_ordered(&filter->list, elem);

	filter_elem = list_get(&filter->list, filter->size/2)->data;
	return filter_elem->value;
}

static list_elem_t* filter_pop_oldest(list_t* list) {
	list_elem_t* elem;
	list_size_t oldest;
	list_size_t pos;

	pos = 0;
	elem = list_get_first(list);
	while (elem) {
		median_filter_elem_t *filter_elem;

		filter_elem = elem->data;
		if (--filter_elem->ttl == 0) {
			oldest = pos;
		}
		elem = list_get_next(elem);
		++pos;
	}

	return list_remove(list, oldest);
}

static void filter_insert_ordered(list_t* list, list_elem_t* new_elem) {
	list_elem_t* elem;
	median_filter_elem_t* filter_elem;
	uint16_t new_value;
	list_size_t pos;

	filter_elem = new_elem->data;
	new_value = filter_elem->value;

	pos = 0;
	elem = list_get_first(list);
	while (elem) {
		filter_elem = elem->data;
		if (new_value < filter_elem->value) {
			break;
		}

		elem = list_get_next(elem);
		++pos;
	}

	list_insert(list, new_elem, pos);
}
