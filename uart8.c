/*
 * uart.c
 *
 *  Created on: 25.04.2013
 *      Author: krichevskiy
 */

#include "uart8.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(void);
void uart_set_baudrate(unsigned long baudrate);
void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size);
void uart_put_char(uint8_t c);

void uart_init() {
	uart_set_baudrate(UART_BAUD);
	uart_set_mode(UART_PARITY_MODE, UART_STOP_BITS, UART_CHAR_SIZE);
}

void uart_set_baudrate(unsigned long baudrate) {
	uint16_t brr_value;

	UCSRB &= ~(_BV(TXEN) | _BV(RXEN));
	brr_value = ((F_CPU/8)/ baudrate) - 1;

	UCSRA = _BV(U2X);
	UBRRH = (brr_value >> 8) & 0x0f;
	UBRRL = brr_value;
	UCSRB |= (_BV(TXEN) | _BV(RXEN));
}

void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size) {
	uint8_t ucsrc;

	UCSRB &= ~(_BV(TXEN) | _BV(RXEN));
	ucsrc = (parity << UPM0) | (stop << USBS);
	ucsrc |= (char_size << UCSZ0);
	UCSRC = ucsrc | _BV(URSEL);
	UCSRB |= (_BV(TXEN) | _BV(RXEN));
}

void uart_put_char(uint8_t c) {
	loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}
