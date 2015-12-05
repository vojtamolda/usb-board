/*
QBoardSettings - Settings dialog that detects and sets up USB Measuring Boards.

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

#ifndef QBOARDSETTINGS_H
#define QBOARDSETTINGS_H

#include <QMessageBox>
#include <QSettings>
#include <QDialog>

#include "ui_boardsettings.h"
#include "ftditablemodel.h"
#include "boardwidget.h"
#include "ftdidevice.h"

class QBoardSettings : public QDialog, private Ui::QBoardSettings {
    Q_OBJECT
public slots:
    void accept();
    void refreshBoards();
    void restoreDefaults();

public:
    QBoardSettings(QBoardWidget* boardWidget, QWidget *parent = NULL);
    void changeEvent(QEvent *event);

private slots:
    void syncVid();
    void syncPid();
    void syncBoard(QModelIndex currentSelection, QModelIndex pastSelection);
    void syncReferenceVoltage();

    void setPid(const int pid);
    void setVid(const int vid);
    void setBoard(QFtdiDevice* board);
    void setReferenceVoltage(const double referenceVoltage);

private:
    int vid;
    int pid;
    double referenceVoltage;

    QFtdiDevice* board;
    QBoardWidget* boardWidget;
    QFtdiDeviceTableModel* deviceTableModel;
};

#endif // QBOARDSETTINGS_H
