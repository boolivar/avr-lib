/*
 * adc.c
 *
 *  Created on: 22.09.2010
 *      Author: krichevskiy
 */
#include "adc.h"
#include "macros.h"

void adc_init(adc_voltage_reference_enum vRef,
		      adc_result_adjust_enum      adj,
		      adc_prescaler_enum          div)
{
	ADMUX = (vRef << REFS0) | (adj << ADLAR);
	ADCSRA = _BV(ADEN) | (div << ADPS0);
}

void adc_interrupt_enable(void)
{
	bit_set(ADCSRA, ADIE);
}

void adc_interrupt_disable(void)
{
	bit_clr(ADCSRA, ADIE);
}

/*
 *  ������� ���� ��������� ����� ��� �
 *  ���������� ���������� ��������.
 */
uint16_t adc_get_value(adc_channel_enum channel)
{
#if 0
	unsigned short value;
	register unsigned char  tmp;

	tmp = ADMUX & 0xf0;     // ����� ������
	ADMUX = tmp | channel;  //

	bit_set(ADCSRA, ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);

	value = ADCL;
	value |= (ADCH << 8);

	return value;
#endif
	adc_start_conversion(channel);
	while (!adc_is_conversion_complete())
		;
	return adc_get_conversion_result();
}

void adc_start_conversion(adc_channel_enum channel) {
	register unsigned char  tmp;

	tmp = ADMUX & 0xf0;
	ADMUX = tmp | channel;

	bit_set(ADCSRA, ADSC);
}

uint16_t adc_get_conversion_result() {
	uint16_t value;
	value = ADCL;
	value |= (ADCH << 8);
	return value;
}

/*
 *  ������� ��������� ���� ��� ���
 *  ������ �� ��������� ����������.
 */
void adc_start_interrupted_conversion(adc_channel_enum channel)
{
	register unsigned char  tmp;

	tmp = ADMUX & 0xf0;     // ����� ������
	ADMUX = tmp | channel;  //

	bit_set(ADCSRA, ADSC);
}
