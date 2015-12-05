; This examples demonstrates how libusb's drivers
; can be installed automatically along with your application using an installer.
;
; Requirements: Inno Setup (http://www.jrsoftware.org/isdl.php)
;
; To use this script, do the following:
; - copy libusb's driver (libusb0.sys, libusb0.dll) to this folder
; - create an .inf and .cab file using libusb's 'inf-wiward.exe'
;   and save the generated files in this folder.
; - in this script replace <your_inf_file.inf> with the name of your .inf file
; - customize other settings (strings)
; - open this script with Inno Setup
; - compile and run
[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; LicenseFile: "license.txt"
Name: "cs"; MessagesFile: "compiler:Languages\Czech.isl"; LicenseFile: "license.txt"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"; LicenseFile: "license.txt"
Name: "ge"; MessagesFile: "compiler:Languages\German.isl"; LicenseFile: "license.txt"

[CustomMessages]
en.TranslatedAppName=USB Attenuator
cs.TranslatedAppName=USB Attenuátor
fr.TranslatedAppName=USB Attenuateur
ge.TranslatedAppName=USB Attenuator

[Setup]
AppName={cm:TranslatedAppName}
AppVerName={cm:TranslatedAppName} 1.0
AppPublisher=Vojta Molda
AppPublisherURL = http://kfnt.mff.cuni.cz/
AppVersion=1.0
DefaultDirName={pf}\{cm:TranslatedAppName}
DefaultGroupName={cm:TranslatedAppName}
Compression=lzma
SolidCompression=yes
; Support only Windows 2000 or higher
MinVersion=5,5
PrivilegesRequired=admin
UninstallFilesDir="{app}"

[Files]
; Copy the file to the App folder
Source: "target\win32\*"; DestDir: "{app}"
Source: "lib\win32\driver\*"; DestDir: "{app}\driver"


; also copy the DLL to the system folder so that rundll32.exe will find it
Source: "driver\*.dll"; DestDir: "{win}\system32"; FLags: replacesameversion restartreplace uninsneveruninstall

[Icons]
Name: "{group}\{cm:TranslatedAppName}"; Filename: "{app}\attenuator.exe"; WorkingDir: "{app}"
Name: "{group}\{cm:UninstallProgram,{cm:TranslatedAppName}}"; Filename: "{uninstallexe}"

[Run]
; Invoke libUSB's DLL to install the .inf file
Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {app}\attenuator.inf"; StatusMsg: "Installing driver (this may take a few seconds) ..."

