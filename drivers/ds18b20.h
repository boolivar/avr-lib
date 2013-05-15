#if !defined(__DS18B20_H__)
#define __DS18B20_H__ 

typedef enum {
	DS18B20_SEARCH_ROM = 0xF0,
	DS18B20_READ_ROM = 0x33,
	DS18B20_MATCH_ROM = 0x55,
	DS18B20_SKIP_ROM = 0xCC,
	DS18B20_ALARM_SEARCH = 0xEC,
	DS18B20_CONVERT_T = 0x44,
	DS18B20_WRITE_SCRATCHPAD = 0x4E,
	DS18B20_READ_SCRATCHPAD = 0xBE,
	DS18B20_RECALL_E2 = 0xB8,
	DS18B20_READ_POWER_SUPPLY = 0xB4,
} ds18b20_cmd_enum;

extern uint8_t ds18b20_start_convert_t(void);
extern uint8_t ds18b20_is_conversion(void);
extern uint8_t ds18b20_read_temperature(uint16_t* temperature);
extern uint8_t ds18b20_crc(uint8_t crc, uint8_t x);

#endif
