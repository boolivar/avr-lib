#ifndef __TIME_H__
#define __TIME_H__

#include <stdbool.h>

typedef void (*timer_callback_t)(void);

typedef struct {
    timer_callback_t callback;
    unsigned int time;
    unsigned int period;
    unsigned char ready;
    void* next;
} timer_t;

extern void time_init(void);
extern void time_create_timer(timer_t *timer, unsigned int period, timer_callback_t callback);
extern void time_set_timer_period(timer_t *timer, unsigned int period);
extern void time_reset_timer(timer_t *timer);
extern void time_stop_timer(timer_t* timer);
extern void time_process_timers(void);
extern bool time_is_timer_run(timer_t* timer);
extern void time_tick(void);

#endif              
