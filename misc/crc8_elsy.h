#ifndef CRC8_ELSY_INCLUDED
#define CRC8_ELSY_INCLUDED

extern uint8_t calc_crc(const void *data, uint8_t len);
extern uint8_t update_crc(uint8_t data, uint8_t crc);

extern unsigned char elsy_make_crc(const void *data, unsigned char num);

#endif
