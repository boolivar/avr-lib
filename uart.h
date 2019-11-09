#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

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
	CS_8_BIT = 3,
	CS_9_BIT
} char_size_enum;

typedef enum {
	UDR_EMPTY_INT = 0x80,
	TX_COMPLETE_INT = 0x40,
	RX_COMPLETE_INT = 0x20
} uart_int_enum;

typedef enum {
	TRANSMITTER = 0x08,
	RECEIVER = 0x10,
} uart_rxtx_enum;

inline uint8_t uart_int_is_enabled(uart_int_enum int_mask) {
	return UCSR0B & int_mask;
}

/**
 * use uart_int_enum values for mask
 */
inline void uart_int_ena(uint8_t int_mask) {
	UCSR0B |= int_mask;
}

/**
 * use uart_int_enum values for mask
 */
inline void uart_int_dis(uint8_t int_mask) {
	UCSR0B &=~ int_mask;
}

/**
 * use uart_rxtx_enum values for mask
 */
inline void uart_rxtx_enable(uint8_t rxtx_mask) {
	UCSR0B |= rxtx_mask;
}

/**
 * use uart_rxtx_enum values for mask
 */
inline void uart_rxtx_disable(uint8_t rxtx_mask) {
	UCSR0B &=~ rxtx_mask;
}

extern void uart_set_baudrate(unsigned long baudrate);
extern void uart_set_mode(parity_mode_enum parity, stop_bits_enum stop, char_size_enum char_size);
extern void init_dbg_uart(void);

#endif
