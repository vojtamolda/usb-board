# USB Attenuator


## How does it work?
Essentially, you stick USB cable into the gadget one side and custom parallel port signals get out on the other side. To make the device work two things are needed - some hardware and an application that can talk to that hardware.

![diagram](/doc/image/attenuator.ep.png "Diagram")


## Application
Under the hood of the *USB Attenuator* application sits the [*Qt* framework](http://qt.io). Hardware communication uses [*libftdi*](http://www.intra2net.com/en/developer/libftdi/index.php) and [*libusb*](http://libusb.info/). These libraries allow the USB communication code to be written only once, since they provide an abstraction layer from the underlying OS specific API.

![application](/doc/image/attenuator.png "Application")

For detailed instructions on how to compile the application, please see the [readme.md](readme.md) file. No worries. It's not as complicated as it sounds.


## Electronics
All the magic happens thanks to the built-in bit-bang mode of the FT245 IC. Other parts are there just to help the FT245 do it's job. You can can read tons more details about the IC in the [datasheet](http://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT245R.pdf).

### Schematics
The PCB contains FT245 (*IC1*) chip which is essentially a black-box with USB (2.0 480Mb/s) on one side and parallel port (RS-485) on the other side. The rest of the parts is there just to make the FT245 work. The FT245 chip is a bit sclerotic and tends to forget its name (Vendor and Product IDs). Because of this there's a three-wire serial EEPROM (93C46 - *IC3*) that keeps the configuration data. Old fashioned RS-485 use +/- 5 V levels so the board is equipped with a voltage converter (ICL7660 - *IC2*). The rest is just standard connectors, filtering capacitors, ferrite cores and resistors.

![schematics](/pcb/attenuator.sch.png "Schematics")

### Printed Circuit Board
The PCB is designed as two-sided, but it's possible to use only one sided etching photomask and three jumper wires, which is a good compromise that prevents alignment problems that arise with two sided PCBs. Happy soldering!

| IC              | Interface | Function
|-----------------|-----------|-----------------------------------------------------------------
| FT245BL (IC1)   | USB       | USB to parallel port converter
| ICL7660SA (IC2) | N/A       | Negative voltage converter (outputs -5V)
| 93-C46ASN (IC3) | SPI       | Microwire compatible serial 1kB EEPROM (keeps USB descriptors)

![pcb](/pcb/attenuator.brd.png "Printed Circuit Board")

 - Dimensions 25 x 40 x 12 mm / 1.0 x 1.6 x 0.5 in
 - Operating temperature range: -40 to +70 °C / -40 to +160 °F
 - Weight: 3.8 g / 0.16 oz

