#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>

typedef uint8_t q_element_t;

#if 1

typedef struct queue_struct {
    q_element_t *in;
    q_element_t *out;
    q_element_t *data_start;
    q_element_t *data_end;
    uint8_t size;
    uint8_t count;
} queue_t;

#else

typedef struct queue_struct {
    q_element_t *data;

    uint8_t in;
    uint8_t out;

    uint8_t size;
    uint8_t count;
} queue_t;

#endif

extern void q_create(queue_t *q, void *pool, unsigned char size);
extern void q_put(queue_t *q, q_element_t elem);
extern q_element_t q_get(queue_t *q);
extern void q_flush(queue_t *q);
extern unsigned char q_rest(const queue_t *q);
extern uint8_t q_space(const queue_t *q);

#endif
