/*
 * ds3231.c
 *
 *  Created on: 15.02.2013
 *      Author: krichevskiy
 */

#include "i2c.h"

#define _I2C_RATE_VAL (F_CPU/(2*200000*1) - 8/1)//0xf7
#define _DS3231_ADDR 0xd0

void ds3231_init(void);
uint8_t ds3231_read(uint8_t addr, uint8_t* data);

void ds3231_init() {
	i2c_set_bitrate_div(_I2C_RATE_VAL);
	i2c_set_prescaler(I2C_DIV_1);
}

uint8_t ds3231_read(uint8_t addr, uint8_t* data) {
	uint8_t ecode;

	do {
		i2c_start();
		ecode = i2c_read_state_wait();
		if ((ecode != I2C_MASTER_START) && (ecode != I2C_MASTER_REPSTART))
			break;

		i2c_slaW(_DS3231_ADDR);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(addr);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_start();
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_REPSTART)
			break;

		i2c_slaR(_DS3231_ADDR);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_SLAR_ACK)
			break;

		i2c_read_nack();
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_RXDATA_NACK)
			break;

		*data = i2c_get_byte();
		ecode = 0;
	} while(0);

	i2c_stop();

	return ecode;
}

uint8_t ds3231_write(uint8_t addr, uint8_t data) {
	uint8_t ecode;

	do {
		i2c_start();
		ecode = i2c_read_state_wait();
		if ((ecode != I2C_MASTER_START) && (ecode != I2C_MASTER_REPSTART))
			break;

		i2c_slaW(_DS3231_ADDR);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(addr);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_TXDATA_ACK)
			break;

		i2c_write(data);
		ecode = i2c_read_state_wait();
		if (ecode != I2C_MASTER_TXDATA_ACK)
			break;

		ecode = 0;
	} while(0);

	i2c_stop();

	return ecode;
}
