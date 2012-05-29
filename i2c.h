/*
 * i2c.h
 *
 *  Created on: 24.05.2011
 *      Author: krichevskiy
 */

#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

#include <stdbool.h>

#define I2C_FREQ_MAX_KHZ (F_CPU/16/1000)

typedef uint8_t i2c_state_t;

typedef enum {
	I2C_MASTER_START = 0x08,
	I2C_MASTER_REPSTART = 0x10,
	I2C_MASTER_SLAW_ACK = 0x18,
	I2C_MASTER_SLAW_NACK = 0x20,
	I2C_MASTER_TXDATA_ACK = 0x28,
	I2C_MASTER_TXDATA_NACK = 0x30,
	I2C_MASTER_SLAR_ACK = 0x40,
	I2C_MASTER_SLAR_NACK = 0x48,
	I2C_MASTER_RXDATA_ACK = 0x50,
	I2C_MASTER_RXDATA_NACK = 0x58,
	I2C_MASTER_ARB_LOST = 0x38
} i2c_status_code_enum;

typedef enum {
	I2C_DIV_1 = 0,
	I2C_DIV_4 = 1,
	I2C_DIV_16 = 2,
	I2C_DIV_64 = 3,
} i2c_prescaler_enum;

inline void i2c_set_prescaler(i2c_prescaler_enum presc)
{
	register unsigned char tmp = TWSR & ~((1 << TWPS1) | (1 << TWPS0));
	TWSR = tmp | (presc << TWPS0);
}

inline void i2c_set_bitrate_div(unsigned char div)
{
	TWBR = div;
}

inline void i2c_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
}

inline void i2c_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

inline void i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) |	(1<<TWEN);
}

inline void i2c_slaW(unsigned char sla)
{
	i2c_write(sla);
}

inline void i2c_slaR(unsigned char sla)
{
	i2c_write(sla | 0x01);
}

inline void i2c_read_nack(void)
{
	TWCR = (1<<TWINT) |	(1<<TWEN);
}

inline void i2c_read_ack(void)
{
	TWCR = (1<<TWINT) |	(1<<TWEN) | (1 << TWEA);
}

inline unsigned char i2c_get_byte(void)
{
	return TWDR;
}

inline void i2c_wait(void)
{
	while (!(TWCR & (1<<TWINT)))
		;
}

inline i2c_state_t i2c_read_state_immediate(void)
{
	return (TWSR & 0xF8);
}

inline i2c_state_t i2c_read_state_wait(void)
{
	i2c_wait();
	return (TWSR & 0xF8);
}

extern bool i2c_set_clock_freq(unsigned long f_kHz);

#endif /* I2C_H_ */
