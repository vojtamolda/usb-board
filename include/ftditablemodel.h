/*
QFtdiDeviceTableModel - Model that provides available QFtdiDevices as
a data source for QTableView.

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

#ifndef QTABLEMODEL_H
#define QTABLEMODEL_H

#include <QAbstractTableModel>
#include <QTableView>
#include <QList>

#include "ftdidevice.h"

class QFtdiDeviceTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QFtdiDeviceTableModel(QTableView *parentView);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void refreshDeviceList(const int vid = QFtdiDevice::Default.Vid, const int pid = QFtdiDevice::Default.Pid);
    QFtdiDevice* getDevice(const QModelIndex &index) const;
    QModelIndex getIndex(QFtdiDevice* const device) const;

private:
    QList<QFtdiDevice*>* deviceList;

    static const struct QFtdiDeviceTableColumn {
        static const int Vendor = 0;
        static const int Product = 1;
        static const int SerialNumber = 2;
        static const int Count = 3;
    } Column;
};

#endif // QTABLEMODEL_H
