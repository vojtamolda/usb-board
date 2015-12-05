/*
ATmega168 - Breadboard Prototype Application.

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

#include <ftdi.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#define ATMEGA168_FIRMWARE_SHARED
#include "firmware/atmega168.h"


void delay(unsigned int msec);
void terminate(int retval);


struct ftdi_context ftdic;


int main(int argc, char **argv) {
	rxdata_t rxdata;
	txdata_t txdata;

	signal(SIGINT, terminate);

	if (ftdi_init(&ftdic) < 0) {
		fprintf(stderr, "FTDI init failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 1;
	}
	if (ftdi_usb_open(&ftdic, 0x0403, 0x6001) < 0) {
		fprintf(stderr, "USB open failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 2;
	}
 	if (ftdi_set_baudrate(&ftdic, USART_BAUD) < 0) {
 		fprintf(stderr, "Set baud rate failed - %s.\n", ftdi_get_error_string(&ftdic));
 		return 3;
 	}
	if (ftdi_set_line_property(&ftdic, BITS_8, STOP_BIT_1, NONE) < 0) {
		fprintf(stderr, "Set line property failed- %s.\n", ftdi_get_error_string(&ftdic));
		return 4;
	}

	while (1) {

		rxdata.data.dout[0] = 255;
		rxdata.data.dout[1] = 255;

		rxdata.data.dac[0] = 4095;
		rxdata.data.dac[1] = 2048;

		rxdata.data.pwm[0] = 128;
		rxdata.data.pwm[1] = 128;
		rxdata.data.pwm[2] = 0;
		rxdata.data.pwm[3] = 0;
		rxdata.data.pwm[4] = 128;
		rxdata.data.pwm[5] = 128;
		rxdata.data.pwm[6] = 128;
		rxdata.data.pwm[7] = 128;

		if (ftdi_write_data(&ftdic, (unsigned char*) rxdata.raw, sizeof(rdata_t)) < 0) {
			fprintf(stderr, "Read data failed - %s\n", ftdi_get_error_string(&ftdic));
			return 5;
		}

		delay(100);

		if (ftdi_read_data(&ftdic, (unsigned char*) txdata.raw, sizeof(tdata_t)) < 0) {
			fprintf(stderr, "Read data failed - %s\n", ftdi_get_error_string(&ftdic));
			return 6;
		} else {
			printf("Analog inputs: %4d %4d %4d %4d %4d %4d.\n",
					txdata.data.adc[0], txdata.data.adc[1], txdata.data.adc[2],
					txdata.data.adc[3], txdata.data.adc[4], txdata.data.adc[5]);
			printf("Digital inputs: %3d %3d.\n", txdata.data.din[0], txdata.data.din[1]);
		}
	}

	terminate(0);
}


void delay(unsigned int msec) {
	#ifdef _WIN32
		Sleep(msec);
	#else
		usleep(msec*1000);
	#endif
}


void terminate(int retval) {
	printf("\nTerminating...\n");
	if (ftdi_usb_close(&ftdic) < 0) {
		fprintf(stderr, "USB close failed - %s.\n", ftdi_get_error_string(&ftdic));
		retval = 7;
	}
	ftdi_deinit(&ftdic);
	exit(retval);
}
