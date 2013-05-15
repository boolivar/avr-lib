#include <avr/io.h>

#include "inc/global.h"
#include "inc/ds18b20.h"
#include "one_wire.h"

static const uint8_t dscrc_table[256] = {
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156,
	126, 32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162, 64, 30, 95, 1,
	227, 189, 62, 96, 130, 220, 35, 125, 159, 193, 66, 28, 254, 160, 225,
	191, 93, 3, 128, 222, 60, 98, 190, 224, 2, 92, 223, 129, 99, 61, 124,
	34, 192, 158, 29, 67, 161, 255, 70, 24, 250, 164, 39, 121, 155, 197,
	132, 218, 56, 102, 229, 187, 89, 7, 219, 133, 103, 57, 186, 228, 6, 88,
	25, 71, 165, 251, 120, 38, 196, 154, 101, 59, 217, 135, 4, 90, 184, 230,
	167, 249, 27, 69, 198, 152, 122, 36, 248, 166, 68, 26, 153, 199, 37,
	123, 58, 100, 134, 216, 91, 5, 231, 185, 140, 210, 48, 110, 237, 179,
	81, 15, 78, 16, 242, 172, 47, 113, 147, 205, 17, 79, 173, 243, 112, 46,
	204, 146, 211, 141, 111, 49, 178, 236, 14, 80, 175, 241, 19, 77, 206,
	144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238, 50, 108, 142, 208,
	83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115, 202, 148, 118,
	40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139, 87, 9, 235,
	181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22, 233, 183,
	85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168, 116, 42,
	200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53 };

uint8_t ds18b20_start_convert_t(void);
uint8_t ds18b20_is_conversion(void);
uint8_t ds18b20_read_temperature(uint16_t* temperature);
uint8_t ds18b20_crc(uint8_t crc, uint8_t x);

uint8_t ds18b20_start_convert_t() {
	if (ow_reset_presence()) {
		ow_write_byte(DS18B20_SKIP_ROM);
		ow_write_byte(DS18B20_CONVERT_T);
		return 1;
	}
	return 0;
}

uint8_t ds18b20_is_conversion() {
	return (ow_read_bit() == 0);
}

uint8_t ds18b20_read_temperature(uint16_t* temperature) {
	uint8_t i;
	uint8_t data[9];
	uint8_t crc;

	if (ow_reset_presence()) {
		ow_write_byte(DS18B20_SKIP_ROM);
		ow_write_byte(DS18B20_READ_SCRATCHPAD);

		crc = 0;

		for (i = 0; i < 9; ++i) {
			data[i] = ow_read_byte();
			crc = ds18b20_crc(crc, data[i]);
		}

		if (crc == 0) {
			*temperature = ((uint16_t)data[1] << 8) | data[0];
			return 1;
		}
	}

	return 0;
}

uint8_t ds18b20_crc(uint8_t crc, uint8_t x) {
	return dscrc_table[crc ^ x];
}
