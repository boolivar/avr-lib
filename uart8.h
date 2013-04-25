/*
 * uart.h
 *
 *  Created on: 25.04.2013
 *      Author: krichevskiy
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

#define enable_rx_interrupt() do{UCSRB |= _BV(RXCIE);}while(0)
#define disable_rx_interrupt() do{UCSRB &= ~_BV(RXCIE);}while(0)
#define enable_tx_interrupt() do{UCSRA = _BV(TXC); UCSRB |= _BV(TXCIE);}while(0)
#define disable_tx_interrupt() do{UCSRB &= ~_BV(TXCIE);}while(0)

typedef enum {
	PARITY_DISABLE = 0,
	PARITY_EVEN = 2,
	PARITY_ODD = 3
} parity_mode_enum;

typedef enum {
	STOP_1_BIT = 0,
	STOP_2_BITS = 1
} stop_bits_enum;

typedef enum {
	CS_5_BIT = 0,
	CS_6_BIT = 1,
	CS_7_BIT = 2,
	CS_8_BIT = 3
} char_size_enum;

#define UART_BAUD 9600
#define UART_PARITY_MODE PARITY_DISABLE
#define UART_STOP_BITS STOP_1_BIT
#define UART_CHAR_SIZE CS_8_BIT

extern void uart_init(void);
extern void uart_set_baudrate(unsigned long baudrate);
extern void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size);
extern void uart_put_char(uint8_t c);

#endif /* UART_H_ */
