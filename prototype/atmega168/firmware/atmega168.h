/*
ATmega168 - Breadboard Prototype Firmware.

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

#ifndef ATMEGA168_FIRMWARE_H
	#define ATMEGA168_FIRMWARE_H


	#ifdef ATMEGA168_FIRMWARE_SHARED
		#define USART_BAUD 115200
		#define DACBUF_SIZE 2
		#define ADCBUF_SIZE 6

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


	#ifdef ATMEGA168_FIRMWARE_PRIVATE
		#define USART_UBRR ((F_CPU+USART_BAUD*8L)/(USART_BAUD*16L)-1)
		#define USART_FLWCTRL
		#ifdef USART_FLWCTRL
			#define USART_PORT PORTD
			#define USART_PIN PIND
			#define USART_DDR DDRD
			#define USART_CTS PD3
			#define USART_RTS PD2
		#endif

		#define SPI_PORT PORTB
		#define SPI_PIN PINB
		#define SPI_DDR DDRB
		#define SPI_DAC_CS PB2
		#define SPI_MOSI PB3
		#define SPI_MISO PB4
		#define SPI_CLK PB5
		#define SPI_DAC_SHDN PB0
		#define SPI_DAC_LDAC PB1

		#define TXBUF_SIZE 32
		#define TXBUF_MASK TXBUF_SIZE-1
		#define RXBUF_SIZE 32
		#define RXBUF_MASK RXBUF_SIZE-1
		#define RXBUF_MARGIN 0

		#define ADCBUF_MASK (_BV(REFS0) | _BV(REFS1) | _BV(ADLAR))

		#define DAC0_MASK 0x30
		//#define DAC0_MASK 0x10 - Turns on reference voltage 2x multiplier.
		#define DAC1_MASK 0xB0
		//#define DAC1_MASK 0x90 - Turns on reference voltage 2x multiplier.

		#define DAC_TR_START 0
		#define DAC_TR_END 1

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

		void dac_set(char flag);
		void dout_set(void);
		void pwm_set(void);
		void din_get(void);

		void usart_init(void);
		void pwm_init(void);
		void adc_init(void);
		void spi_init(void);
		void io_init(void);
	#endif


#endif
