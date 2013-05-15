/*
 * timeout.h
 *
 *  Created on: 25.04.2013
 *      Author: krichevskiy
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <inttypes.h>
#include <stdbool.h>

typedef uint16_t timeout_counter_t;

struct timeout_struct;

struct timeout_struct {
	timeout_counter_t time;
	struct timeout_struct* next;
};

typedef struct timeout_struct timeout_t;

static inline bool timeout_is_elapsed(const timeout_t* timeout) {
	return (timeout->time == 0);
}

extern void timeout_init(timeout_t* timeout);
extern void timeout_reset(timeout_t* timeout, timeout_counter_t time);
extern void timeout_tick(void);
extern bool timeout_is_elapsed(const timeout_t* timeout);

#endif /* TIMEOUT_H_ */
