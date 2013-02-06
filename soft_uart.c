#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "soft_uart.h"
#include "global.h"
#include "fifo.h"
#include "async.h"

static uint8_t 				bits_left_in_rx;
static uint8_t 				bits_left_out_tx;

static uint8_t	            internal_rx_buffer;
static uint16_t				internal_tx_buffer;

void SUART_Init(void)
{
    RX_DDRX &= ~_BV(RX_PIN);	
    RX_PORTX |= (1<<RX_PIN);	//вкл pull-up резистор
  
    TX_DDRX |= _BV(TX_PIN);		
	set_tx_pin_high();			
        
	DISABLE_TIMER2_INTERRUPT( );
	TCCR2|= _BV(WGM21);     //CTC mode
 	TCCR2|= _BV(CS21);			//прескалер таймера=8 
	
	MCUCR |= _BV(ISC01);		//задний фронт
	ENABLE_EXTERNAL0_INTERRUPT( );
	
	TCCR1B|= _BV(WGM12);	//CTC 
	TCCR1B|= _BV(CS11);		//прескалер=8
	
	OCR1AH= 0;
	OCR1AL= TICKS2WAITONE;
}

ISR(INT0_vect)
{
	SFIOR |= _BV(PSR2);
	TCNT2 = INTERRUPT_EXEC_CYCL;
	OCR2 = TICKS2WAITONE_HALF;
	CLEAR_TIMER2_INTERRUPT( );
	ENABLE_TIMER2_INTERRUPT( );

	bits_left_in_rx = 0;
	internal_rx_buffer = 0;
	DISABLE_EXTERNAL0_INTERRUPT( );
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
    	DISABLE_TIMER2_INTERRUPT( );
		CLEAR_EXTERNAL0_INTERRUPT( );
		ENABLE_EXTERNAL0_INTERRUPT( );
		if (q_space(&internal_rx_fifo)) {
			q_put(&internal_rx_fifo, internal_rx_buffer);
		}
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
