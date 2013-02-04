#include "queue.h"

#include <util/atomic.h>

void q_create(queue_t *q, void *pool, uint8_t size);
void q_put(queue_t *q, q_element_t elem);
q_element_t q_get(queue_t *q);
void q_flush(queue_t *q);
unsigned char q_rest(const queue_t *q);
uint8_t q_space(const queue_t *q);

void q_create(queue_t *q, void *pool, uint8_t size)
{
#if 1
    q->in = pool;
    q->out = pool;
    q->data_start = pool;
    q->size = size/sizeof(q_element_t);
    q->data_end = (q->data_start + q->size);
    q->count = 0;
#else
    q->in = 0;
    q->out = 0;
    q->data = pool;
    q->size = size/sizeof(q_element_t);
    q->count = 0;
#endif
}

void q_put(queue_t *q, q_element_t elem)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		++q->count;
	}
#if 1
    *q->in++ = elem;

    if (q->in == q->data_end) {
        q->in = q->data_start;
    }
#else
    q->data[q->in] = elem;
    if (++q->in == q->size) {
    	q->in = 0;
    }
#endif
}

q_element_t q_get(queue_t *q)
{
    q_element_t elem;
#if 1
    elem = *q->out++;
    if (q->out == q->data_end) {
        q->out = q->data_start;
    }
#else

    elem = q->data[q->out];
    if (++q->out == q->size) {
    	q->out = 0;
    }
#endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		--q->count;
	}

    return elem;
}

void q_flush(queue_t *q) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		q->count = 0;
		q->out = q->in;
	}
}

uint8_t q_rest(const queue_t *q) {
	return q->count;
}

uint8_t q_space(const queue_t *q) {
	return (q->size - q->count);
}
