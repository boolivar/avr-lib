/*
 * adc.h
 *
 *  Created on: 22.09.2010
 *      Author: krichevskiy
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr\io.h>

typedef enum
{
	AREF = 0,
	AVCC = 1,
	INTERNAL_2_56V = 3,
} adc_voltage_reference_enum;

typedef enum
{
	ADJ_RIGHT = 0,
	ADJ_LEFT  = 1,
} adc_result_adjust_enum;

typedef enum
{
	  ADC_DIV_2 = 1,
	  ADC_DIV_4 = 2,
	  ADC_DIV_8 = 3,
	  ADC_DIV_16 = 4,
	  ADC_DIV_32 = 5,
	  ADC_DIV_64 = 6,
	  ADC_DIV_128 = 7,
} adc_prescaler_enum;

typedef enum
{
	ADC0 = 0,
	ADC1 = 1,
	ADC2 = 2,
	ADC3 = 3,
	ADC4 = 4,
	ADC5 = 5,
	ADC6 = 6,
	ADC7 = 7,
	BG_1_3V = 14,
	VSS     = 15,
} adc_channel_enum;

#define adc_is_conversion_complete() ((ADCSRA & _BV(ADSC)) == 0)

extern void adc_init(adc_voltage_reference_enum vRef,
		             adc_result_adjust_enum      adj,
		             adc_prescaler_enum          div);
void adc_start_conversion(adc_channel_enum channel);
extern uint16_t adc_get_conversion_result(void);
extern uint16_t adc_get_value(adc_channel_enum channel);
extern void adc_start_interrupted_conversion(adc_channel_enum channel);

#endif /* ADC_H_ */
