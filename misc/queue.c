#include "queue.h"

void q_create(queue_t *q, unsigned char *pool, unsigned char size);
void q_put(queue_t *q, q_element_t elem);
q_element_t q_get(queue_t *q);
unsigned char q_rest(const queue_t *q);

void q_create(queue_t *q, unsigned char *pool, unsigned char size)
{
    q->in = pool;
    q->out = pool;
    q->data_start = pool;
    q->data_end = (pool + size);
    q->count = 0;
}

void q_put(queue_t *q, q_element_t elem)
{
    *q->in++ = elem;

    if (q->in == q->data_end) {
        q->in = q->data_start;
    }

    ++q->count;
}

q_element_t q_get(queue_t *q)
{
    q_element_t elem;

    --q->count;
    elem = *q->out++;
    if (q->out == q->data_end) {
        q->out = q->data_start;
    }

    return elem;
}

unsigned char q_rest(const queue_t *q)
{
    return q->count;
}
