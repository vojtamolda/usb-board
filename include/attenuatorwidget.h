/*
QAttenuatorWidget - Controller widget that manages USB Signal Attenuator
PCB based on FT245 IC.

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

#ifndef QATTENUATORWIDGET_H
#define QATTENUATORWIDGET_H

#include <QSignalMapper>
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QSettings>
#include <QWidget>
#include <QMenu>

#include "ftdidevice.h"
#include "ftditablemodel.h"
#include "ui_attenuatorwidget.h"

class QAttenuatorWidget : public QWidget, private Ui::QAttenuatorWidget {
    Q_OBJECT
public slots:
    void connect();
    void disconnect();
    void transmit(const int attenuation);
    void toggleSettings(bool state);
    void refreshAttenuators();

public:
    QAttenuatorWidget(QWidget *parent = NULL);
    void changeEvent(QEvent *event);

    void setAttenuator(QFtdiDevice* attenuator);
    QFtdiDevice* getAttenuator() const;

    void storeSettings(QSettings &settings) const;
    void restoreSettings(QSettings &settings);

signals:
    void attenuatorConnected(bool state);
    void attenuatorFailed(int error);

private slots:
    void deviceConnected(bool state);
    void deviceFailed(int error);

    void syncAttenuator(QModelIndex currentSelection, QModelIndex pastSelection);

private:
    QFtdiDevice* attenuator;
    QFtdiDeviceTableModel* deviceTableModel;

    static const struct QAttenuatorWidgetDefault {
        static const int Vid = 0x0403;
        static const int Pid = 0x6001;
        static const int Attenuation = 0;
    } Default;

    static const struct QAttenuatorWidgetRange {
        static const int Attenuation = (1 << 8) - 1;
    } Range;

    static const struct QAttenuatorWidgetSettingKey {
        static QString Attenuator() { return QString("Attenuator"); }
        static QString Attenuation() { return QString("Attenuation"); }
    } SettingKey;
};

#endif // QATTENUATORWIDGET_H
