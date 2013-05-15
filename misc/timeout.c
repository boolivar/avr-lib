/*
 * timeouts.c
 *
 *  Created on: 24.04.2013
 *      Author: krichevskiy
 */

#include "timeout.h"

#include <util/atomic.h>

#include <string.h>

//static timeout_t* head = NULL;
static timeout_t head = {0};

void timeout_init(timeout_t* timeout);
void timeout_reset(timeout_t* timeout, timeout_counter_t time);
void timeout_tick(void);

static bool timeout_is_linked(const timeout_t* timeout);

void timeout_init(timeout_t* timeout) {
	timeout->time = 0;
	timeout->next = NULL;
}

void timeout_reset(timeout_t* timeout, timeout_counter_t time) {
#if 0
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (!timeout_is_linked(timeout)) {
			timeout->next = head;	// not need to be atomic
			head = timeout;
		}
		timeout->time = time;
	}
#else
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (!timeout_is_linked(timeout)) {
			timeout->next = head.next;
			head.next = timeout;
		}
		timeout->time = time;
	}
#endif
}

void timeout_tick() {
	timeout_t* timeout;

#if 0
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
#else
	timeout = &head;
	while (timeout->next) {
		if (--timeout->next->time == 0) {
			timeout->next = timeout->next->next;
		}
		timeout = timeout->next;
	}
#endif
}

static bool timeout_is_linked(const timeout_t* timeout) {
	return (timeout->time != 0);
}
