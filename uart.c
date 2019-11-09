
#include "uart.h"

#include <stdio.h>

void uart_set_baudrate(unsigned long baudrate);
void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size);
static int uart_putchar(char c, FILE *stream);

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void init_dbg_uart(void) {
	UCSR0B |= _BV(TXEN0);
	stdout = &uart_str;
}

void uart_set_baudrate(unsigned long baudrate) {
	UCSR0A |= _BV(U2X0);
	UBRR0 = (F_CPU / (8UL * baudrate)) - 1;
}

void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size) {
	uint8_t ucsrc = (parity << UPM00) | (stop << USBS0);

	if (char_size == CS_9_BIT) {
		ucsrc |= _BV(UCSZ01) | _BV(UCSZ00);
		UCSR0B |= _BV(UCSZ02);
	}else {
		ucsrc |= (char_size << UCSZ00);
	}

	UCSR0C = ucsrc;
}

/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);

	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;

	return 0;
}
