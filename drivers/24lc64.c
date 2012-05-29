/*
 * 24lc64.c
 *
 *  Created on: 30.05.2011
 *      Author: krichevskiy
 */

#include "24lc64.h"

lc_err_t write_byte(e2p_addr_t address, unsigned char byte)
{
	register lc_err_t err_code;

	// ѕустой внешний цикл дл€ простого
	// перехода к завершению по команде break.
	do {
		// ∆дем пока освободитс€ микросхема
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(0xa0);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);

		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(address >> 8);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(address);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(byte);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		err_code = 0;
	} while (0);

	i2c_stop();
	return err_code;
}

lc_err_t read_byte(e2p_addr_t address, unsigned char *byte)
{
	register lc_err_t err_code;

	do {
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(0xa0);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(address >> 8);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(address);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_start();
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_REPSTART)
			break;

		i2c_slaR(0xa0);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_SLAR_ACK)
			break;

		i2c_read_nack();
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_RXDATA_NACK)
			break;

		*byte = i2c_get_byte();
		err_code = 0;
	} while (0);

	i2c_stop();
	return err_code;
}

lc_err_t write_page(e2p_addr_t address, const unsigned char *data, unsigned char size)
{
	register lc_err_t err_code;

	do {
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(0xa0);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(address >> 8);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(address);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		while (size)
		{
			i2c_write(*data++);
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_TXDATA_ACK)
				break;

			--size;
		}

		err_code = 0;
	} while(0);

	i2c_stop();
	return err_code;
}

lc_err_t read_page(e2p_addr_t address, unsigned char *data, unsigned char size)
{
	register lc_err_t err_code;

	do {
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(0xa0);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(address >> 8);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(address);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_start();
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_REPSTART)
			break;

		i2c_slaR(0xa0);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_SLAR_ACK)
			break;

		while (size > 1)
		{
			i2c_read_ack();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_RXDATA_ACK)
				break;

			*data++ = i2c_get_byte();
			--size;
		}
		if (err_code != I2C_MASTER_RXDATA_ACK)
			break;

		i2c_read_nack();
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_RXDATA_NACK)
			break;

		*data = i2c_get_byte();
		err_code = 0;
	} while(0);

	i2c_stop();
	return err_code;
}
