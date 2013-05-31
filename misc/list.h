/*
 * list.h
 *
 *  Created on: 30.05.2013
 *      Author: krichevskiy
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>

typedef uint8_t list_size_t;

typedef struct list_elem_struct {
	void* data;
	struct list_elem_struct *next;
} list_elem_t;

typedef struct {
	list_elem_t head;
	//list_elem_t tail;
	list_size_t len;
} list_t;

static inline list_size_t list_len(list_t* list) {
	return list->len;
}

static inline list_elem_t* list_get_first(list_t* list) {
	return list->head.next;
}

static inline list_elem_t* list_get_next(list_elem_t* elem) {
	return elem->next;
}

//#define list_get_data(elem, type) (type)(elem->data)
//#define list_set_data(elem, data) elem->data = data

extern void list_init(list_t* list);
extern void list_insert(list_t* list, list_elem_t* elem, list_size_t pos);
extern list_elem_t* list_remove(list_t* list, list_size_t pos);
extern list_elem_t* list_get(const list_t* list, list_size_t pos);

#endif /* LIST_H_ */
