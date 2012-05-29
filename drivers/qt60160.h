
#ifndef _QT60160_H_
#define _QT60160_H_

#define ADDR_KEYBOARD           17           // адрес устройства i2c

uint8_t kbd_init(void);
uint8_t kbd_poll(char *c);

#endif
