#ifndef QUEUE_H_
#define QUEUE_H_

typedef unsigned char q_element_t;

typedef struct queue_struct {
    q_element_t *in;
    q_element_t *out;
    unsigned char count;
    unsigned char *data_start;
    unsigned char *data_end;
} queue_t;

extern void q_create(queue_t *q, unsigned char *pool, unsigned char size);
extern void q_put(queue_t *q, q_element_t elem);
extern q_element_t q_get(queue_t *q);
extern unsigned char q_rest(const queue_t *q);

#endif
