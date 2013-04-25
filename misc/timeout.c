/*
 * timeouts.c
 *
 *  Created on: 24.04.2013
 *      Author: krichevskiy
 */

#include "timeout.h"

#include <util/atomic.h>

#include <string.h>

static timeout_t* head;

void timeout_init(timeout_t* timeout);
void timeout_reset(timeout_t* timeout, timeout_counter_t time);
void timeout_tick(void);
bool timeout_is_elapsed(const timeout_t* timeout);

static bool timeout_is_linked(const timeout_t* timeout);

void timeout_init(timeout_t* timeout) {
	timeout->time = 0;
	timeout->next = NULL;
}

void timeout_reset(timeout_t* timeout, timeout_counter_t time) {
	uint8_t linked_flag;

	linked_flag = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (timeout_is_linked(timeout)) {
			timeout->time = time;
			linked_flag = 1;
		}
	}

	if (!linked_flag){
		timeout->time = time;
		timeout->next = head;
		head = timeout;
	}
}

void timeout_tick() {
	timeout_t* timeout;

	while (head && (--head->time == 0)) {
		head = head->next;
	}

	if (head) {
		timeout = head;
		while (timeout->next) {
			if (--timeout->next->time == 0) {
				timeout->next = timeout->next->next;
			}
			timeout = timeout->next;
		}
	}
}

bool timeout_is_elapsed(const timeout_t* timeout) {
	return (timeout->time == 0);
}

static bool timeout_is_linked(const timeout_t* timeout) {
	return (timeout->time != 0);
}
