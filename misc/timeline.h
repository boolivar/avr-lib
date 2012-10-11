#ifndef TIMELINE_H_
#define TIMELINE_H_

typedef unsigned short timeline_t;
typedef void (*timeline_callback_t)(void);

typedef struct {
	unsigned char ready;
	unsigned char active;
	timeline_t time;
	timeline_callback_t cb;
	timepoint_t* next;
} timepoint_t;

extern void timeline_tick(void);
extern void timeline_exec(void);
extern void timeline_set_timepoint(timepoint_t* timepoint, timeline_t time, timeline_callback_t cb);

#endif
