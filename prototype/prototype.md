# Breadboard Prototypes


## What's this?
Following page contains descriptions of three simple prototype applications that served as proof of concept. These files are here to show and document the development path (i.e. dead ends) towards [USB Attenuator](/doc/attenuator.md) and [USB Measuring Board](/doc/board.md).


## Discover
This application tests basic functionality of the *libftdi* library. It detects and prints out a detailed list of all available FTDI chips. Any (FT232/245/2232 etc..) known model is detected and listed.

![bitbang_application](/prototype/discover/discover.run.png "Discover Prototype Application")

If the application ends up with an error or doesn't detect any device, typically there's a collision between *libftdi* and OS kernel driver. Please, see the *Compilation* section of the [readme file](/readme.md) for more details on how to unload the colliding kernel driver for each OS.


## Bitbang
This prototype tests the bitbang mode. Bitbang mode allows direct access to pins of FT232R IC. Pins can be setup as either input or output. Based on the supplied argument, the application launches in two different modes -
input mode `i`, that reads state of connected switches or output mode `o`, which can for example blink LEDs.
 
![bitbang_application](/prototype/bitbang/bitbang.run.png "Bitbang Prototype Application")

Example connection schematics that can be used for input and output mode are shown on the following picture. In case of the output mode, every GPIO is connected to a 330Ω resistor and a LED. Input mode uses the same resistors as pull-downs and switches connect the GPIO lines to +5V.

![bitbang_schematics](/prototype/bitbang/bitbang.sch.png "Bitbang Prototype Schematics")

PS: I know 330 Ω pull-down resistors are a silly idea, since it wastes a ton of power when the switches are on, but I'm too lazy to change the breadboard and nobody cares about prototypes anyway :-)


## ATmega168
This breadboard prototype is a smaller brother of [USB Measuring Board](/doc/board.md). Instead of gigantic ATmega1280 with hundreds of legs, this prototype uses ATmega168 in DIP-28 package. Both microcontrollers use the same RISC instruction set, so the main difference is only the number of GPIO ports.

Key feature of the prototype is the ability to program the microcontroller directly via the USB cable, without the need for any external programmer. This allows for quick development cycles and easy firmware updates once the board is deployed into production.

![atmega168_application](/prototype/atmega168/atmega168.run.png "ATmega168 Prototype Application")

Application running on the PC and the firmware running on the ATmega microcontroller talk to each other via USB to serial port converter provided by the FT232R IC sitting on UMS3 module. UMS3 is just a convenient breakout DIP package for FT232R IC and it isn't used in the production models. After receiving data packet from the serial port ATmega168 firmware sets the values of analog outputs via SPI interface and sets the state of the digital output GPIO pins. After that it sends back the values of analog and digital inputs.

Controlling application running on the PC periodically scans and prints out the values of analog (10-bit value) and digital inputs to the terminal. Values of analog and digital outputs transmitted into the microcontroller are hard-coded into the source code.

Following picture shows schematics of the ATmega breadboard and list of used ICs and their function:

![atmega168_schematics](/prototype/atmega168/atmega168.sch.png "ATmega168 Prototype Schematics")

| IC        | Interface | Function
|-----------|-----------|---------------------------------------------------------
| ATmega168 | UART      | RISC 8-bit microcontroller (running at 20 MHz)
| UMS3      | USB       | USB to UART converter (breakout for FT232R)
| MCP4922P  | SPI       | Digital to analog converter (12-bit precision)
| ULN2803A  | GPIO      | Darlington driver for digital outputs (500mA max. current)
| 6N134D    | GPIO      | Optocoupler for galvanic separation of digital inputs
| CLM385M   | N/A       | Voltage 2.5V reference for digital-analog and analog-digital converters

Here's a photograph of the assembled breadboard in it's full beauty :) 

![atmega168_breadboard](/prototype/atmega168/atmega168.brd.jpg "ATmega168 Prototype Breadboard")

For more details, please see *Chapter 5 - Prototype* of the bachelor [thesis](/doc/thesis.pdf) (available only in Czech).
