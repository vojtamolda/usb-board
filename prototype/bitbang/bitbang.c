/*
Bitbang - Test FT232 Bitbang Mode.

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
#include <signal.h>
#include <stdlib.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif


void usage();
void delay(unsigned int msec);
void terminate(int retval);


struct ftdi_context ftdic;


int main(int argc, char *argv[]) {
	unsigned char buf;
	unsigned char bitmask;

	signal(SIGINT, terminate);

	if (argc != 2) {
		usage();
		return 1;
	}

	if (ftdi_init(&ftdic) < 0) {
		fprintf(stderr, "FTDI init failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 2;
	}
	if (ftdi_usb_open(&ftdic, 0x0403, 0x6001) < 0) {
		fprintf(stderr, "USB open failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 3;
	}

	/* Individual bytes define the bitbang mode to:
		+ HIGH/ON/1 - output
		+ LOW/OFF/0 - input */
	switch (argv[1][0]) {

		case 'o':
			bitmask = 0xFF;
			if (ftdi_set_bitmode(&ftdic, bitmask, BITMODE_BITBANG) < 0) {
				fprintf(stderr, "Enable bitbang failed - %s.\n", ftdi_get_error_string(&ftdic));
				return 4;
			}
			for (buf = 0xFF; 1 ; buf--) {
				if (ftdi_write_data(&ftdic, &buf, 1) < 0) {
					fprintf(stderr,"Write data failed - %s.\n", ftdi_get_error_string(&ftdic));
					return 5;
				}
				printf(".");
				fflush(stdout);
				delay(100);
			}
			break;

		case 'i':
			bitmask = 0x00;
			if (ftdi_set_bitmode(&ftdic, bitmask, BITMODE_BITBANG) < 0) {
				fprintf(stderr, "Enable bitbang failed - %s.\n", ftdi_get_error_string(&ftdic));
				return 4;
			}
			for (buf = 0x00; 1 ; buf++) {
				if (ftdi_read_pins(&ftdic, &buf) < 0) {
					fprintf(stderr,"Read pins failed - %s.\n", ftdi_get_error_string(&ftdic));
					return 5;
				}
				printf("%i ",buf);
				fflush(stdout);
				delay(500);
			}
			break;

		default:
			usage(argv[0]);
			return 6;
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


void usage() {
	printf("Usage: bitbang {o|i}\n");
  printf("  o - Output mode - Blinks leds in a fancy way :-).\n");
  printf("  i - Input mode - Reads state of all lines.\n");
  printf("Press CTRL-C to terminate.\n");
}


void terminate(int retval) {
	printf("\nTerminating...\n");
	if (ftdi_disable_bitbang(&ftdic) < 0) {
		fprintf(stderr, "Disable bitbang failed - %s.\n", ftdi_get_error_string(&ftdic));
		retval = 7;
	}
	if (ftdi_usb_close(&ftdic) < 0) {
		fprintf(stderr, "USB close failed - %s.\n", ftdi_get_error_string(&ftdic));
		retval = 8;
	}
	ftdi_deinit(&ftdic);
	exit(retval);
}
