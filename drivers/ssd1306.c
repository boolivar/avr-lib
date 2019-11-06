
#include "ssd1306.h"

i2c_state_t ssd1306_configure(ssd1306_addr_t addr, const ssd1306_config_t* cfg);
i2c_state_t ssd1306_start(ssd1306_addr_t addr);
i2c_state_t ssd1306_send_command2(ssd1306_command_t cmd, uint8_t data);
i2c_state_t ssd1306_send_command(ssd1306_command_t cmd);
i2c_state_t ssd1306_send(uint8_t value);
void ssd1306_stop();

i2c_state_t ssd1306_configure(ssd1306_addr_t addr, const ssd1306_config_t* cfg) {
	i2c_state_t state;

	do {
		state = ssd1306_start(addr);
		if (state != I2C_MASTER_SLAW_ACK) {
			break;
		}

		if (cfg->display_clock != 0) {
			state = ssd1306_send_command2(SSD1306_CLOCK_RATIO, *cfg->display_clock);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->multiplex_ratio != 0) {
			state = ssd1306_send_command2(SSD1306_MULTIPLEX_RATIO, *cfg->multiplex_ratio);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->display_offset != 0) {
			state = ssd1306_send_command2(SSD1306_DISPLAY_OFFSET, *cfg->display_offset);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->contrast != 0) {
			state = ssd1306_send_command2(SSD1306_DISPLAY_CONTRAST, *cfg->contrast);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->vcomh_deselect_level != 0) {
			state = ssd1306_send_command2(SSD1306_VCOMH_DESELECT_LEVEL, (*cfg->vcomh_deselect_level & 0x07) << 4);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->display_start_line != 0) {
			state = ssd1306_send_command(SSD1306_DISPLAY_START_LINE | (*cfg->display_start_line & 0x3f));
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->charge_pump != 0) {
			state = ssd1306_send_command2(SSD1306_CHARGE_PUMP, cfg->charge_pump);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->precharge != 0) {
			state = ssd1306_send_command2(SSD1306_PRECHARGE_PERIOD, cfg->precharge);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->segment_remap != 0) {
			state = ssd1306_send_command(cfg->segment_remap);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->com_remap != 0) {
			state = ssd1306_send_command(cfg->com_remap);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->com_config != 0) {
			state = ssd1306_send_command2(SSD1306_COM_CONFIG, cfg->com_config);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->display_mode != 0) {
			state = ssd1306_send_command(cfg->display_mode);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}

		if (cfg->inverse_mode != 0) {
			state = ssd1306_send_command(cfg->inverse_mode);
			if (state != I2C_MASTER_TXDATA_ACK) {
				break;
			}
		}
	} while (0);

	ssd1306_stop();

	return state;
}

i2c_state_t ssd1306_start(ssd1306_addr_t addr) {
	i2c_state_t state;

	i2c_start();

	state = i2c_read_state_wait();
	if (state != I2C_MASTER_START) {
		return state;
	}

	i2c_slaW(addr << 1);

	return i2c_read_state_wait();
}

i2c_state_t ssd1306_send_command2(ssd1306_command_t cmd, uint8_t data) {
	i2c_state_t state = ssd1306_send_command(cmd);
	if (state != I2C_MASTER_TXDATA_ACK) {
		return state;
	}
	return ssd1306_send_command(cmd);
}

i2c_state_t ssd1306_send_command(ssd1306_command_t cmd) {
	i2c_state_t state = ssd1306_send(CTRL_COMMAND);
	if (state != I2C_MASTER_TXDATA_ACK) {
		return state;
	}
	return ssd1306_send(cmd);
}

i2c_state_t ssd1306_send(uint8_t value) {
	i2c_write(value);
	return i2c_read_state_wait();
}

void ssd1306_stop() {
	i2c_stop();
	i2c_wait_stop();
}
