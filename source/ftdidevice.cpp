/*
QFtdiDevice - Lightweight wrapper class to provide QT signal/slot
mechanism to control FT232/245 family of ICs.

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

#include "ftdidevice.h"

QFtdiDevice::QFtdiDevice(QSettings &settings, QObject* parent) :
    QObject(parent), inited(false), opened(false),
    vid(settings.value(SettingKey.Vid()).toInt()), pid(settings.value(SettingKey.Pid()).toInt()),
    serial(settings.value(SettingKey.Serial()).toString()),
    vendor(settings.value(SettingKey.Vendor()).toString()), product(settings.value(SettingKey.Product()).toString())
{ }

QFtdiDevice::QFtdiDevice(const int vid, const int pid, const QString &serial,
                         const QString &vendor, const QString &product, QObject* parent) :
    QObject(parent), inited(false), opened(false),
    vid(vid), pid(pid),
    serial(serial),
    vendor(vendor), product(product)
{ }

bool QFtdiDevice::operator==(const QFtdiDevice &other) const {
    return ((other.getVid() == this->getVid()) &&
            (other.getPid() == this->getPid()) &&
            (other.getSerial() == this->getSerial()));
}

int QFtdiDevice::init() {
    if (!inited) {
        if (ftdi_init(&context) < 0) {
            inited = false;
            emit deviceFailed(Error.InitFailed);
            return Error.InitFailed;
        }
    }
    inited = true;
    return Error.None;
}

int QFtdiDevice::connect(const char bitbangIOMask) {
    init();
    if (!opened) {
        if (!inited) {
            opened = false;
            emit deviceConnected(false);
            return Error.NotInited;
        }
        if (ftdi_usb_open_desc(&context, vid, pid, NULL, serial.toLatin1()) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_set_bitmode(&context, bitbangIOMask, BITMODE_BITBANG) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
    }
    opened = true;
    emit deviceConnected(true);
    return Error.None;
}

int QFtdiDevice::connect(const int baudrate, const uchar latency,
                         const uint txchunk, const uint rxchunk) {
    init();
    if (!opened) {
        if (!inited) {
            opened = false;
            emit deviceConnected(false);
            return Error.NotInited;
        }
        if (ftdi_usb_open_desc(&context, vid, pid, NULL, serial.toLatin1()) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_set_baudrate(&context, baudrate) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_set_line_property(&context, BITS_8, STOP_BIT_1, NONE) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_set_latency_timer(&context, latency) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_write_data_set_chunksize(&context, txchunk) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
        if (ftdi_read_data_set_chunksize(&context, rxchunk) < 0) {
            opened = false;
            emit deviceFailed(Error.ConnectFailed);
            emit deviceConnected(false);
            return Error.ConnectFailed;
        }
    }
    opened = true;
    emit deviceConnected(true);
    return Error.None;
}

int QFtdiDevice::transmit(const uchar* buffer, const int size) {
    if (opened) {
        if (ftdi_write_data(&context, (uchar*) buffer, size) < 0) {
            emit deviceFailed(Error.TransmitFailed);
            return Error.TransmitFailed;
        }
        return Error.None;
    } else {
        emit deviceFailed(Error.NotConnected);
        return Error.NotConnected;
    }
}

int QFtdiDevice::receive(uchar* buffer, const int size) {
    if (opened) {
        if (ftdi_read_data(&context, buffer, size) < 0) {
            emit deviceFailed(Error.ReceiveFailed);
            return Error.ReceiveFailed;
        }
        return Error.None;
    } else {
        emit deviceFailed(Error.NotConnected);
        return Error.NotConnected;
    }
}

void QFtdiDevice::storeSettings(QSettings &settings) const {
    settings.setValue(SettingKey.Vid(), vid);
    settings.setValue(SettingKey.Pid(), pid);
    settings.setValue(SettingKey.Serial(), serial);
    settings.setValue(SettingKey.Vendor(), vendor);
    settings.setValue(SettingKey.Product(), product);
}

int QFtdiDevice::disconnect() {
    if (opened) {
        if (ftdi_usb_close(&context) < 0) {
            opened = false;
            emit deviceFailed(Error.DisconnectFailed);
            emit deviceConnected(false);
            return Error.DisconnectFailed;
        }
    }
    opened = false;
    emit deviceConnected(false);
    return Error.None;
}

int QFtdiDevice::deinit() {
    if (inited) {
        ftdi_deinit(&context);
    }
    inited = false;
    return Error.None;
}

QFtdiDevice::~QFtdiDevice() {
    disconnect();
    deinit();
}

QList<QFtdiDevice*>* QFtdiDevice::DeviceList(const int vid, const int pid, QObject *parent) {
    struct ftdi_device_list* ftdi_device = NULL;
    struct ftdi_device_list* ftdi_device_list = NULL;
    static struct ftdi_context* ftdi_context = ftdi_new();
    QList<QFtdiDevice*>* deviceList = new QList<QFtdiDevice*>;

    if (ftdi_usb_find_all(ftdi_context, &ftdi_device_list, vid, pid) < 0) {
        return deviceList;
    }
    ftdi_device = ftdi_device_list;
    while (ftdi_device != NULL) {
        char serial[64];
        char vendor[128];
        char product[128];
        if (ftdi_usb_get_strings(ftdi_context, ftdi_device->dev,
                vendor, sizeof(vendor), product, sizeof(product), serial, sizeof(serial)) >= 0) {
            deviceList->append(new QFtdiDevice(vid, pid, serial, vendor, product, parent));
        }
        ftdi_device = ftdi_device->next;
    }
    ftdi_list_free(&ftdi_device_list);
    return deviceList;
}

bool QFtdiDevice::isInited() const {
    return inited;
}

bool QFtdiDevice::isConnected() const {
    return opened;
}

int QFtdiDevice::getVid() const {
    return vid;
}

int QFtdiDevice::getPid() const {
    return pid;
}

QString QFtdiDevice::getSerial() const {
    return serial;
}

QString QFtdiDevice::getVendor() const {
    return vendor;
}

QString QFtdiDevice::getProduct() const {
    return product;
}

QString QFtdiDevice::getErrorMessage() {
    if (!inited) return QString();
    return QString(ftdi_get_error_string(&context));
}
