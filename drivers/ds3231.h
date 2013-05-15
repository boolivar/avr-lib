/*
 * ds3231.h
 *
 *  Created on: 15.02.2013
 *      Author: krichevskiy
 */

#ifndef DS3231_H_
#define DS3231_H_

#define DS3231_SECONDS_SUBADDR 0
#define DS3231_MINUTES_SUBADDR 1
#define DS3231_HOURS_SUBADDR 2

extern void ds3231_init(void);
extern uint8_t ds3231_read(uint8_t addr, uint8_t* data);
extern uint8_t ds3231_write(uint8_t addr, uint8_t data);

#endif /* DS3231_H_ */
