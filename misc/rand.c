#include "rand.h"

unsigned char rand_byte(void);
unsigned char rand_byte_strong(void);

unsigned char rand_byte()
{
	unsigned char result;
	unsigned char tmp;
	unsigned int i;

	enable_adc();

	for (i = 0; i < 8; ++i)
	{
		tmp = adc_get_value(0);
		result ^= (tmp << i);
	}

	disable_adc();

	return result;
}

unsigned char rand_byte_strong() {
	unsigned char result;
	unsigned char tmp;
	unsigned char i;

	enable_adc();

	i = 0;
	while (i < 8) {
		tmp = adc_get_value(0) & 0x01;
		tmp |= adc_get_value(0) << 1;

		if ((tmp & 0x03) == 0x01) {
			result ^= (tmp << i);
		}else if ((tmp & 0x03) == 0x02) {
			;
		}else
			continue;

		++i;
	}

	disable_adc();

	return result;
}