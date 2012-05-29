//==============================================================================
// Description: Работа с таймерами ATMega328
//              
// Author: pogorelov 
// Revision:  
// Date: 
//==============================================================================

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "timer328.h"

void set_timer0_wg_mode(timer0_wg_mode_enum wgm)
{
	TCCR0A &= ~(_BV(WGM01) | _BV(WGM00));
	TCCR0B &= ~_BV(WGM02);

	TCCR0A |= (wgm & 0x03);
	TCCR0B |= ((wgm >> 2) << WGM02);
/*
	switch (wgm)
	{
	case T0_WG_NORMAL:
		break;

	case T0_WG_PWM_PHASE_CORRECT:
		TCCR0A |= _BV(WGM00);
		break;

	case T0_WG_CTC:
		TCCR0A |= _BV(WGM01);
		break;

	case T0_WG_FAST_PWM:
		TCCR0A |= (_BV(WGM00) | _BV(WGM01));
		break;

	case T0_WG_PWM_PHASE_CORRECT_OCRA:
		TCCR0B |= _BV(WGM02);
		TCCR0A |= _BV(WGM00);
		break;

	case T0_WG_FAST_PWM_OCRA:
		TCCR0B |= _BV(WGM02);
		TCCR0A |= _BV(WGM01) | _BV(WGM00);
		break;

	default:
		break;
	} // switch
*/
}

void set_timer0_clock_source(timer0_clock_select_enum clock_select)
{
	uint8_t tmp = TCCR0B;
	tmp &= ~0x07;
	tmp |= (clock_select & 0x07);
	TCCR0B = tmp;
}

void set_timer0_OC1A_mode(timer0_compare_output_mode_enum mode)
{
	TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0));
	TCCR0A |= (mode << COM0A0);
}

void set_timer0_OC1B_mode(timer0_compare_output_mode_enum mode)
{
	TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0));
	TCCR0A |= (mode << COM0B0);
}

void timer0_int_ena(timer0_int_enum timer0_int)
{
	switch (timer0_int)
	{
	case TIMER0_OVERFLOW_INTERRUPT:
		TIFR0 |= _BV(TOV0); //clear pending ints
		TIMSK0 |= _BV(TOIE0);
		break;

	case TIMER0_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
		TIFR0 |= _BV(OCF0A); //clear pending ints
		TIMSK0 |= _BV(OCIE0A);
		break;

	case TIMER0_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
		TIFR0 |= _BV(OCF0B); //clear pending ints
		TIMSK0 |= _BV(OCIE0B);
		break;

	case TIMER0_ALL_INTERRUPTS:
		TIFR0 |= (_BV(OCF0A) | _BV(OCF0B) | _BV(TOV0)); //clear pending ints
		TIMSK0 |= (_BV(OCIE0A) | _BV(OCIE0B) | _BV(TOIE0));
		break;

	default:
		break;
	}/*** switch ***/
}

void timer0_int_dis(timer0_int_enum timer0_int)
{
	switch (timer0_int)
	{
	case TIMER0_OVERFLOW_INTERRUPT:
		TIMSK0 &= ~_BV(TOIE0);
		break;

	case TIMER0_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
		TIMSK0 &= ~_BV(OCIE0A);
		break;

	case TIMER0_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
		TIMSK0 &= ~_BV(OCIE0B);
		break;

	case TIMER0_ALL_INTERRUPTS:
		TIMSK0 &= ~(_BV(OCIE0A) | _BV(OCIE0B) | _BV(TOIE0));
		break;
	default:
		break;
	}/*** switch ***/
}

void set_timer1_wg_mode(timer1_wg_mode_enum wgm)
{
	TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));
	TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));

	switch (wgm)
	{
	case NORMAL:
		break;
	case PWM_PHASE_CORRECT_8_BIT:
		TCCR1A |= _BV(WGM10);
		break;
	case PWM_PHASE_CORRECT_9_BIT:
		TCCR1A |= _BV(WGM11);
		break;
	case PWM_PHASE_CORRECT_10_BIT:
		TCCR1A |= (_BV(WGM11) | _BV(WGM10));
		break;
	case CTC_OCR1A:
		TCCR1B |= _BV(WGM12);
		break;
	case FAST_PWM_8_BIT:
		TCCR1A |= _BV(WGM10);
		TCCR1B |= _BV(WGM12);
		break;
	case FAST_PWM_9_BIT:
		TCCR1A |= _BV(WGM11);
		TCCR1B |= _BV(WGM12);
		break;
	case FAST_PWM_10_BIT:
		TCCR1A |= (_BV(WGM11) | _BV(WGM10));
		TCCR1B |= _BV(WGM12);
		break;
	case PWM_PHASE_AND_FREQ_CORRECT_ICR1:
		TCCR1B |= _BV(WGM13);
		break;
	case PWM_PHASE_AND_FREQ_CORRECT_OCR1A:
		TCCR1A |= _BV(WGM10);
		TCCR1B |= _BV(WGM13);
		break;
	case PWM_PHASE_CORRECT_ICR1:
		TCCR1A |= _BV(WGM11);
		TCCR1B |= _BV(WGM13);
		break;
	case PWM_PHASE_CORRECT_OCR1A:
		TCCR1A |= (_BV(WGM11) | _BV(WGM10));
		TCCR1B |= _BV(WGM13);
		break;
	case CTC_ICR1:
		TCCR1B |= (_BV(WGM13) | _BV(WGM12));
		break;
	case RESERVED:
		TCCR1A |= _BV(WGM10);
		TCCR1B |= (_BV(WGM13) | _BV(WGM12));
		break;
	case FAST_PWM_ICR1:
		TCCR1A |= _BV(WGM11);
		TCCR1B |= (_BV(WGM13) | _BV(WGM12));
		break;
	case FAST_PWM_OCR1A:
		TCCR1A |= (_BV(WGM11) | _BV(WGM10));
		TCCR1B |= (_BV(WGM13) | _BV(WGM12));
		break;
	default:
		break;
	}/*** switch ***/
}

