/*
 * can.h
 *
 *  Created on: 09.01.2013
 *      Author: krichevskiy
 */

#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>

#define CAN_BRP_16TQ(kbps) (F_CPU/1000/16/kbps - 1)
#define CAN_BRP_12TQ(kbps) (F_CPU/1000/12/kbps - 1)
#define CAN_BRP_8TQ(kbps) (F_CPU/1000/8/kbps - 1)

#define can_mob_is_tx_state_ok(state) (state & _BV(TXOK))
#define can_mob_is_rx_state_ok(state) (state & _BV(RXOK))

#define can_mob_is_tx_ok() can_mob_is_tx_state_ok(CANSTMOB)
#define can_mob_is_rx_ok() can_mob_is_rx_state_ok(CANSTMOB)

#if (F_CPU == 16000000UL)
#	define CAN_BRP_AUTO(a) CAN_BRP_16TQ(a)
#elif (F_CPU == 12000000UL)
#	define CAN_BRP_AUTO(a) CAN_BRP_12TQ(a)
#else
#	define CAN_BRP_AUTO(a) CAN_BRP_8TQ(a)
#endif

typedef enum {
	CAN_BR_100_KBPS_16TQ = CAN_BRP_16TQ(100),
	CAN_BR_125_KBPS_16TQ = CAN_BRP_16TQ(125),
	CAN_BR_200_KBPS_16TQ = CAN_BRP_16TQ(200),
	CAN_BR_250_KBPS_16TQ = CAN_BRP_16TQ(250),
	CAN_BR_500_KBPS_16TQ = CAN_BRP_16TQ(500),
	CAN_BR_1000_KBPS_16TQ = CAN_BRP_16TQ(1000),
} can_baudrate_16tq_enum;

typedef enum {
	CAN_BR_100_KBPS_12TQ = CAN_BRP_12TQ(100),
	CAN_BR_125_KBPS_12TQ = CAN_BRP_12TQ(125),
	CAN_BR_200_KBPS_12TQ = CAN_BRP_12TQ(200),
	CAN_BR_250_KBPS_12TQ = CAN_BRP_12TQ(250),
	CAN_BR_500_KBPS_12TQ = CAN_BRP_12TQ(500),
	CAN_BR_1000_KBPS_12TQ = CAN_BRP_12TQ(1000),
} can_baudrate_12tq_enum;

typedef enum {
	CAN_BR_100_KBPS_8TQ = CAN_BRP_8TQ(100),
	CAN_BR_125_KBPS_8TQ = CAN_BRP_8TQ(125),
	CAN_BR_200_KBPS_8TQ = CAN_BRP_8TQ(200),
	CAN_BR_250_KBPS_8TQ = CAN_BRP_8TQ(250),
	CAN_BR_500_KBPS_8TQ = CAN_BRP_8TQ(500),
	CAN_BR_1000_KBPS_8TQ = CAN_BRP_8TQ(1000),
} can_baudrate_8tq_enum;

typedef enum {
	CAN_BR_100_KBPS = CAN_BRP_AUTO(100),
	CAN_BR_125_KBPS = CAN_BRP_AUTO(125),
	CAN_BR_200_KBPS = CAN_BRP_AUTO(200),
	CAN_BR_250_KBPS = CAN_BRP_AUTO(250),
	CAN_BR_500_KBPS = CAN_BRP_AUTO(500),
	CAN_BR_1000_KBPS = CAN_BRP_AUTO(1000),
} can_baudrate_enum;

typedef enum {
	CAN_STANDBY,
	CAN_ENABLE,
	CAN_LISTEN
} can_mode_enum;

typedef enum {
	CAN_SMP_ONCE,
	CAN_SMP_THREE_POINT
} can_sample_point_enum;

typedef enum {
	CAN_MOB_MODE_DISABLED,
	CAN_MOB_MODE_TX,
	CAN_MOB_MODE_RX,
	CAN_MOB_MODE_FRAME_BUFFER,
} mob_mode_enum;

typedef enum {
	CAN_RB0_BIT = 0x01,
	CAN_RB1_BIT = 0x02,
	CAN_RTR_BIT = 0x04
} can_bits_enum;

typedef enum {
	CAN_IDE_MSK_BIT = 0x01,
	CAN_RTR_MSK_BIT = 0x04
} can_mask_bits_enum;

typedef enum {
	CAN_INT_TIMER_OVERRUN = 0x01,
	CAN_INT_GENERAL_ERRORS = 0x02,
	CAN_INT_FRAME_BUFFER = 0x04,
	CAN_INT_MOB_ERRORS = 0x08,
	CAN_INT_TXOK = 0x10,
	CAN_INT_RXOK = 0x20,
	CAN_INT_BUS_OFF = 0x40,
	CAN_INT_ISR = 0x80
} can_ints_enum;

extern void can_init_br(can_baudrate_enum br, can_sample_point_enum smp);

extern void can_init_br_16tq(can_baudrate_16tq_enum br, can_sample_point_enum smp);
extern void can_init_br_12tq(can_baudrate_12tq_enum br, can_sample_point_enum smp);
extern void can_init_br_8tq(can_baudrate_8tq_enum br, can_sample_point_enum smp);

extern void can_init_tq(uint8_t brp, uint8_t tprs, uint8_t tphs1, uint8_t tphs2,
		uint8_t tsjw, can_sample_point_enum smp);

static inline void can_reset(void) {
	CANGCON |= _BV(SWRES);
	loop_until_bit_is_clear(CANGCON, SWRES);
}

extern void can_reset_mobs(void);
extern void can_set_mode(can_mode_enum mode);

extern void can_mob_select(uint8_t mob);

static inline uint8_t can_mob_get_state(void) {
	return CANSTMOB;
}

static inline void can_mob_clear_state(void) {
	CANSTMOB = 0;
}

extern void can_mob_set_mode(mob_mode_enum mode);
extern void can_mob_refresh_mode(void);

extern void can_mob_set_id(uint16_t id, can_bits_enum bits);
extern void can_mob_set_ext_id(uint32_t id, can_bits_enum bits);
extern void can_mob_set_ext_id_mask(uint32_t mask, can_mask_bits_enum bits);

extern void can_mob_set_remote_frame_data_len(uint8_t len);
extern void can_mob_put_message_data(const uint8_t* data, uint8_t len);
extern uint8_t can_mob_get_data(uint8_t *data);
extern void can_mob_set_data(const uint8_t *data, uint8_t len);
extern uint32_t can_mob_get_msg_id(void);

#endif /* CAN_H_ */
