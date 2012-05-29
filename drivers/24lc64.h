/*
 * 24lc64.h
 *
 *  Created on: 30.05.2011
 *      Author: krichevskiy
 */

#ifndef _24LC64_H_
#define _24LC64_H_

#include "../i2c.h"

typedef unsigned int e2p_addr_t;

typedef i2c_state_t lc_err_t;

inline void init_24lc64(void)
{
	i2c_set_clock_freq(200);
}

extern lc_err_t write_byte(e2p_addr_t address, unsigned char byte);
extern lc_err_t read_byte(e2p_addr_t address, unsigned char *byte);
extern lc_err_t write_page(e2p_addr_t address, const unsigned char *data, unsigned char size);
extern lc_err_t read_page(e2p_addr_t address, unsigned char *data, unsigned char size);

#endif /* _24LC64_H_ */
