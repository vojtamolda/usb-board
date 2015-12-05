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

#ifndef QFTDIDEVICE_H
#define QFTDIDEVICE_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QList>

#include <ftdi.h>

class QFtdiDevice: public QObject {
    Q_OBJECT
public slots:
    int connect(const char bitbangIOMask);
    int connect(const int baudrate, const uchar latency,
                const uint txchunk, const uint rxchunk);
    int disconnect();
    int transmit(const uchar* buffer, const int size);
    int receive(uchar* buffer, const int size);

signals:
    void deviceConnected(bool state);
    void deviceFailed(int error);

public:
    QFtdiDevice(QSettings &settings, QObject* parent = NULL);
    QFtdiDevice(const int vid, const int pid, const QString &serial,
                const QString &vendor, const QString &product, QObject* parent = NULL);
    ~QFtdiDevice();
    bool operator==(const QFtdiDevice &other) const;

    void storeSettings(QSettings &settings) const;
    static QList<QFtdiDevice*>* DeviceList(const int vid = Default.Vid, const int pid = Default.Pid,
                                           QObject* parent = NULL);

    bool isInited() const;
    bool isConnected() const;

    int getVid() const;
    int getPid() const;
    QString getSerial() const;
    QString getVendor() const;
    QString getProduct() const;
    QString getErrorMessage();

    static const struct QFtdiDeviceError {
        static const int None = 0;
        static const int InitFailed = 1;
        static const int NotInited = 2;
        static const int ConnectFailed = 3;
        static const int NotConnected = 4;
        static const int TransmitFailed = 5;
        static const int ReceiveFailed = 6;
        static const int DisconnectFailed = 7;
    } Error;

    static const struct QFtdiDeviceDefault {
        static const int Vid = 0x0403;
        static const int Pid = 0x6001;
    } Default;

private:
    bool inited;
    bool opened;

    const int vid;
    const int pid;
    const QString serial;
    const QString vendor;
    const QString product;

    struct ftdi_context context;

    int init();
    int deinit();

    static const struct QFtdiDeviceSettingKey {
        static QString Vid() { return QString("Vid"); }
        static QString Pid() { return QString("Pid"); }
        static QString Serial() { return QString("Serial"); }
        static QString Vendor() { return QString("Vendor"); }
        static QString Product() { return QString("Product"); }
    } SettingKey;
};

#endif // QFTDIDEVICE_H
