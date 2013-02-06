#ifndef SOFT_UART_H_INCLUDED
#define SOFT_UART_H_INCLUDED

#include <avr/io.h>


#define BAUD_RATE      57600       //скорость обмена по софтверной шине
#define SUART_BITS		8

#define RX_PINX       PIND
#define RX_PORTX      PORTD
#define RX_DDRX       DDRD
#define RX_PIN        2  

#define TX_PORTX      PORTD
#define TX_DDRX       DDRD
#define TX_PIN        4

#define get_rx_pin_status() RX_PINX & _BV(RX_PIN)
#define set_tx_pin_high()   TX_PORTX |= _BV(TX_PIN)
#define set_tx_pin_low()    TX_PORTX &= ~_BV(TX_PIN)

#define ENABLE_TIMER2_INTERRUPT( )       ( TIMSK |= _BV(OCIE2))
#define DISABLE_TIMER2_INTERRUPT( )      ( TIMSK &= ~_BV(OCIE2))
#define CLEAR_TIMER2_INTERRUPT( )        ( TIFR = _BV(OCF2))

#define ENABLE_EXTERNAL0_INTERRUPT( )   ( GICR |= _BV(INT0))
#define DISABLE_EXTERNAL0_INTERRUPT( )  ( GICR &= ~_BV(INT0))
#define CLEAR_EXTERNAL0_INTERRUPT( )     (GIFR = _BV(INTF0))

#define ENABLE_TIMER1_INTERRUPT( )       ( TIMSK |= _BV(OCIE1A))
#define DISABLE_TIMER1_INTERRUPT( )      ( TIMSK &= ~_BV(OCIE1A))
#define TEST_TIMER1_INTERUPT( )			 (TIMSK & _BV(OCIE1A))
#define CLEAR_TIMER1_INTERRUPT( )        ( TIFR = _BV(OCF1A))

#define TICKS2WAITONE ((F_CPU/BAUD_RATE/8)-1)						//для прескалера=8 скорости могут быть не выше 57600
#define TICKS2WAITONE_HALF  (TICKS2WAITONE+ (TICKS2WAITONE/2)) 
#define INTERRUPT_EXEC_CYCL   2
#define TX_INTERRUPT_EXEC_CYCL 16


extern void SUART_Init(void);
extern void SUART_PutChar(uint8_t ch);

#endif 

