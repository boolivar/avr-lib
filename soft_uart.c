#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "soft_uart.h"
#include "global.h"
#include "fifo.h"
#include "async.h"
#include "timer8.h"

static uint8_t bits_left_in_rx;
static uint8_t bits_left_out_tx;

static uint8_t internal_rx_buffer;
static uint16_t internal_tx_buffer;

static suart_rx_handler_t rx_handler;

void SUART_Init(suart_rx_handler_t handler);
void SUART_PutChar(uint8_t ch);

void SUART_Init(suart_rx_handler_t handler)
{
	rx_handler = handler;

    RX_DDRX &= ~_BV(RX_PIN);	
    RX_PORTX |= _BV(RX_PIN);
  
    TX_DDRX |= _BV(TX_PIN);		
	set_tx_pin_high();			

	DISABLE_TIMER2_INTERRUPT( );
 	set_timer2_wg_mode(T2_WG_CTC);
	set_timer2_clock_source(T2S_DIV8);

	EXTINT_SET_INT_SENSE_CONTOL(IS_FALLING_EDGE);
	ENABLE_EXTERNAL_INTERRUPT();

	set_timer1_wg_mode(CTC_OCR1A);
	set_timer1_clock_source(T1S_DIV8);
	OCR1AH = 0;
	OCR1AL = TICKS2WAITONE;
}

ISR(EXTINT_VECTOR)
{
	SFIOR |= _BV(PSR2);
	TCNT2 = INTERRUPT_EXEC_CYCL;
	OCR2 = TICKS2WAITONE_HALF;
	CLEAR_TIMER2_INTERRUPT( );
	ENABLE_TIMER2_INTERRUPT( );

	bits_left_in_rx = 0;
	internal_rx_buffer = 0;
	DISABLE_EXTERNAL_INTERRUPT();
}

ISR(TIMER2_COMP_vect)
{
    OCR2 = TICKS2WAITONE;

    if(get_rx_pin_status())	{
		internal_rx_buffer |= 0x80;
	}

    if(++bits_left_in_rx < SUART_BITS) {
    	internal_rx_buffer >>= 1;
    }else {
    	DISABLE_TIMER2_INTERRUPT();
		CLEAR_EXTERNAL_INTERRUPT();
		ENABLE_EXTERNAL_INTERRUPT();
		rx_handler(internal_rx_buffer);
    }
}


ISR(TIMER1_COMPA_vect)
{
	if(bits_left_out_tx < (SUART_BITS+2)) {
		if(internal_tx_buffer & 1)
		{
			set_tx_pin_high();
		}
		else
		{
			set_tx_pin_low();
		}	    
		internal_tx_buffer >>= 1;
		bits_left_out_tx++;
    }
	else {
		DISABLE_TIMER1_INTERRUPT( );
	}	
}

void SUART_PutChar(uint8_t ch)
{
	internal_tx_buffer=(uint16_t) ch;
	internal_tx_buffer<<= 1;
	internal_tx_buffer |= 0x200;
	bits_left_out_tx= 0;
	TCCR1B&= ~_BV(CS11);
	TCNT1L = TX_INTERRUPT_EXEC_CYCL;
	TCCR1B|= _BV(CS11);

	CLEAR_TIMER1_INTERRUPT( );
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ENABLE_TIMER1_INTERRUPT( );
	}
}
