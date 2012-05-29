/*
 * i2c.c
 *
 *  Created on: 24.05.2011
 *      Author: krichevskiy
 */

#include <math.h>
#include <stdlib.h>

#include "i2c.h"

static unsigned char prescalers[] = {1, 4, 16, 64};

bool i2c_set_clock_freq(unsigned long f_kHz)
{
	unsigned char i;
	unsigned char presc_index_selected;
	unsigned int twbr_x_prescaler;
	unsigned char twbr[sizeof(prescalers)/sizeof(prescalers[0])];
	unsigned long freq[sizeof(prescalers)/sizeof(prescalers[0])];
	unsigned long delta_min;


	if (f_kHz == 0)
	{
		// zero frequency
		return false;
	}

	if (f_kHz >= I2C_FREQ_MAX_KHZ)
	{
		// frequency so high
		return false;
	}

	f_kHz *= 1000;

	// TWBR * PRESCALER
	twbr_x_prescaler = floor((float)F_CPU/2/f_kHz - 8 + 0.5);

	for (i = 0; i < sizeof(twbr)/sizeof(twbr[0]); ++i)
	{
		twbr[i] = floor((float)twbr_x_prescaler/prescalers[i] + 0.5);  // calc TWBR for each prescaler
		freq[i] = F_CPU/(16 + 2*twbr[i]*prescalers[i]);  // calc frequency
		freq[i] = labs(freq[i] - f_kHz);
	}

	// find smallest delta
	presc_index_selected = 0;
	delta_min = freq[0];
	for (i = 1; i < sizeof(twbr)/sizeof(twbr[0]); ++i)
	{
		if (freq[i] < delta_min)
		{
			delta_min = freq[i];
			presc_index_selected = i;
		}
	}

	// set values with smallest freq delta
	i2c_set_bitrate_div(twbr[presc_index_selected]);
	i2c_set_prescaler(presc_index_selected);

	return true;
}
