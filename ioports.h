#ifndef IOPORTS_H_
#define IOPORTS_H_

#include <avr/io.h>
#include <stdbool.h>

#define IO_STRUCT(port, pin) {&PORT##port, &DDR##port, &PIN##port, _BV(pin)}

typedef struct
{
  volatile uint8_t *port_ptr;
  volatile uint8_t *ddr_ptr;
  volatile uint8_t *pin_ptr;
  const uint8_t pin_mask;
} io_pin_struct;

extern void gpio_setup(const io_pin_struct pins[], uint8_t size, bool out, bool pu_h);
extern void gpio_set_state(const io_pin_struct pins[], uint8_t size, uint8_t state_mask);
extern uint8_t gpio_get_state(const io_pin_struct pins[], uint8_t size);

extern void gpio_set(const io_pin_struct *pin);
extern void gpio_clear(const io_pin_struct *pin);
extern uint8_t gpio_is_set(const io_pin_struct *pin);

#endif

