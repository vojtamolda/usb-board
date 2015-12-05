# USB Measuring Board


## How does it work?
Essentially, you stick USB cable into the gadget one side and whole bunch of input and output signals start to blink on the other side. To make the device work two things are needed - some hardware and an application that can talk to that hardware.

![diagram](/doc/image/board.ep.png "Diagram")


## Application
Under the hood of the *USB Measuring Board* application sits the [*Qt* framework](http://qt.io). Hardware communication uses [*libftdi*](http://www.intra2net.com/en/developer/libftdi/index.php) and [*libusb*](http://libusb.info/). These libraries allow the USB communication code to be written only once, since they provide an abstraction layer from the underlying OS specific API.

![application](/doc/image/board.png "Application")

For detailed instructions on how to compile the application, please see the [readme.md](readme.md) file. No worries. It's not as complicated as it sounds.


## Firmware
Key feature of the board design is the ability to program the microcontroller directly via the USB cable, without the need for any external programmer. This allows quick development cycles and easy firmware updates once the board is deployed into production. 

Controlling application running on the PC periodically polls the board. The packet going into the device contains fresh values of digital and analog outputs the user has set. Board sends back measured values of digital and analog inputs which are then displayed to the user. 

After receiving a data packet from the serial port firmware running on the ATmega1280 sets the desired value of all outputs and PWMs. Then it sends back the values of analog and digital inputs.


## Electronics
All the magic happens thanks to just two ICs - ATmega 1280 and FT232 IC. ATmega 1280 microcontroller is the brain of the operation and FT232 provides USB communication. Other parts are there just to help these two do their job. You can read tons more details about both ICs in their datasheets [here](http://www.atmel.com/Images/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf) and [here](http://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT245R.pdf).


#### Schematics
Following picture shows schematics of the ATmega breadboard and list of used ICs and their function:

![schematics](/pcb/board.sch.png "Schematics")

The PCB contains FT232 (*IC2*) chip which works as a black-box with USB (2.0 480Mb/s) on one side and serial port (RS-232) on the other side. The logic of the communication is handled in the ATmega1280 (*IC1*) microcontroller. The rest of the parts is there just to support the microcontroller to do its job.

Outputs of ATmega1280 can be loaded only with 20mA current. Because of this limitations all outputs are provided with power amplification. Darlington driver ULN2803A (*IC6,8*) can handle 500mA and ULN2064B outputs are capable of 1.5A. Two analog outputs are provided by MCP4922P (*IC3*) 12-bit analog converter. The MCP4922P and the ATmega microprocessor communicate via SPI bus.

Digital inputs are galvanically separated from the board to reduce the risk of damage. Two kinds of optocouplers are used - PC849 (*OK1-2*) and HCPL2630 (*OK3-6*).

#### Printed Circuit Board
The PCB is designed as two-sided and tolerances are relatively large, so it can be manufactured via traditional laboratory optical etching mask.

| IC               | Interface | Function
|------------------|-----------|---------------------------------------------------------
| ATmega1280 (IC1) | UART      | RISC 8-bit microcontroller (running at 16 MHz)
| FT232R (IC2)     | USB       | USB to UART converter
| MCP4922P (IC3)   | SPI       | Digital to analog converter (12-bit precision)
| CLM385M (IC4)    | N/A       | Voltage 2.5V reference for digital-analog and analog-digital converters
| ULN2803A (IC6,8) | GPIO      | Darlington driver for digital outputs (500mA max. current)
| ULN2064B (IC5,7) | GPIO      | Darlington driver for digital outputs (1.5A max. current)
| PC849 (OK1-2)    | GPIO      | Optocoupler for galvanic separation of digital inputs (4 channel)
| HCPL2630 (OK3-6) | GPIO      | Optocoupler for galvanic separation of digital inputs (2 channel)

![pcb](/pcb/board.brd.png "Printed Circuit Board")

 - Dimensions 140 x 135 x 20 mm / 5.5 x 5.3 x 0.8 in
 - Operating temperature range: -40 to +70 °C / -40 to +160 °F
 - Weight: 130 g / 4.5 oz
