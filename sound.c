#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer8.h"

#include "sound.h"

static const melody_t *melody_ptr = 0;
static const melody_t *melody_start;
static uint8_t melody_loop;
static melody_t melody_data = {0};
static uint8_t is_playing_now = 0;

void sound_init(void);
void start_sound(const melody_t* melody, uint8_t loop);
void stop_sound(void);

static inline void enable_sound(void);
static inline void disable_sound(void);
static void set_note(uint16_t note);

static void init_timer_0 (void);
static void init_timer_1(void);

void sound_init() {
	init_timer_0();
	init_timer_1();
}

static void init_timer_0(void)
{
	set_timer0_clock_source(T0S_DIV1024);
	timer0_int_ena(TIMER0_OVERFLOW_INTERRUPT);
}

static void init_timer_1(void)
{
	set_timer1_OC1A_mode(CLEAR);
	set_timer1_wg_mode(FAST_PWM_ICR1);
	set_timer1_clock_source(T1S_DIV1);
}

void start_sound(const melody_t* melody, uint8_t loop)
{
	if (!is_playing_now) {
		melody_ptr = melody;
		melody_start = melody;
		melody_loop = loop;

		melody_data.len = 0;
		is_playing_now = 1;
	}
}

void stop_sound() {
	is_playing_now = 0;
	disable_sound();
}

ISR(TIMER0_OVF_vect) {
	if (is_playing_now) {
		if (melody_data.len == 0) {
			melody_data = *melody_ptr++;
			if (melody_data.len) {
				set_note(melody_data.note);
			}else {
				if (melody_loop) {
					melody_ptr = melody_start;
				}else {
					is_playing_now = 0;
					disable_sound();
				}
			}
		}else {
			--melody_data.len;
		}
	}
}

static void set_note(uint16_t note) {
	if (note) {
		ICR1 = note;
		OCR1A = note/2;
		enable_sound();
	}else {
		disable_sound();
	}
}

static inline void disable_sound() {
	SOUND_DDRX &= ~_BV(SOUND_PIN);
}

static inline void enable_sound() {
	SOUND_DDRX |= _BV(SOUND_PIN);
}
