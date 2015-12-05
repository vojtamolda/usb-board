/*
Discover - Find All Connected FT232 Chips.

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


int main(int argc, char *argv[]) {
	int retval;
	
	char serial[64];
	char vendor[128];
	char product[128];
	unsigned int chipid;
	
	struct ftdi_context ftdic;
	struct ftdi_device_list *curdev;
	struct ftdi_device_list *devlist;

	if (ftdi_init(&ftdic) < 0) {
		fprintf(stderr, "FTDI init failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 1;
	}
	if ((retval = ftdi_usb_find_all(&ftdic, &devlist, 0x0403, 0x6001)) < 0) {
		fprintf(stderr, "USB discovery failed - %s.\n", ftdi_get_error_string(&ftdic));
		return 2;
	}

	printf("Discovered FTDI devices (%d) :\n\n", retval);

	for (curdev = devlist; curdev != NULL; curdev = curdev->next) {
		if ((retval = ftdi_usb_get_strings(&ftdic, curdev->dev, vendor, sizeof(vendor), product, sizeof(product), serial, sizeof(serial))) < 0) {
			fprintf(stderr, "USB descriptors acquire failed - %s\n.", ftdi_get_error_string(&ftdic));
			return 3;
		}
		if((retval = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0) {
			fprintf(stderr, "Device open failed - %s.\n", ftdi_get_error_string(&ftdic));
			return 4;
		}
		if (ftdi_read_chipid(&ftdic, &chipid) < 0) {
			fprintf(stderr, "Read FTDI ChipID failed - %s.\n", ftdi_get_error_string(&ftdic));
			return 5;
		}

		printf("Manufacturer  : %s\n", vendor);
		printf("Product       : %s\n", product);
		printf("Serial Number : %s\n", serial);
		printf("Chip ID       : %u\n", chipid);
		printf("\n");
	}

	ftdi_usb_close(&ftdic);
	ftdi_list_free(&devlist);
	ftdi_deinit(&ftdic);

	return 0;
}
