#ifndef QUEUE_EX_H__
#define QUEUE_EX_H__

typedef uint8_t q_ex_elem_t;
typedef uint8_t q_ex_size_t;
typedef uint8_t q_ex_err_t;

typedef struct {
	q_ex_elem_t* data;
	q_ex_size_t in;
	q_ex_size_t out;
	q_ex_size_t size;
} q_ex_t;

void q_ex_create(q_ex_t* q, void* pool, q_ex_size_t size);
q_ex_err_t q_ex_push_back(q_ex_t* q, q_ex_elem_t elem);
q_ex_elem_t q_ex_get(q_ex_t* q);
q_ex_err_t q_ex_push_front(q_ex_t* q, q_ex_elem_t elem);
void q_ex_is_empty(q_ex_t* q);
void q_ex_flush(q_ex_t* q);

#endif
