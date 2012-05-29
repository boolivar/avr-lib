/*
 * spi.h
 *
 *  Created on: 01.03.2012
 *      Author: krichevskiy
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define MOSI_DDR DDRC
#define MOSI_PORT PORTC
#define MOSI_BIT 2

#define MISO_DDR DDRC
#define MISO_PIN PINC
#define MISO_BIT 3

#define CLK_DDR DDRC
#define CLK_PORT PORTC
#define CLK_BIT 4

#define SS_DDR DDRC
#define SS_PORT PORTC
#define SS_BIT 5

#define mosi_high()	(MOSI_PORT |= (1 << MOSI_BIT))
#define mosi_low()	(MOSI_PORT &=~(1 << MOSI_BIT))
#define miso_in()	(MISO_PIN & (1 << MISO_BIT))

#define clk_high()	(CLK_PORT |= (1 << CLK_BIT))
#define clk_low()	(CLK_PORT &= ~(1 << CLK_BIT))

#define ss_low() (SS_PORT &=~ (1 << SS_BIT))
#define ss_high()(SS_PORT |= (1 << SS_BIT))

/* Redefine for proper spi operation */
#define clk_write_latch() clk_high()
#define clk_read_latch() clk_low()
#define spi_chip_select() ss_low()
#define spi_chip_unselect() ss_high()

extern uint8_t spi_exchange_byte(uint8_t data);
extern void spi_init(void);

#endif /* SPI_H_ */
