#include "ioports.h"

void gpio_setup(const io_pin_struct pins[], uint8_t size, bool out, bool pu_h);
void gpio_set_state(const io_pin_struct pins[], uint8_t size, uint8_t state_mask);
uint8_t gpio_get_state(const io_pin_struct pins[], uint8_t size);

void gpio_set(const io_pin_struct *pin);
void gpio_clear(const io_pin_struct *pin);
uint8_t gpio_is_set(const io_pin_struct *pin);

void gpio_setup(const io_pin_struct pins[], uint8_t size, bool out, bool pu_h) {
	uint8_t mask;
	uint8_t i;

	for(i = 0; i < size; ++i) {
		mask = pins[i].pin_mask;

		//pull-up/high ?
		if (pu_h) {
			*pins[i].port_ptr |= mask;
		} else {
			*pins[i].port_ptr &= ~mask;
		}

		// in/out ?
		if (out) {
			*pins[i].ddr_ptr  |= mask;
		} else {
			*pins[i].ddr_ptr  &=~ mask;
		}
	}
}

void gpio_set_state(const io_pin_struct pins[], uint8_t size, uint8_t state_mask) {
	uint8_t i;
	uint8_t pin_mask;
	uint8_t mask = 1;
	
	for(i = 0; i < size; ++i) {
		pin_mask = pins[i].pin_mask;

		if(state_mask & mask) {
			*pins[i].port_ptr |= pin_mask;
		} else {
			*pins[i].port_ptr &= ~pin_mask;
		}
		
		mask <<= 1;
	}
}

uint8_t gpio_get_state(const io_pin_struct pins[], uint8_t size) {
	uint8_t i;
	uint8_t pin_mask;
	uint8_t mask = 1;
	uint8_t state = 0;
	
	for(i = 0; i < size; ++i) {
		pin_mask = pins[i].pin_mask;

		if (*pins[i].pin_ptr & pin_mask) {
			state |= mask;
		}
		
		mask <<= 1;
	}

	return state;
}

void gpio_set(const io_pin_struct *pin) {
	*pin->port_ptr |= pin->pin_mask;
}

void gpio_clear(const io_pin_struct *pin) {
	*pin->port_ptr &=~ pin->pin_mask;
}

uint8_t gpio_is_set(const io_pin_struct *pin) {
	return (*pin->port_ptr & pin->pin_mask);
}