void set_timer1_OC1A_mode(timer1_compare_output_mode_enum mode)
{
	TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0));

	switch (mode)
	{
	case T1_NORMAL_PORT_OPERATION:
		break;
	case T1_TOGGLE:
		TCCR1A |= _BV(COM1A0);
		break;
	case T1_CLEAR:
		TCCR1A |= _BV(COM1A1);
		break;
	case T1_SET:
		TCCR1A |= (_BV(COM1A1) | _BV(COM1A0));
		break;
	default:
		break;
	}/*** switch ***/
}

void set_timer1_OC1B_mode(timer1_compare_output_mode_enum mode)
{
	TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));

	switch (mode)
	{
	case T1_NORMAL_PORT_OPERATION:
		break;
	case T1_TOGGLE:
		TCCR1A |= _BV(COM1B0);
		break;
	case T1_CLEAR:
		TCCR1A |= _BV(COM1B1);
		break;
	case T1_SET:
		TCCR1A |= (_BV(COM1B1) | _BV(COM1B0));
		break;
	default:
		break;
	}/*** switch ***/
}

void set_timer1_clock_source(timer1_clock_select_enum clock_select)
{
	uint8_t tmp = TCCR1B;
	tmp &= ~0x07;
	tmp |= (clock_select & 0x07);
	TCCR1B = tmp;
}

void timer1_int_ena(timer1_int_enum timer1_int)
{
	TIFR1 |= 1 << TOV1; //clear pending ints

	switch (timer1_int)
	{
	case TIMER1_INPUT_CAPTURE_INTERRUPT:
		TIMSK1 |= 1 << ICIE1;
		break;
	case TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
		TIMSK1 |= 1 << OCIE1A;
		break;
	case TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
		TIMSK1 |= 1 << OCIE1B;
		break;
	case TIMER1_OVERFLOW_INTERRUPT:
		TIMSK1 |= 1 << TOIE1;
		break;
	default:
		break;
	}/*** switch ***/
}

void timer1_int_dis(timer1_int_enum timer1_int)
{
	switch (timer1_int)
	{
	case TIMER1_INPUT_CAPTURE_INTERRUPT:
		TIMSK1 &= ~(1 << ICIE1);
		break;
	case TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
		TIMSK1 &= ~(1 << OCIE1A);
		break;
	case TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
		TIMSK1 &= ~(1 << OCIE1B);
		break;
	case TIMER1_OVERFLOW_INTERRUPT:
		TIMSK1 &= ~(1 << TOIE1);
		break;
	case TIMER1_ALL_INTERRUPTS:
		TIMSK1
				&= ~((1 << ICIE1) | (1 << OCIE1A) | (1 << OCIE1B) | (1
						<< TOIE1));
		break;
	default:
		break;
	}/*** switch ***/
}

void set_timer2_wg_mode(timer2_wg_mode_enum wgm)
{
	uint8_t tmp = TCCR2A;

	tmp &= ~(_BV(WGM20) | _BV(WGM21));

	switch (wgm)
	{
	case T2_WG_NORMAL:
		break;
	case T2_WG_PWM_PHASE_CORRECT:
		tmp |= _BV(WGM20);
		break;
	case T2_WG_CTC:
		tmp |= _BV(WGM21);
		break;
	case T2_WG_FAST_PWM:
		tmp |= (_BV(WGM20) | _BV(WGM21));
		break;
	}/*** switch ***/

	TCCR2A = tmp;
}

void set_timer2_clock_source(timer2_clock_select_enum clock_select)
{
	uint8_t tmp = TCCR2B;
	tmp &= ~0x07;
	tmp |= (clock_select & 0x07);
	TCCR2B = tmp;
}

void timer2_int_ena(timer2_int_enum timer2_int)
{
	switch (timer2_int)
	{
	case TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT:
		TIFR2 |= _BV(OCF2A); //clear pending ints
		TIMSK2 |= _BV(OCIE2A);
		break;
	case TIMER2_OVERFLOW_INTERRUPT:
		TIFR2 |= _BV(TOV2); //clear pending ints
		TIMSK2 |= _BV(TOIE2);
		break;
	default:
		break;
	}/*** switch ***/
}

void timer2_int_dis(timer2_int_enum timer2_int)
{
	switch (timer2_int)
	{
	case TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT:
		TIMSK2 &= ~_BV(OCIE2A);
		break;
	case TIMER2_OVERFLOW_INTERRUPT:
		TIMSK2 &= ~_BV(TOIE2);
		break;
	case TIMER2_ALL_INTERRUPTS:
		TIMSK2 &= ~(_BV(OCF2A) | _BV(TOIE2));
		break;
	default:
		break;
	}/*** switch ***/
}

