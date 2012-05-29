/*
 * spi.c
 *
 *  Created on: 01.03.2012
 *      Author: krichevskiy
 */


#include "spi.h"

#include <avr/io.h>
#include <util/delay.h>

void spi_init(void) {
	MOSI_DDR |= (1 << MOSI_BIT);
	MISO_DDR  &=~ (1 << MISO_BIT);
	SS_DDR |= (1 << SS_BIT);
	CLK_DDR |= (1 << CLK_BIT);
}

uint8_t spi_exchange_byte(uint8_t data) {
	unsigned char i;

	for (i = 0; i < 8; ++i) {
		if (data & 0x80) {
			mosi_high();
		}else {
			mosi_low();
		}

		data <<= 1;
		_delay_us(1);
		clk_write_latch();
		_delay_us(1);

		if (miso_in()) {
			data |= 0x01;
		}
		clk_read_latch();
	}

	return data;
}
