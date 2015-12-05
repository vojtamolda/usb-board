/*
ATmega168 - Breadboard Prototype Firmware for ATmega168.

Copyright (C) 2009  Vojta Molda <vojta.molda@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define ATMEGA168_FIRMWARE_SHARED
#define ATMEGA168_FIRMWARE_PRIVATE
#include "atmega168.h"


int main(void) {
	sei();
	io_init();
	spi_init();
	adc_init();
	pwm_init();
	usart_init();

	while (1) {
		// Receive data from application.
		usart_receive_data();
		// Send DAC control sequence.
		dac_set(DAC_TR_START);
		// Set PWM duty cycle.
		pwm_set();
		// Set digital outputs.
		dout_set();
		// Get digital inputs.
		din_get();
		// Send data to application.
		usart_send_data();
	}
}


inline void io_init(void) {
	// Setup digital inputs and pull-up resistors.
	cbi(DDRB, PB0);
	sbi(PORTB, PB0);
	cbi(DDRB, PB1);
	sbi(PORTB, PB1);
	// Setup digital outputs.
	sbi(DDRD, PD7);
}

inline void pwm_init(void) {
	// Setup PWM outputs.
	sbi(DDRD, PD5);
	sbi(DDRD, PD6);
	// Fast PWM mode.
	sbi(TCCR0A, WGM00);
	sbi(TCCR0A, WGM01);
	// Non-inverting PWM mode.
	sbi(TCCR0A, COM0A1);
	sbi(TCCR0A, COM0B1);
	// Setup clock multiplier.
	sbi(TCCR0B, CS01);
	OCR0A = 0;
	OCR0B = 0;
}

inline void spi_init(void) {
	sbi(SPI_DDR, SPI_CLK);
	sbi(SPI_DDR, SPI_MOSI);
	sbi(SPI_DDR, SPI_DAC_CS);
	sbi(SPI_DDR, SPI_DAC_SHDN);
	sbi(SPI_DDR, SPI_DAC_LDAC);
	// Turn on chip select.
	sbi(SPI_PORT, SPI_DAC_CS);
	// Turn on SPI interface.
	sbi(SPCR, SPE);
	// Turn on end of transfer IRQ.
	sbi(SPCR, SPIE);
	// Setup SPI master.
	sbi(SPCR, MSTR);
	// Setup clock multiplier.
	sbi(SPSR, SPI2X);
}

inline void adc_init(void) {
	// Turn off all other ADC pin functions.
	sbi(DIDR0, ADC0D);
	sbi(DIDR0, ADC1D);
	sbi(DIDR0, ADC2D);
	sbi(DIDR0, ADC3D);
	sbi(DIDR0, ADC4D);
	sbi(DIDR0, ADC5D);
	// Setup clock multiplier.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);
	// Setup reference voltage.
	sbi(ADMUX, REFS0);
	// Turn on AD converter.
	sbi(ADCSRA, ADEN);
	// Turn on end of conversion IRQ.
	sbi(ADCSRA, ADIE);
	// Start conversion.
	sbi(ADCSRA, ADSC);
}

inline void usart_init(void) {
	// Setup baudrate.
	UBRR0 = USART_UBRR;
	// Turn on TX line.
	sbi(UCSR0B, TXEN0);
	// Turn on RX line.
	sbi(UCSR0B, RXEN0);
	// Turn on byte receive IRQ.
	sbi(UCSR0B, RXCIE0);
	// Turn on empty data register IRQ.
	sbi(UCSR0B, UDRIE0);
	// Setup 8-bit transmit frame length.
	sbi(UCSR0C, UCSZ01);
	sbi(UCSR0C, UCSZ00);
	#ifdef USART_FLWCTRL
		// Setup CTS input and pull-up resistor.
		cbi(USART_DDR, USART_CTS);
		sbi(USART_PORT, USART_CTS);
		// Setup RTS output.
		sbi(USART_DDR, USART_RTS);
		sbi(USART_PORT, USART_RTS);
	#endif
}


void usart_send_data(void) {
	uint8_t tx_size = 0;
	while (tx_size < sizeof(txdata_t)) {
		usart_putchar(txdata.raw[tx_size]);
		tx_size++;
	}
}

void usart_putchar(char byte) {
	while (tx.size == TXBUF_SIZE);
	tx.wr++;
	tx.size++;
	tx.wr &= TXBUF_MASK;
	tx.buf[tx.wr] = byte;
	sbi(UCSR0B, UDRIE0);
	return;
}

char usart_getchar(void) {
	while (rx.size == 0);
	rx.rd++;
	rx.size--;
	rx.rd &= RXBUF_MASK;
	#ifdef USART_FLWCTRL
		if (rx.size < (RXBUF_SIZE - RXBUF_MARGIN)) {
			sbi(USART_PORT, USART_RTS);
		}
	#endif
	return rx.buf[rx.rd];
}

void usart_receive_data(void) {
	uint8_t rx_size = 0;
	while (rx_size < sizeof(rxdata_t)) {
		rxdata.raw[rx_size] = usart_getchar();
		rx_size++;
	}
}


void din_get(void) {
	txdata.data.din[0] = bv(PINB, PB0);
	txdata.data.din[1] = bv(PINB, PB1);
}

void pwm_set(void) {
	OCR0A = rxdata.data.pwm[0];
	OCR0B = rxdata.data.pwm[1];
}

void dout_set(void) {
	if (rxdata.data.dout[0] == 0) {
		cbi(PORTD, PD7);
	} else {
		sbi(PORTD, PD7);
	}
}

void dac_set(char flag) {
	static uint8_t chan = 0;
	switch (flag) {
		case DAC_TR_START:
			cbi(SPI_PORT, SPI_DAC_CS);
				switch (chan) {
				case 0:
					SPDR = ((rxdata.data.dac[chan] >> 8) | DAC0_MASK);
					break;
				case 1:
					SPDR = ((rxdata.data.dac[chan] >> 8) | DAC1_MASK);
					break;
			}
			break;
		case DAC_TR_END:
			SPDR = rxdata.data.dac[chan];
			if (chan == 0) {
				chan = 1;
			} else {
				chan = 0;
			}
			break;
	}
}


ISR(USART_UDRE_vect, ISR_BLOCK) {
	if (tx.size == 0) {
		cbi(UCSR0B, UDRIE0);
	} else {
		#ifdef USART_FLWCTRL
			while (!bv(USART_PIN, USART_CTS));
		#endif
		tx.rd++;
		tx.size--;
		tx.rd &= TXBUF_MASK;
		UDR0 = tx.buf[tx.rd];
	}
}

ISR(USART_RX_vect, ISR_BLOCK) {
	rx.wr++;
	rx.size++;
	rx.wr &= RXBUF_MASK;
	rx.buf[rx.wr] = UDR0;
	#ifdef USART_FLWCTRL
		if (rx.size > (RXBUF_SIZE - RXBUF_MARGIN)) {
			cbi(USART_PORT, USART_RTS);
		}
	#endif
}

ISR(ADC_vect, ISR_BLOCK) {
	static uchar mux = 0;
	txdata.data.adc[mux] = ADC;
	if (mux == (ADCBUF_SIZE - 1)) {
		mux = 0;
	} else {
		mux++;
	}
	ADMUX = (ADMUX & ADCBUF_MASK) | mux;
	sbi(ADCSRA, ADSC);
}

ISR(SPI_STC_vect, ISR_BLOCK) {
	static uchar eop = 0;
	if (eop == 1) {
		sbi(SPI_PORT, SPI_DAC_CS);
		eop = 0;
	} else {
		dac_set(DAC_TR_END);
		eop = 1;
	}
}
