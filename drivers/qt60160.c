
#include <stdio.h>
#include <inttypes.h>

#include <avr/pgmspace.h>
#include <util/delay.h>

#include "qt60160.h"
#include "i2c.h"

#define SUB_ADDR_KBD_DETECT_STATUS   1
#define SUB_ADDR_KBD_CAL        125
#define SUB_ADDR_KBD_SETUPS_WRITE_UNLOCK 130
#define SUB_ADDR_KBD_SETUPS     131

static prog_uint8_t kbd_setups[] =
{
	0x55, //Setups write-unlock. Write 0x55 immediately before writing setups

	//131-154 Neg thresh, Neg Drift Comp
	//Upper nibble = Neg Drift comp - via Lookup Table (LUT)
	//Lower nibble = Neg Threshold - take operand and add 4 to get value, default value: 6
	[1 ... 24] = 0x42,

	//155-178 Pos Drift Comp
	//Upper nibble = Pos Drift comp - via LUT,
	//default value: 4 (0.6s / count of drift compensation)
	[25 ... 48] = (2 << 4),

	//179-202 Normal DI Limit, Fast DI Limit
	//Upper nibble = Fast DI Limit, values same as operand (0 does not work), FDIL Default value: 5
	//Lower nibble = Normal DI Limit, values same as operand (0 = disabled burst), NDIL Default value: 2
	[49 ... 64] = 0x54, //эти 16 клавиш работают
	[65 ... 72] = 0x50, //эти 8 клавиш выключены

	//203-226 Neg recal delay
	//Range is in 0.5 sec increments; 0 = infinite, default = 10s,
	//Range is {infinite, 0.5...127s}; 255 is illegal to use
	[73 ... 96] = 20,

	//227-250 Wake On Touch, Burst Length, AKS, Scope Sync
	//Bit 7 = Scope sync, 1 = enabled
	//Bit 6 = AKS (Adjacent Key Suppression - подавление смежных клавиш), 1 - enabled,
	//Bits 5, 4 = BL, via LUT, default = 48 (index 2)
	//Bit 3 = WAKE, 1 - enabled,
	[97 ... 120] = _BV(6) | (2<<4) | _BV(3),

	//251 Sleep Duration, Mains Sync
	//Bits 2,1,0 = Sleep Duration, 8 values via LUT, default = 125ms (index 3)
	//Bits 6 = Mains sync, negative edge, 1 = enabled, default = off
	0x00,

	//252 Awake Timeout
	//Range is in 100ms increments; 1 = 100ms. Default = 2.5s. 0 is illegal to use
	1,

	//253 Drift Hold Time
	//Range is in 100ms increments; 0 = disable, 1 = 100ms, default = 1s
	10
};

uint8_t kbd_init(void);
uint8_t kbd_poll(char *c);

static uint8_t kbd_calibrate(void);
static uint8_t kbd_send_setup(void);
static uint8_t kbd_verify_setup(void);

static uint8_t i2c_kbd_write(uint8_t addr, uint8_t subaddr, const uint8_t *data, uint8_t len);
static uint8_t i2c_kbd_write_P(uint8_t addr, uint8_t subaddr, const prog_uint8_t *data, uint8_t len);
static uint8_t i2c_kbd_read(uint8_t addr, uint8_t subaddr, uint8_t *data, uint8_t len);

uint8_t kbd_init() {
	uint8_t data_buf;

	//ѕо включению произведем пустое чтение. „тобы убедитьс€, что клавиатура вышла из Reset
	//»наче получим Incorrect Setup Data
	if (i2c_kbd_read(ADDR_KEYBOARD, SUB_ADDR_KBD_DETECT_STATUS, &data_buf, 1)) {
		fprintf(stderr, "\nkbd dummy read error");
		return 1;
	}

	//Verify Setup Block
	if (kbd_verify_setup()) {
		//Incorrect Setup Data
		fprintf(stderr, "\nIncorrect Setup Data");
		if (kbd_send_setup()) {
			//send setup failure
			return 1;
		}
	}

	return 0;
}

static uint8_t kbd_calibrate()
{
	uint8_t data_buf = 0x55;
	return i2c_kbd_write(ADDR_KEYBOARD, SUB_ADDR_KBD_CAL, &data_buf, 1);
}

static uint8_t kbd_send_setup() {
	if (i2c_kbd_write_P(ADDR_KEYBOARD, SUB_ADDR_KBD_SETUPS_WRITE_UNLOCK,
			kbd_setups, sizeof(kbd_setups))) {
		fprintf(stderr, "\nkbd_send_setup failure"); //SUB_ADDR_KBD_SETUPS_WRITE_UNLOCK
		return 1;
	}

	fprintf(stderr, "\nkbd_send_setup ok");
	return 0;
}

