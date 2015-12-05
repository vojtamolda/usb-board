/*
Firmware - USB Measuring Board Firmware for ATmega1280.

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

#define USBMD_FIRMWARE_SHARED
#define USBMD_FIRMWARE_PRIVATE
#include "firmware.h"


int main(void) {
	sei();
	io_init();
	dac_init();
	adc_init();
	pwm_init();
	usart_init();

	rxdata.data.dac[0] = 0;
	rxdata.data.dac[1] = 0;
	rxdata.data.dout[0] = 0;
	rxdata.data.dout[1] = 0;
	rxdata.data.pwm[0] = 0;
	rxdata.data.pwm[1] = 0;
	rxdata.data.pwm[2] = 0;
	rxdata.data.pwm[3] = 0;
	rxdata.data.pwm[4] = 0;
	rxdata.data.pwm[5] = 0;
	rxdata.data.pwm[6] = 0;
	rxdata.data.pwm[7] = 0;

	while (1) {
		// Receive data from application.
        usart_receive_data();
		// Set DAC channel #1.
		dac_set(0);
		// Set PWM duty cycle.
		pwm_set();
		// Set digital outputs.
		dout_set();
		// Get digital inputs.
		din_get();
		// Set DAC channel #2.
		dac_set(1);
		// Send data to application.
		usart_send_data();
	}
}


inline void io_init(void) {
	// Setup digital inputs and pull-up resistors.
	DDRJ = 0x00;
	PORTJ = 0xFF;
	DDRD = 0x00;
	PORTD = 0xFF;
	// Setup digital outputs.
	DDRC = 0xFF;
	DDRA = 0xFF;
}

inline void pwm_init(void) {
	// Setup PWM outputs.
	sbi(DDRB, PB6);
	sbi(DDRB, PB7);
	sbi(DDRE, PE4);
	sbi(DDRE, PE5);
	sbi(DDRH, PH4);
	sbi(DDRH, PH5);
	sbi(DDRL, PL4);
	sbi(DDRL, PL5);
	// Fast PWM mode.
	sbi(TCCR1A, WGM10);
	cbi(TCCR1A, WGM11);
	sbi(TCCR1B, WGM12);
	cbi(TCCR1B, WGM13);
	sbi(TCCR3A, WGM30);
	cbi(TCCR3A, WGM31);
	sbi(TCCR3B, WGM32);
	cbi(TCCR3B, WGM33);
	sbi(TCCR4A, WGM40);
	cbi(TCCR4A, WGM41);
	sbi(TCCR4B, WGM42);
	cbi(TCCR4B, WGM43);
	sbi(TCCR5A, WGM50);
	cbi(TCCR5A, WGM51);
	sbi(TCCR5B, WGM52);
	cbi(TCCR5B, WGM53);
	// Non-inverting PWM mode.
	sbi(TCCR1A, COM1B1);
	sbi(TCCR1A, COM1C1);
	sbi(TCCR3A, COM3B1);
	sbi(TCCR3A, COM3C1);
	sbi(TCCR4A, COM4B1);
	sbi(TCCR4A, COM4C1);
	sbi(TCCR5A, COM5B1);
	sbi(TCCR5A, COM5C1);
	// Setup clock multiplier.
	sbi(TCCR1B, CS10);
	sbi(TCCR3B, CS30);
	sbi(TCCR4B, CS40);
	sbi(TCCR5B, CS50);
}

inline void dac_init(void) {
	// Setup inputs and outptus.
	sbi(SPI_DDR, SPI_CLK);
	sbi(SPI_DDR, SPI_MOSI);
	sbi(SPI_DDR, SPI_DAC_CS);
	sbi(SPI_DDR, SPI_DAC_SHDN);
	sbi(SPI_DDR, SPI_DAC_LDAC);
	// Setup SPI master in mode 0.
	sbi(UCSR0C, UMSEL01);
	sbi(UCSR0C, UMSEL00);
	cbi(UCSR0C, UDORD0);
	cbi(UCSR0C, UCPHA0);
	cbi(UCSR0C, UCPOL0);
	// Turn on TX and RX line.
	sbi(UCSR0B, RXEN0);
	sbi(UCSR0B, TXEN0);
	// Setup SPI baudrate.
	UBRR0 = SPI_UBRR;
	// Turn on chip select.
	sbi(SPI_PORT, SPI_DAC_CS);
	// Turn on shutdown.
	sbi(SPI_PORT, SPI_DAC_SHDN);
	// Turn off latch.
	cbi(SPI_PORT, SPI_DAC_LDAC);
}

inline void adc_init(void) {
	// Turn off all other ADC pin functions.
	DIDR0 = 0xFF;
	DIDR2 = 0xFF;
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
	UBRR2 = USART_UBRR;
	// Turn on 2x baud rate multiplier.
	cbi(UCSR2A, U2X2);
	// Turn on TX line.
	sbi(UCSR2B, TXEN2);
	// Turn on RX line.
	sbi(UCSR2B, RXEN2);
	// Turn on byte receive IRQ.
	sbi(UCSR2B, RXCIE2);
	// Turn on empty data register IRQ.
	sbi(UCSR2B, UDRIE2);
	// Setup 8-bit transmit frame length.
	sbi(UCSR2C, UCSZ21);
	sbi(UCSR2C, UCSZ20);
	#ifdef USART_FLWCTRL
		// Setup CTS input and pull-up resistor.
		cbi(USART_DDR, USART_CTS);
		sbi(USART_PORT, USART_CTS);
		// Setup RTS output.
		sbi(USART_DDR, USART_RTS);
		sbi(USART_PORT, USART_RTS);
	#endif
}


void din_get(void) {
	txdata.data.din[0] = PIND;
	txdata.data.din[1] = PINJ;
}

void pwm_set(void) {
	OCR1B = rxdata.data.pwm[0];
	OCR1C = rxdata.data.pwm[1];
	OCR3B = rxdata.data.pwm[2];
	OCR3C = rxdata.data.pwm[3];
	OCR4B = rxdata.data.pwm[4];
	OCR4C = rxdata.data.pwm[5];
	OCR5B = rxdata.data.pwm[6];
	OCR5C = rxdata.data.pwm[7];
}

void dout_set(void) {
	PORTA = rxdata.data.dout[0];
	PORTC = rxdata.data.dout[1];
}

void dac_set(uint8_t chan) {
	while (!bv(UCSR0A, UDRE0));
	cbi(SPI_PORT, SPI_DAC_CS);
	switch (chan) {
		case 0:
			UDR0 = ((rxdata.data.dac[0] >> 8) | DAC0_MASK);
			break;
		case 1:
			UDR0 = ((rxdata.data.dac[1] >> 8) | DAC1_MASK);
			break;
	}
	UDR0 = rxdata.data.dac[chan];
	sbi(UCSR0B, TXCIE0);
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
	sbi(UCSR2B, UDRIE2);
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


ISR(USART2_UDRE_vect, ISR_BLOCK) {
	if (tx.size == 0) {
		cbi(UCSR2B, UDRIE2);
	} else {
		#ifdef USART_FLWCTRL
			while (!bv(USART_PIN, USART_CTS));
		#endif
		tx.rd++;
		tx.size--;
		tx.rd &= TXBUF_MASK;
		UDR2 = tx.buf[tx.rd];
	}
}

ISR(USART2_RX_vect, ISR_BLOCK) {
	rx.wr++;
	rx.size++;
	rx.wr &= RXBUF_MASK;
	rx.buf[rx.wr] = UDR2;
	#ifdef USART_FLWCTRL
		if (rx.size > (RXBUF_SIZE - RXBUF_MARGIN)) {
			cbi(USART_PORT, USART_RTS);
		}
	#endif
}

ISR(ADC_vect, ISR_BLOCK) {
	static uchar mux = 0;
	txdata.data.adc[mux] = ADC;
	switch (mux) {
		case (ADCBUF_SIZE - 1):
			cbi(ADCSRB, MUX5);
			mux = 0;
			break;
		case 7:
			sbi(ADCSRB, MUX5);
			mux++;
			break;
		default:
			mux++;
	}
	ADMUX = (ADMUX & ADCBUF_MASK) | (mux & ADCMUX_MASK);
	sbi(ADCSRA, ADSC);
}

ISR(USART0_TX_vect, ISR_BLOCK) {
	sbi(SPI_PORT, SPI_DAC_CS);
	cbi(UCSR0B, TXCIE0);
}
