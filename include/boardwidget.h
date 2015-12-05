/*
QBoardWidget - Controller widget that manages USB Measuring Board
PCB based on FT232 and ATmega1280 ICs.

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

#ifndef QBOARDWIDGET_H
#define QBOARDWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QMessageBox>
#include <QTimer>

#include "ftdidevice.h"
#include "ui_boardwidget.h"

#define USBMBOARD_FIRMWARE_SHARED
#include <stdint.h>
#include "firmware.h"

class QBoardWidget : public QWidget, private Ui::QBoardWidget {
    Q_OBJECT
public slots:
    void connect();
    void disconnect();
    void transmit();

signals:
    void boardConnected(bool flag);
    void boardFailed(int error);

public:
    QBoardWidget(QWidget* parent = NULL);
    void changeEvent(QEvent *event);


    void storeSettings(QSettings &settings) const;
    void restoreSettings(QSettings &settings);

    void setBoard(QFtdiDevice* board);
    QFtdiDevice* getBoard() const;
    void setReferenceVoltage(const double referenceVoltage);
    double getReferenceVoltage() const;

    static const struct QBoardWidgetDefault {
        static double ReferenceVoltage() { return 2.500; }
    } Default;

private slots:
    void deviceConnected(bool flag);
    void deviceFailed(int error);
    void diSync();
    void aiSync();
    void doSetup();
    void doSync(const int sender = SignalMap.SpinBox);
    void aoSetup();
    void aoSync(const int sender = SignalMap.SpinBox);
    void pwmSetup();
    void pwmSync(const int sender = SignalMap.SpinBox);

private:
    QSignalMapper* doSignalMapper;
    QSignalMapper* aoSignalMapper;
    QSignalMapper* pwmSignalMapper;

    QTimer* timer;
    QFtdiDevice* board;
    double referenceVoltage;

    rxdata_t rxdata;
    txdata_t txdata;

    static const struct QBoardWidgetRange {
        static const int DI = (1 << 8) - 1;
        static const int AI = (1 << 10) - 1;
        static const int DO = (1 << 8) - 1;
        static const int AO = (1 << 12) - 1;
        static const int PWM = (1 << 8) - 1;
    } Range;

    static const struct QBoardWidgetSettingKey {
        static QString Board() { return QString("Board"); }
        static QString ReferenceVoltage() { return QString("ReferenceVoltage"); }
    } SettingKey;

    static const struct QBoardWidgetSignalMap {
        static const int CheckBox = 1;
        static const int SpinBox = 2;
        static const int Slider = 3;
    } SignalMap;
};

#endif // QBOARDWIDGET_H
