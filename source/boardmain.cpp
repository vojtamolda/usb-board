/*
Qt GUI application that controls USB Measuring Board based on FT232 USB to
Serial Port converter and ATmega1280 8-bit microcontroller. The board was
typically used to control experiments at the Department of Low-Temperature
Physics, Charles University, Prague.

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

#include "boardwindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    application.setOrganizationName("KFNT MFF UK");
    application.setOrganizationDomain("kfnt.mff.cuni.cz");
    application.setApplicationName("USB Measuring Board");
    application.setApplicationVersion("1.1");
    #ifdef TARGET_OS_MAC
      application.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    #endif

    QBoardWindow window;
    window.show();
    return application.exec();
}
