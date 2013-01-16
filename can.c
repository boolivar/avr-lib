/*
 * can.c
 *
 *  Created on: 28.12.2012
 *      Author: krichevskiy
 */


#include <avr/io.h>

#include "can.h"

void can_init_br(can_baudrate_enum br, can_sample_point_enum smp);

void can_init_br_16tq(can_baudrate_16tq_enum br, can_sample_point_enum smp);
void can_init_br_12tq(can_baudrate_12tq_enum br, can_sample_point_enum smp);
void can_init_br_8tq(can_baudrate_8tq_enum br, can_sample_point_enum smp);

void can_init_tq(uint8_t brp, uint8_t tprs, uint8_t tphs1, uint8_t tphs2,
		uint8_t tsjw, can_sample_point_enum smp);

void can_set_mode(can_mode_enum mode);
void can_reset_mobs(void);

void can_mob_select(uint8_t mob);
void can_mob_set_mode(mob_mode_enum mode);
void can_mob_refresh_mode(void);

void can_mob_set_id(uint16_t id, can_bits_enum bits);
void can_mob_set_ext_id(uint32_t id, can_bits_enum bits);
void can_mob_set_ext_id_mask(uint32_t mask, can_mask_bits_enum bits);

void can_mob_set_remote_frame_data_len(uint8_t len);
void can_mob_put_message_data(const uint8_t* data, uint8_t len);
uint8_t can_mob_get_data(uint8_t *data);
uint32_t can_mob_get_msg_id(void);

void can_disable_interrupts(can_ints_enum ints);
void can_enable_interrupts(can_ints_enum ints);

void can_enable_mob_interrupt(uint8_t mob);
void can_enable_mob_interrupts_mask(uint16_t mob_mask);

void can_disable_mob_interrupt(uint8_t mob);
void can_disable_mob_interrupts_mask(uint16_t mob_mask);

void can_init_br(can_baudrate_enum br, can_sample_point_enum smp) {
#if (F_CPU == 16000000UL)
#	define can_init_br_internal(a,b) can_init_br_16tq(a,b)
#elif (F_CPU == 12000000UL)
#	define can_init_br_internal(a,b) can_init_br_12tq(a,b)
#else
#	define can_init_br_internal(a,b) can_init_br_8tq(a,b)
#endif
	can_init_br_internal(br,smp);
}

void can_init_br_16tq(can_baudrate_16tq_enum br, can_sample_point_enum smp) {
	can_init_tq(br, 7, 4, 4, 1, smp);
}

void can_init_br_12tq(can_baudrate_12tq_enum br, can_sample_point_enum smp) {
	can_init_tq(br, 5, 3, 3, 1, smp);
}

void can_init_br_8tq(can_baudrate_8tq_enum br, can_sample_point_enum smp) {
	can_init_tq(br, 3, 2, 2, 1, smp);
}

void can_init_tq(uint8_t brp, uint8_t tprs, uint8_t tphs1, uint8_t tphs2,
		uint8_t tsjw, can_sample_point_enum smp) {
	CANBT1 = (brp << BRP0);
	CANBT2 = ((tsjw-1) << SJW0) | ((tprs-1) << PRS0);
	CANBT3 = ((tphs2-1) << PHS20) | ((tphs1-1) << PHS10) | smp;
}

void can_set_mode(can_mode_enum mode) {
	uint8_t cangcon;

	cangcon = CANGCON;

	cangcon &= ~(_BV(LISTEN) | _BV(ENASTB));
	switch (mode) {
	case CAN_ENABLE:
		cangcon = _BV(ENASTB);
		break;

	case CAN_LISTEN:
		cangcon = (_BV(LISTEN) | _BV(ENASTB));
		break;

	case CAN_STANDBY:
	default:
		CANGCON |= _BV(ABRQ);
		loop_until_bit_is_set(CANGSTA, RXBSY)
			;
		break;
	}

	CANGCON = cangcon;

	if (cangcon & _BV(ENASTB)) {
		loop_until_bit_is_set(CANGSTA, ENFG);
	}else {
		loop_until_bit_is_clear(CANGSTA, ENFG);
	}
}

void can_reset_mobs() {
	uint8_t i;

	for (i = 0; i < 15; ++i) {
		can_mob_select(i);
		can_mob_set_mode(CAN_MOB_MODE_DISABLED);
		can_mob_clear_state();
	}
}

