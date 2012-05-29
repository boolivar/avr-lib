/*
 * spi.h
 *
 *  Created on: 22.09.2010
 *      Author: krichevskiy
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr\io.h>

typedef enum
{
	MSB_FIRST = 0,
	LSB_FIRST = 1,
} spi_data_order_enum;

typedef enum
{
	SLAVE  = 0,
	MASTER = 1,
} spi_mode_enum;

typedef enum
{
	LEAD_RISE = 0,
	TRAIL_FALL = 1,
	LEAD_FALL = 2,
	TRAIL_RISE = 3,
} spi_sampling_mode_enum;

typedef enum
{
	SPI_DIV_4 = 0,
	SPI_DIV_16 = 1,
	SPI_DIV_64 = 2,
	SPI_DIV_128 = 3,
	SPI_DIV_2 = 4,
	SPI_DIV_8 = 5,
	SPI_DIV_32 = 6,
} spi_clock_rate_enum;

inline void spi_write_interrupted (unsigned char byte)
{
	SPDR = byte;
}

inline unsigned char spi_read_interrupted (void)
{
	return SPDR;
}

extern void spi_init(spi_mode_enum          spi_mode,
		             spi_data_order_enum    data_order,
		             spi_sampling_mode_enum sampling_mode,
		             spi_clock_rate_enum    clock_rate);
extern unsigned char spi_write_byte (unsigned char byte);
extern void spi_write_interrupted (unsigned char byte);
extern void spi_write_sequence (const unsigned char *bytes, unsigned char length);
extern unsigned char spi_write_byte (unsigned char byte);

#endif /* SPI_H_ */
