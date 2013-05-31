/*
 * list.c
 *
 *  Created on: 30.05.2013
 *      Author: krichevskiy
 */

#include "list.h"

#include <string.h>

void list_init(list_t* list);
void list_insert(list_t* list, list_elem_t* elem, list_size_t pos);
list_elem_t* list_remove(list_t* list, list_size_t pos);
list_elem_t* list_get(const list_t* list, list_size_t pos);

void list_init(list_t* list) {
	list->head.next = NULL;
	list->len = 0;
}

void list_insert(list_t* list, list_elem_t* elem, list_size_t pos) {
	list_size_t i;
	list_elem_t *prev;

	prev = &list->head;
	i = 0;

	while (i < pos) {
		prev = prev->next;
		++i;
	}

	elem->next = prev->next;
	prev->next = elem;
	++list->len;
}

list_elem_t* list_remove(list_t* list, list_size_t pos) {
	if (pos < list->len) {
		list_elem_t *prev;
		list_elem_t *elem;

		prev = &list->head;
		elem = list->head.next;

		while (pos) {
			prev = elem;
			elem = elem->next;
			--pos;
		}

		prev->next = elem->next;
		--list->len;

		return elem;
	}

	return NULL;
}

list_elem_t* list_get(const list_t* list, list_size_t pos) {
	if (pos < list->len) {
		list_elem_t *elem;

		elem = list->head.next;
		while (pos) {
			elem = elem->next;
			--pos;
		}
		return elem;
	}

	return NULL;
}