int8_t can_find_free_mob() {
	uint8_t i;
	uint8_t mask;

	mask = 0x01;
	for (i = 0; i < 8; ++i) {
		if (CANEN2 & mask) {
			return i;
		}
		mask <<= 1;
	}

	mask = 0x01;
	for (; i < 15; ++i) {
		if (CANEN1 & mask) {
			return i;
		}
		mask <<= 1;
	}

	return -1;
}

int8_t can_find_next_free_mob(uint8_t i) {
	uint16_t mask;

	mask = _BV(i);
	for (; i < 15; ++i) {
		if ((CANEN2 & mask) || (CANEN1 & (mask >> 8))) {
			return i;
		}
		mask <<= 1;
	}

	return -1;
}

void can_mob_select(uint8_t mob) {
	CANPAGE = (mob << MOBNB0);
}

void can_mob_set_mode(mob_mode_enum mode) {
	CANCDMOB = (CANCDMOB & ~(_BV(CONMOB1) | _BV(CONMOB0))) | (mode << CONMOB0);
}

void can_mob_refresh_mode() {
	volatile uint8_t cancdmob;

	cancdmob = CANCDMOB;
	CANCDMOB = cancdmob;

	can_mob_clear_state();
}

void can_mob_set_id(uint16_t id, can_bits_enum bits) {
	id <<= IDT18;

	CANIDT4 = bits;
	CANIDT2 = id;
	CANIDT1 = (id >> 8);

	CANCDMOB &= ~_BV(IDE);
}

void can_mob_set_ext_id(uint32_t id, can_bits_enum bits) {
	id <<= IDT0;

	CANIDT4 = id | bits;
	CANIDT3 = (id >> 8);
	CANIDT2 = (id >> 16);
	CANIDT1 = (id >> 24);

	CANCDMOB |= _BV(IDE);
}

void can_mob_set_id_mask(uint16_t mask, can_mask_bits_enum bits) {
	mask <<= IDMSK18;

	CANIDM4 = bits;
	CANIDM2 = mask;
	CANIDM1 = (mask >> 8);
}

void can_mob_set_ext_id_mask(uint32_t mask, can_mask_bits_enum bits) {
	mask <<= IDMSK0;

	CANIDM4 = mask | bits;
	CANIDM3 = (mask >> 8);
	CANIDM2 = (mask >> 16);
	CANIDM1 = (mask >> 24);
}

void can_mob_set_remote_frame_data_len(uint8_t len) {
	CANCDMOB = (CANCDMOB & ~0x0f) | len;
	CANIDT4 |= _BV(RTRTAG);
}

void can_mob_put_message_data(const uint8_t* data, uint8_t len) {
	uint8_t i;

	CANCDMOB = (CANCDMOB & ~0x0f) | len;
	CANIDT4 &= ~_BV(RTRTAG);

	for (i = 0; i < len; ++i) {
		CANMSG = data[i];
	}
}

uint8_t can_mob_get_data(uint8_t *data) {
	uint8_t len;

	len = (CANCDMOB & 0x0f);

	for (uint8_t i = 0; i < len; ++i) {
		data[i] = CANMSG;
	}

	return len;
}

uint32_t can_mob_get_msg_id() {
	uint32_t id;

	id = ((uint32_t)CANIDT1 << 24) | ((uint32_t)CANIDT2 << 16) | ((uint16_t)CANIDT3 << 8) | CANIDT4;

	return (id >> IDT0);
}

void can_enable_interrupts(can_ints_enum ints) {
	CANGIE |= ints;
}

void can_disable_interrupts(can_ints_enum ints) {
	CANGIE &= ~ints;
}

void can_enable_mob_interrupt(uint8_t mob) {
	can_enable_mob_interrupts_mask(1 << mob);
}

void can_disable_mob_interrupt(uint8_t mob) {
	can_disable_mob_interrupts_mask(1 << mob);
}

void can_enable_mob_interrupts_mask(uint16_t mob_mask) {
	CANIE2 |= mob_mask;
	CANIE1 |= ((mob_mask >> 8) & 0x7f);
}

void can_disable_mob_interrupts_mask(uint16_t mob_mask) {
	CANIE2 &= ~mob_mask;
	CANIE1 &= ~((mob_mask >> 8) & 0x7f);
}
