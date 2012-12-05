
#include "time.h"

#include <util/atomic.h>

static timer_t* timer_chain = 0;

void time_init(void);
void time_create_timer(timer_t *timer, unsigned int period, timer_callback_t callback);
void time_set_timer_period(timer_t *timer, unsigned int period);
void time_reset_timer(timer_t *timer);
void time_stop_timer(timer_t* timer);
bool time_is_timer_run(timer_t* timer);
void time_process_timers(void);
void time_tick(void);

void time_create_timer(timer_t *timer, unsigned int period, timer_callback_t callback) {
    timer->time = 0;
    timer->period = period;
    timer->callback = callback;
    timer->ready = 0;

    timer->next = timer_chain;
    timer_chain = timer;
}

void time_set_timer_period(timer_t *timer, unsigned int period) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        timer->period = period;
    }
}

void time_reset_timer(timer_t *timer) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        timer->time = timer->period;
    }
}

void time_stop_timer(timer_t* timer) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        timer->time = 0;
    }
}

bool time_is_timer_run(timer_t* timer) {
    bool is_timer_run;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        is_timer_run = (timer->time > 0);
    }

    return is_timer_run;
}

void time_process_timers() {
    timer_t* timer = timer_chain;

    while (timer) {
        if (timer->ready) {
            timer->ready = 0;
            timer->callback();
        }

        timer = timer->next;
    }
}

void time_tick() {
    timer_t* timer = timer_chain;

    while (timer) {
        if (timer->time) {
            if (--timer->time == 0) {
                timer->time = timer->period;
                timer->ready = 1;
            }
        }

        timer = timer->next;
    }
}
