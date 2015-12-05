# USB Measuring Board & USB Attenuator

Wouldn't be cool to drive any experiment directly from your computer? Without the need to get up from your chair and touch things? Well, now your computer can directly flip switches, measure voltages, send signals and talk to other devices using the *USB Measuring Board* or *USB Attenuator*!

![board_diagram](/doc/image/readme.png "USB Measuring Board & USB Attenuator")
![board_pcb](/pcb/board.brd.png "Printed Circuit Board")

If your applications is simple and requires just a few inputs and outputs then [*USB Attenuator*](/doc/attenuator.md) is the right solution. [*USB Measuring Board*](/doc/board.md) is more versatil and can handle control of more complex experiments. See [board.md](/doc/board.md) and [attenuator.md](/doc/attenuator.md) readme files for more details.

Both projects were originally inspired by the needs of the [Department of Low-Temperature Physics, Charles University, Prague](http://kfnt.mff.cuni.cz), but obviously range of application is much wider. *USB Measuring Board* was developed as part of my [bachelor thesis](/doc/thesis.pdf) written in 2009. If you're interested in how it all works, take a look at the  [prototypes](/prototypes/prototypes.md).


## Compilation
So... How do I build this monster? It's not as complicated as it looks. Build chain uses *QMake* (part of [*Qt framework*](http://qt.io)). Besides *Qt*, you'll need two libraries that provide USB communication - [*libftdi*](http://www.intra2net.com/en/developer/libftdi/index.php) and [*libusb*](http://libusb.info/). Tested platforms are Mac OS X 10.11, Ubuntu Linux 14.04 LTS and Windows 8, but chances are it would run on any platform supported by *Qt* and *libusb*.

Firmware powering the ATmega microprocessor requires it's own version C compiler, linker and standard C libraries called [*avr-toolchain*](http://savannah.nongnu.org/projects/avr-libc/). To upload the firmware no special programmer is needed. Microprocessor programming can be done over the USB cable by [*avrdude*](http://savannah.nongnu.org/projects/avrdude/).

#### Mac OS X
- Download and install latest version of *Qt* 5.x libraries from [here](http://www.qt.io/download/).
- Install *libftdi* library. [Homebrew](http://brew.sh/) installs *libusb* automatically as a dependency.
```bash
   # Get Homebrew OS X package manager here: http://brew.sh/.
   brew install libftdi
```
- Install [*avr-gcc*](https://gcc.gnu.org/wiki/avr-gcc), [*avr-libc*](http://savannah.nongnu.org/projects/avr-libc/)  and [*avrdude*](http://savannah.nongnu.org/projects/avrdude/) required to compile and upload firmware.
```bash
   # Alternatively download and install CrossPack.
   brew install avrdude
   brew tap larsimmisch/avr
   brew install avr-libc
```
- Compile both projects with *QMake* and make.
```bash
   # Alternatively open the .pro file in Qt Creator IDE.
   qmake board.pro
   qmake attenuator.pro
   cd firmware; make;
```
- Remove colliding OS X kernel extension and run the compiled application.
```bash
   # Remove AppleUSBFTDI kernel extension to prevent 'Unable to claim USB device' errors.
   sudo kextunload -bundle-id com.apple.driver.AppleUSBFTDI
```

#### Linux
- Install your distribution's default version of *Qt* 5.x libraries.
```bash
   # DEB-based distributions. (Ubuntu, Debian...)
   apt-get install qt5-default
   # RPM-based distributions. (RedHat, SUSE...)
   yum install qt5-default
```
- Install development version of *libftdi-dev* library. Package manager installs *libusb-dev* automatically as a dependency.
```bash
   # DEB-based distributions.
   apt-get install libftdi-dev
   # RPM-based distributions.
   yum install libftdi-dev
```
- Install [*avr-gcc*](https://gcc.gnu.org/wiki/avr-gcc), [*avr-libc*](http://savannah.nongnu.org/projects/avr-libc/)  and [*avrdude*](http://savannah.nongnu.org/projects/avrdude/) required to compile and upload firmware.
```bash
   # DEB-based distributions.
   apt-get install avrdude avr-gcc avr-libc
   # RPM-based distributions.
   yum install avrdude avr-gcc avr-libc
```
- Compile both projects with *QMake*.
```bash
   # Alternatively open the .pro file in Qt Creator IDE.
   qmake board.pro
   qmake attenuator.pro
   cd firmware; make
```
- Remove conflicting kernel module and run the compiled application.
```bash
   # Unload kernel module to prevent 'Unable to claim USB device' errors.
   sudo rmmod ftdi_sio
```

#### Windows
- Download and install latest version of *Qt* 5.x libraries from [here](http://www.qt.io/download/).
- Download and compile libraries required for USB communication.
  - Source code and documentation for *libusb* is available [here](http://github.com/libusb/libusb/)
  - Source code and documentation for *libftdi* is available [here](http://www.intra2net.com/en/developer/libftdi/repository.php).
  - Copy freshly compiled static libraries and headers (*ftdi.h*, *usb.h*, *libftdi.lib* and *libusb.lib*) into *lib/win32/*.
- Download and install [*WinAVR*](http://winavr.sourceforge.net/) required to compile and upload firmware.
- Compile both projects with *QMake*.
```bat
   :: Alternatively open the .pro file in Qt Creator IDE.
   qmake.exe board.pro
   qmake.exe attenuator.pro
   cd firmware; make.exe
```


## Schematics and PCB Layouts
Circuit schematics (.sch) and PCB layouts (.brd) files are designed with [Eagle CAD](http://www.cadsoftusa.com/). Free version for non-commercial use version can be downloaded [here](http://www.cadsoftusa.com/download-eagle/).


## License
This work is licensed under the [GNU GPL 3.0 License](http://www.gnu.org/licenses/gpl-3.0.html) © 2009.
