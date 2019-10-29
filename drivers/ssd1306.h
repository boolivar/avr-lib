#ifndef SSD1306_H
#define SSD1306_H

#include "i2c.h"

typedef enum {
	SA0_LOW = 0b0111100,
	SA0_HIGH = 0b0111101,
} ssd1306_addr_t;

typedef enum {
	CTRL_DATA_STREAM = 0,
	CTRL_COMMAND = 0x80,
	CTRL_DATA = 0xC0,
} ssd1306_control_t;

typedef enum {
	SSD1306_DISPLAY_OFF = 0xAE,
	SSD1306_DISPLAY_ON = 0xAF,
	SSD1306_CLOCK_RATIO = 0xD5,
	SSD1306_MULTIPLEX_RATIO = 0xA8,
	SSD1306_DISPLAY_OFFSET = 0xD3,
	SSD1306_DISPLAY_START_LINE = 0x40,
	SSD1306_CHARGE_PUMP = 0x8D,
	SSD1306_PRECHARGE_PERIOD = 0xD9,
	SSD1306_DISPLAY_RAM = 0xA4,
	SSD1306_DISPLAY_FILL = 0xA5,
	SSD1306_DISPLAY_NORMAL = 0xA6,
	SSD1306_DISPLAY_INVERSE = 0xA7,
	SSD1306_DISPLAY_CONTRAST = 0x81,
	SSD1306_VCOMH_DESELECT_LEVEL = 0xDB,
	SSD1306_COM_CONFIG = 0xDA,
} ssd1306_command_t;

typedef enum {
	EXTERNAL_VCC = 0x10,
	INTERNAL_DC_DC = 0x14,
} charge_pump_t;

typedef enum {
	SEG_0_0 = 0xA0,
	SEG_0_127 = 0xA1,
} segment_remap_t;

typedef enum {
	COM_0_COM_N = 0xC0,
	COM_N_COM_0 = 0xC8,
} com_remap_t;

typedef enum {
	SEQUENTIAL = 0x02,
	ALTERNATIVE = 0x12,
	SEQUENTIAL_REMAP_COM_LR = 0x22,
	ALTERNATIVE_REMAP_COM_LR = 0x32,
} com_cfg_t;

typedef struct {
	uint8_t* display_clock;
	uint8_t* multiplex_ratio;
	uint8_t* display_offset;
	uint8_t* display_start_line;
	uint8_t* contrast;
	uint8_t* vcomh_deselect_level;
	charge_pump_t charge_pump;
	charge_pump_t precharge;
	segment_remap_t segment_remap;
	com_remap_t com_remap;
	com_cfg_t com_config;
} ssd1306_config_t;

extern i2c_state_t ssd1306_configure(ssd1306_addr_t addr, const ssd1306_config_t* cfg);
extern i2c_state_t ssd1306_start(ssd1306_addr_t addr);
extern i2c_state_t ssd1306_send_command2(ssd1306_command_t cmd, uint8_t data);
extern i2c_state_t ssd1306_send_command(ssd1306_command_t cmd);
extern i2c_state_t ssd1306_send(uint8_t value);
extern void ssd1306_stop();

#endif // SSD1306_H
