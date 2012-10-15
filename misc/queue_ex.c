
#include "queue_ex.h"

void q_ex_create(q_ex_t* q, void* pool, q_ex_size_t size);
q_ex_err_t q_ex_push_back(q_ex_t* q, q_ex_elem_t elem);
q_ex_elem_t q_ex_get(q_ex_t* q);
q_ex_err_t q_ex_push_front(q_ex_t* q, q_ex_elem_t elem);
void q_ex_is_empty(q_ex_t* q);
void q_ex_flush(q_ex_t* q);

void q_ex_create(q_ex_t* q, void* pool, q_ex_size_t size)
{
	q->data = pool;
	q->size = size;
	
	q->in = 0;
	q->out = 0;
}

q_ex_err_t q_ex_push_back(q_ex_t* q, q_ex_elem_t elem)
{
	q_ex_size_t next;
	
	next = q->in + 1;
	if (next == q->size) {
		next = 0;
	}
	
	if (next == q->out) {
		return 1;
	}
	
	q->data[next] = elem;
	q->in = next;
	
	return 0;
}

q_ex_elem_t q_ex_get(q_ex_t* q)
{
	q_ex_size_t next;
	q_ex_elem_t elem;
	
	next = q->out;
	elem = q->data[next];
	
	if (++next == q->size) {
		next = 0;
	}
	q->out = next;
	
	return elem;
}

q_ex_err_t q_ex_push_front(q_ex_t* q, q_ex_elem_t elem)
{
	q_ex_size_t next;
	
	next = q->out;
	
	if (next == 0) {
		next = q->size;
	}
	--next;
	
	if (next == q->in) {
		return 1;
	}
	
	q->data[next] = elem;
	q->out = next;
		
	return 0;
}

void q_ex_is_empty(q_ex_t* q)
{
	return q->in == q->out;
}

void q_ex_flush(q_ex_t* q)
{
	q->in = q->out;
}