static uint8_t kbd_verify_setup() {
	uint8_t data_buf[sizeof(kbd_setups) - 1];

	if (i2c_kbd_read(ADDR_KEYBOARD, SUB_ADDR_KBD_SETUPS, data_buf,
			sizeof(data_buf)))
		return 1;

	for (uint8_t i = 0; i < sizeof(data_buf); i++) {
		if (data_buf[i] != pgm_read_byte(&kbd_setups[i+1]))
			return 1;
	}

	return 0;
}

uint8_t kbd_poll(char *c) {
#if 1
	static const prog_uint8_t key_ascii_code_tab[16] = { '8', '7', '*', '0',
			'#', 'C', 'D', '9', '5', '4', '1', '2', '3', 'A', 'B', '6' }; //нова€ клавиатура
#else
	static const prog_uint8_t key_ascii_code_tab[16] = {'2','1','*','0','#','C','D','3','8','7','4','5','6','A','B','9'};  //стара€ клавиатура
#endif

	static uint16_t kbd_data_old = 0;
	char key_code = 0;
	uint8_t data_buf[2];
	uint16_t kbd_data_new, kbd_data_change;
	uint16_t j;

	if (i2c_kbd_read(ADDR_KEYBOARD, SUB_ADDR_KBD_DETECT_STATUS, data_buf, 2)) {
		fprintf(stderr, "\nkeyboard poll error");
		return 1;
	}

	//fprintf(stderr, "\nkeyboard poll ok");

	kbd_data_new = data_buf[0] | data_buf[1] << 8;
	kbd_data_change = (kbd_data_old ^ kbd_data_new) & kbd_data_new;
	kbd_data_old = kbd_data_new;

	if (kbd_data_change) {
		j = 1;
		for (uint8_t i = 0; i < 16; i++) {
			if (kbd_data_change & j) {
				key_code = pgm_read_byte(&key_ascii_code_tab[i]);
				break;
			}
			j <<= 1;
		}
	}

	*c = key_code;

	return 0;
}

static uint8_t i2c_kbd_write(uint8_t addr, uint8_t subaddr, const uint8_t *data, uint8_t len) {
	uint8_t err_code;

	do {
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(addr);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(subaddr);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		while (len)
		{
			i2c_write(*data++);
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_TXDATA_ACK)
				break;

			--len;
		}
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		err_code = 0;
	} while(0);

	i2c_stop();
	return err_code;
}

static uint8_t i2c_kbd_write_P(uint8_t addr, uint8_t subaddr, const prog_uint8_t *data, uint8_t len) {
	uint8_t err_code;

	do {
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(addr);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		i2c_write(subaddr);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		while (len)
		{
			uint8_t data_byte = pgm_read_byte(data++);
			i2c_write(data_byte);
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_TXDATA_ACK)
				break;

			--len;
		}
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		err_code = 0;
	} while(0);

	i2c_stop();
	return err_code;
}

static uint8_t i2c_kbd_read(uint8_t addr, uint8_t subaddr, uint8_t *data, uint8_t len) {
	uint8_t err_code;

	do {
		/*
		 * The host initiates the transfer by sending the START condition,
		 * and follows this by sending the slave address of
		 * the device together with the Write-bit. The device sends an ACK.
		 */
		do {
			i2c_start();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_START && err_code != I2C_MASTER_REPSTART)
				break;

			i2c_slaW(addr);
			err_code = i2c_read_state_wait();
		} while (err_code == I2C_MASTER_SLAW_NACK);
		if (err_code != I2C_MASTER_SLAW_ACK)
			break;

		/*
		 * The host then sends the memory address within the device
		 * it wishes to read from. The device sends an ACK.
		 */
		i2c_write(subaddr);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_TXDATA_ACK)
			break;

		/*
		 * The host must then send a STOP and a START condition
		 * followed by the slave address again but this time accompanied by the Read-bit.
		 * The device will return an ACK, followed by a data byte.
		 */
		i2c_start();
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_REPSTART)
			break;

		i2c_slaR(addr);
		err_code = i2c_read_state_wait();
		if (err_code != I2C_MASTER_SLAR_ACK)
			break;

		while (len > 1)
		{
			i2c_read_ack();
			err_code = i2c_read_state_wait();
			if (err_code != I2C_MASTER_RXDATA_ACK)
				break;

			*data++ = i2c_get_byte();
			--len;
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
