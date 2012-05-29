#ifndef FREQ_HEADER_INCLUDED
#define FREQ_HEADER_INCLUDED

typedef enum {
	TIMER_0_1_PS,
	TIMER_2_PS
} timer_prescalers_enum;

extern unsigned char fit_prescaler(unsigned long freq, unsigned int* top, timer_prescalers_enum tps);

#endif

