//==============================================================================
// Description: Работа с таймерами ATmega8
//              
// Author: pogorelov 
// Revision:  
// Date: 
//==============================================================================

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "timer8.h"

void set_timer0_clock_source(timer0_clock_select_enum clock_select)
{
  uint8_t tmp = TCCR0;
  tmp &= ~0x07;
  tmp |= (clock_select & 0x07);
  TCCR0 = tmp;
}

void timer0_int_ena(timer0_int_enum timer0_int)
{
  switch(timer0_int)
    {
    case TIMER0_OVERFLOW_INTERRUPT:
      TIFR |= _BV(TOV0);   //clear pending ints
      TIMSK |= _BV(TOIE0);
      break;
    default:
      break;
    }/*** switch ***/
}

void timer0_int_dis(timer0_int_enum timer0_int)
{
  switch(timer0_int)
    {
    case TIMER0_OVERFLOW_INTERRUPT:
      TIMSK &= ~_BV(TOIE0);
      break;
    default:
      break;
    }/*** switch ***/
}

void set_timer1_wg_mode(timer1_wg_mode_enum wgm)
{
  TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));

  switch(wgm)
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
      TCCR1B |= (_BV(WGM13) | _BV(WGM13));
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

  switch(mode)
    {
    case NORMAL_PORT_OPERATION:
      break;
    case TOGGLE:
      TCCR1A |= _BV(COM1A0);      
      break;
    case CLEAR:
      TCCR1A |= _BV(COM1A1);
      break;
    case SET:
      TCCR1A |= (_BV(COM1A1) | _BV(COM1A0));      
      break;
    default:
      break;
    }/*** switch ***/
}

void set_timer1_OC1B_mode(timer1_compare_output_mode_enum mode)
{
  TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));  

  switch(mode)
    {
    case NORMAL_PORT_OPERATION:
      break;
    case TOGGLE:
      TCCR1A |= _BV(COM1B0);      
      break;
    case CLEAR:
      TCCR1A |= _BV(COM1B1);
      break;
    case SET:
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
  switch(timer1_int)
    {
	case TIMER1_INPUT_CAPTURE_INTERRUPT:
	  TIFR |= 1<<ICF1;   //clear pending ints
      TIMSK |= 1<<TICIE1;
      break;
    case TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
      TIMSK |= 1<<OCIE1A;
      break;
    case TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
      TIMSK |= 1<<OCIE1B;
      break;
	case TIMER1_OVERFLOW_INTERRUPT:
	  TIFR |= 1<<TOV1;   //clear pending ints
      TIMSK |= 1<<TOIE1;
      break;
    default:
      break;
    }/*** switch ***/
}

void timer1_int_dis(timer1_int_enum timer1_int)
{
  switch(timer1_int)
    {
    case TIMER1_INPUT_CAPTURE_INTERRUPT:
      TIMSK &= ~(1<<TICIE1);
      break;
    case TIMER1_OUTPUT_COMPARE_A_MATCH_INTERRUPT:
      TIMSK &= ~(1<<OCIE1A);
      break;
    case TIMER1_OUTPUT_COMPARE_B_MATCH_INTERRUPT:
      TIMSK &= ~(1<<OCIE1B);
      break;
    case TIMER1_OVERFLOW_INTERRUPT:
      TIMSK &= ~(1<<TOIE1);
      break;
    case TIMER1_ALL_INTERRUPTS:
      TIMSK &= ~((1<<TICIE1) | (1<<OCIE1A) | (1<<OCIE1B) | (1<<TOIE1));
      break;
    default:
      break;
    }/*** switch ***/
}

void set_timer2_wg_mode(timer2_wg_mode_enum wgm)
{
  uint8_t tmp = TCCR2;
  
  tmp &= ~(_BV(WGM20) | _BV(WGM21));

  switch(wgm)
    {
    case T2_WG_NORMAL:
      break;
    case T2_WG_PWM_PHASE_CORRECT:
      tmp |=  _BV(WGM20);
      break;
    case T2_WG_CTC:
      tmp |=  _BV(WGM21);
      break;
    case T2_WG_FAST_PWM:
      tmp |= (_BV(WGM20) | _BV(WGM21));
      break;
    }/*** switch ***/
  
  TCCR2 = tmp;
}

void set_timer2_clock_source(timer2_clock_select_enum clock_select)
{
  uint8_t tmp = TCCR2;
  tmp &= ~0x07;
  tmp |= (clock_select & 0x07);
  TCCR2 = tmp;
}

void timer2_int_ena(timer2_int_enum timer2_int)
{
  switch(timer2_int)
    {
    case TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT:
      TIFR |= _BV(OCF2);   //clear pending ints
      TIMSK |= _BV(OCIE2);
      break;
    case TIMER2_OVERFLOW_INTERRUPT:
      TIFR |= _BV(TOV2);   //clear pending ints
      TIMSK |= _BV(TOIE2);
      break;
    default:
      break;
    }/*** switch ***/
}

void timer2_int_dis(timer2_int_enum timer2_int)
{
  switch(timer2_int)
    {
    case TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT:
      TIMSK &= ~_BV(OCIE2);
      break;
    case TIMER2_OVERFLOW_INTERRUPT:
      TIMSK &= ~_BV(TOIE2);
      break;
    case TIMER2_ALL_INTERRUPTS:
      TIMSK &= ~(_BV(OCF2) | _BV(TOIE2));
      break;
    default:
      break;
    }/*** switch ***/
}

