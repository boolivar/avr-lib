#include <timeline.h>

#include <string.h>

static timeline_t timeline = 0;
static timepoint_t *timepoint_head = NULL;

void timeline_tick(void);
void timeline_exec(void);
void timeline_set_timepoint(timepoint_t* timepoint, timeline_t time, timeline_callback_t cb);

void timeline_tick() {
	timepoint_t *timepoint = timepoint_head;

	++timeline;

	while (timepoint) {
		if (timepoint->active) {
			if (timepoint->time == timeline) {
				timepoint->active = 0;
				timepoint->ready = 1;
			}
		}
		timepoint = timepoint->next;
	}
}

void timeline_exec() {
	timepoint_t *timepoint = timepoint_head;
	while (timepoint) {
		if (timepoint->ready) {
			timepoint->ready = 0;
			timepoint->cb();
		}
	}
}

void timeline_set_timepoint(timepoint_t* timepoint, timeline_t time, timeline_callback_t cb) {
	timepoint->ready = 0;
	timepoint->active = 1;
	timepoint->next = timepoint_head;
	timepoint->cb = cb;
	
	ATOMICBLOCK(ATOMIC_RESTORESTATE) {
		timepoint_head = timepoint;
		timepoint->time = (timeline + time);
	}
}