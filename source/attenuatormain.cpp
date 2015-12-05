/*
Qt GUI Application that controls USB Signal Attenuator based on FT245 USB to
Parallel Port converter. The attenautor was typically used to control
experiments at the Department of Low-Temperature Physics, Charles University,
Prague.

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

#include "attenuatorwindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    application.setOrganizationName("KFNT MFF UK");
    application.setOrganizationDomain("kfnt.mff.cuni.cz");
    application.setApplicationName("USB Attenuator");
    application.setApplicationVersion("1.0");
    #ifdef TARGET_OS_MAC
      application.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    #endif

    QAttenuatorWindow window;
    window.show();
    return application.exec();
}
