/*
Firmware - USB Measuring Board Code for ATMmega1280.

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

#ifndef USBMBOARD_FIRMWARE_H
	#define USBMBOARD_FIRMWARE_H


	#ifdef USBMBOARD_FIRMWARE_SHARED
		#define USART_BAUD 38400
		#define DACBUF_SIZE 2
		#define ADCBUF_SIZE 16

		typedef struct tdata_t {
			uint16_t adc[ADCBUF_SIZE];
			uint8_t din[2];
		} tdata_t;
		typedef union txdata_t {
			tdata_t data;
			char raw[sizeof(tdata_t)];
		} txdata_t;

		typedef struct rdata_t {
			uint16_t dac[DACBUF_SIZE];
			uint8_t dout[2];
			uint8_t pwm[8];
		} rdata_t;
		typedef union rxdata_t {
			rdata_t data;
			char raw[sizeof(rdata_t)];
		} rxdata_t;
	#endif


	#ifdef USBMBOARD_FIRMWARE_PRIVATE
		#define USART_UBRR ((F_CPU/USART_BAUD/16L)-1)
		#define USART_FLWCTRL
		#ifdef USART_FLWCTRL
			#define USART_PORT PORTH
			#define USART_PIN PINH
			#define USART_DDR DDRH
			#define USART_CTS PH3
			#define USART_RTS PH2
		#endif

		#define SPI_UBRR 0
		#define SPI_PORT PORTE
		#define SPI_PIN PINE
		#define SPI_DDR DDRE
		#define SPI_DAC_CS PE3
		#define SPI_MOSI PE1
		#define SPI_MISO PE0
		#define SPI_CLK PE2
		#define SPI_DAC_SHDN PE6
		#define SPI_DAC_LDAC PE7

		// This is probably a bug, since these macros should be defined in iomxx0_1.h.
		#define UDORD0 2
		#define UCPHA0 1
		#define UCPOL0 0


		#define TXBUF_SIZE 64
		#define TXBUF_MASK TXBUF_SIZE-1
		#define RXBUF_SIZE 64
		#define RXBUF_MASK RXBUF_SIZE-1
		#define RXBUF_MARGIN 0

		#define ADCBUF_MASK (_BV(REFS0) | _BV(REFS1) | _BV(ADLAR))
		#define ADCMUX_MASK (_BV(0) | _BV(1) | _BV(2))

		#define DAC0_MASK 0x30
		//#define DAC0_MASK 0x10 - Turns on reference voltage 2x multiplier.
		#define DAC1_MASK 0xB0
		//#define DAC1_MASK 0x90 - Turns on reference voltage 2x multiplier.

		#define abi(port, bit) (port) ^= _BV(bit)
		#define sbi(port, bit) (port) |= _BV(bit)
		#define cbi(port, bit) (port) &= ~_BV(bit)
		#define bv(port, bit) (port & _BV(bit))

		typedef unsigned int uint;
		typedef unsigned char uchar;

		typedef struct txbuf_t {
			char buf[TXBUF_SIZE];
			uint8_t size;
			uint8_t wr;
			uint8_t rd;
		} txbuf_t;
		static volatile txbuf_t tx;
		static volatile txdata_t txdata;

		typedef struct rxbuf_t {
			char buf[RXBUF_SIZE];
			uint8_t size;
			uint8_t wr;
			uint8_t rd;
		} rxbuf_t;
		static volatile rxbuf_t rx;
		static volatile rxdata_t rxdata;

		void usart_send_data(void);
        void usart_receive_data(void);
		void usart_putchar(char byte);
		char usart_getchar(void);

		void dac_set(uint8_t chan);
		void dout_set(void);
		void pwm_set(void);
		void din_get(void);

		void usart_init(void);
		void pwm_init(void);
		void adc_init(void);
		void dac_init(void);
		void io_init(void);
	#endif


#endif
