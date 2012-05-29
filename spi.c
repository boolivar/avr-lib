/*
 * spi.c
 *
 *  Created on: 22.09.2010
 *      Author: krichevskiy
 */

#include "spi.h"
#include "macros.h"

void spi_init(spi_mode_enum          spi_mode,
		      spi_data_order_enum    data_order,
		      spi_sampling_mode_enum sampling_mode,
		      spi_clock_rate_enum    clock_rate)
{
	SPSR = (clock_rate >> 2);
	SPCR =   _BV(SPE)
		   | (spi_mode << MSTR)
		   | (data_order << DORD)
		   | (sampling_mode << CPHA)
		   | ((clock_rate & 0x03) << SPR0);
}

void spi_enable_interrupt(void)
{
	bit_set(SPCR, SPIE);
}

void spi_disable_interupt(void)
{
	bit_clr(SPCR, SPIE);
}

unsigned char spi_write_byte (unsigned char byte)
{
	SPDR = byte;
	loop_until_bit_is_set(SPSR, SPIF);

	return SPDR;
}

void spi_write_sequence (const unsigned char *bytes, unsigned char length)
{
	while (length)
	{
		SPDR = *bytes++;
		loop_until_bit_is_set(SPSR, SPIF);

		--length;
	}
}

